//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <numeric>
Define_Module(Node);

# define INFECTION_FRMT "Infetto %d"

void Node::sendAll(){

    int n = par("numberOfNodes");
    for(int i = 0; i<n; ++i ){
       char str[20];
       sprintf(str, INFECTION_FRMT, infectionHop);

        if(isReachable[i]){
            cMessage * mess = new cMessage(str);
            send(mess,"out",i);
        }
        EV<<"INVIATO "<<str<<endl;
    }
    hasInfected=true;
    colorNode((char*)"red");
}

void Node::scheduleClock(){
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
}

void Node::initialize()
{
    self_id = getIndex();
    int numberOfNodes = par("numberOfNodes");
    isReachable = new bool[numberOfNodes]();
    char str[8];
    double pos_x = par("pos_x");
    double pos_y = par("pos_y");
    double other_x, other_y;
    double radius_squared = (double)par("radius") * (double)par("radius");
    for (int i = 0; i < numberOfNodes; ++i){
        sprintf(str, "nodeX[%d]", i);
        cModule* other = getModuleByPath(str);
        other_x = other->par("pos_x");
        other_y = other->par("pos_y");
        double distance = (other_x - pos_x)*(other_x - pos_x) +
                (other_y - pos_y)*(other_y - pos_y);
        isReachable[i] = (i != self_id && radius_squared >= distance);
//        EV << self_id << "(" << pos_x << "," << pos_y << ")"
//                << " " << i << "(" << other_x << "," << other_y << ")"
//                << " -> " << isReachable[i]
//                << " because distance: " << distance
//                << " radius_squared: " << radius_squared << endl;
        cDisplayString& connDispStr = gate("out", i)->getDisplayString();
        if (isReachable[i])
            connDispStr.parse("ls=black");
        else
            connDispStr.parse("ls=red,0;");
    }
    neighborsSignal = registerSignal("neighbors");
    endTimeSignal = registerSignal("endTime");
    collisionSignal = registerSignal("collision");
    hopSignal = registerSignal("hop");

    int sumNeighbors = 0;
    for (int i = 0; i < numberOfNodes; ++i){
        sumNeighbors += isReachable[i];
    }
    //EV << self_id << " neighbor count: " << sumNeighbors << endl;
    if(par("firstInfected")){
        sendAll();
        emit(endTimeSignal, 0.0);
        emit(hopSignal, infectionHop);
    } else {
        scheduleClock();
        emit(endTimeSignal, -1.0);
        emit(hopSignal, -1);
    }
    /*======statistics======*/

    emit(neighborsSignal, sumNeighbors);
}

void Node::colorNode(char* color){
    char str[50];
    cDisplayString& nodeDispStr = getDisplayString();
    sprintf(str, "p=$pos_x,$pos_y;i=block/process,%s,50", color);
    nodeDispStr.parse(str);
}

void Node::handleMessage(cMessage *msg)
{
    if(hasInfected){ // node is disabled after completing his job
        goto end;
    }
    if(!strcmp("clock", msg->getName())){ // new time slot
        collisionCheck = false;
        collisionOccurred = false;
        if(receivedInfection){
            if (!hasValidMsg){
                emit(endTimeSignal, simTime());
                emit(hopSignal, infectionHop);
            }
            hasValidMsg = true;
            double probability=par("sendingProbability");
            double limit=par("limitProbability");
            EV<<"INFETTO "<<self_id<<endl;
            if(probability<=limit){
                sendAll();
                /*====statistics====*/
                goto end;
            }
            colorNode((char*)"blue");
        } else {
            colorNode((char*)"lightgrey");
        }
        scheduleClock();
    }
    else if (!hasValidMsg){ // infection from other node
        if(!collisionCheck){
            collisionCheck = true;
            receivedInfection = true;
            sscanf(msg->getName(), INFECTION_FRMT, &infectionHop);
            infectionHop++;
            EV << "received" << msg->getName() << " -> " << infectionHop << endl;
            colorNode((char*)"green");
        }
        else{
            EV<<"COLLISIONE "<<getName()<<endl;
            receivedInfection = false;
            colorNode((char*)"orange");
            if (!collisionOccurred){
                emit(collisionSignal, 1);
                collisionOccurred = true;
            }
        }

    }
    end:
        delete(msg);
}

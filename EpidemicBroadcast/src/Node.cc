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

void Node::initialize()
{
    //statCollectorRef = check_and_cast<StatCollector*>(getModuleByPath("statCollector"));
    self_id = getIndex();
    limitProbability=par("limitProbability");
    int sumNeighbors = 0;
    numberOfNodes = par("numberOfNodes");

    setupAdjacencyList();
    for (int i = 0; i < numberOfNodes; ++i){
        sumNeighbors += isReachable[i];
    }

    neighborsSignal = registerSignal("neighbors");
    endTimeSignal = registerSignal("endTime");
    collisionSignal = registerSignal("collision");
    hopSignal = registerSignal("hop");
    //EV << self_id << " neighbor count: " << sumNeighbors << endl;
    if(par("firstInfected")){
        sendAll(0.0);
        star = Done;
        colorNode();
    } else {
        scheduleClock();
        star = Waiting;
        emit(endTimeSignal, -1.0);
        emit(hopSignal, -1);
    }
    emit(neighborsSignal, sumNeighbors);
}

void Node::setupAdjacencyList()
{
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
}

void Node::scheduleClock()
{
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
}

void Node::sendAll(double time)
{
    for(int i = 0; i < numberOfNodes; ++i ){
       char str[20];
       sprintf(str, INFECTION_FRMT, infectionHop);

        if(isReachable[i]){
            cMessage * mess = new cMessage(str);
            send(mess,"out",i);
        }
        EV<<"INVIATO "<<str<<endl;
    }
    //hasInfected=true;
//    colorNode((char*)"red");

    emit(endTimeSignal, time);
    emit(hopSignal, infectionHop);
}

char* STAR2COLOR[5] = {(char*)"lightgrey", (char*)"green", (char*)"orange", (char*)"blue", (char*)"red"};

void Node::colorNode()
{
    char str[50];
    char* color;
    cDisplayString& nodeDispStr = getDisplayString();
    //char** arr = new arr*["lightgrey", "green", "orange", "blue", "red"]();
    color = STAR2COLOR[star];
//    switch (star){
//        case OneMessage:
//            color = (char*)"green"; break;
//        case Collision:
//            color = (char*)"orange"; break;
//        case Ready:
//            color = (char*)"blue"; break;
//        case Done:
//            color = (char*)"red"; break;
//        default:
//            color = (char*)"lightgrey";
//    }
    sprintf(str, "p=$pos_x,$pos_y;i=block/process,%s,50", color);
    nodeDispStr.parse(str);
}

Status Node::getStatus(){
    return star;
}

Status Node::lottery()
{
    sendingProbability=par("sendingProbability");
    if(sendingProbability<=limitProbability){
        sendAll(simTime().dbl());
        return Done;
    }
    return Ready;
}

void Node::handleMessage(cMessage* msg)
{
    if(!strcmp("clock", msg->getName())){
        switch (star) {
            case OneMessage:
            case Ready:
                star = lottery(); break;
            case Collision:
                star = Waiting; break;
            default:
                break;
        }
    } else {
        switch (star) {
            case Waiting:
                sscanf(msg->getName(), INFECTION_FRMT, &infectionHop);
                infectionHop++;
                EV << "received" << msg->getName() << " -> " << infectionHop << endl;
                star = OneMessage; break;
            case OneMessage:
                emit(collisionSignal, 1);
                star = Collision; break;
            default:
                break;
        }
    }
    colorNode();
    if (star != Done)
        scheduleClock();
    delete(msg);
}

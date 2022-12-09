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
Define_Module(Node);

int Node::numInitStages() const { return 2; }

void Node::sendAll(){
    int n = par("numberOfNodes");
    for(int i = 0; i<n; ++i ){
       char str[20];
       sprintf(str, "Infetto %d", self_id);

        cMessage * mess = new cMessage(str);
        if(isReachable[i])
            send(mess,"out",i);
    }
}

void Node::scheduleClock(){
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
}

void Node::initialize(int stage)
{
    self_id = getIndex();
    if (stage == 0){
        isReachable = new bool[par("numberOfNodes")]();
        char str[8];
        double pos_x = par("pos_x");
        double pos_y = par("pos_y");
        double other_x, other_y;
        for (int i = 0; i < (int)par("numberOfNodes"); ++i){
            if (i == self_id){
                isReachable[i] = false;
                continue;
            }
            sprintf(str, "nodeX[%d]", i);
            cModule* other = getModuleByPath(str);
            other_x = other->par("pos_x");
            other_y = other->par("pos_y");
            double distance = (other_x - pos_x)*(other_x - pos_x) +
                    (other_y - pos_y)*(other_y - pos_y);
            double radius_squared = (double)par("radius") * (double)par("radius");
            isReachable[i] = radius_squared >= distance;
            EV << self_id << "(" << pos_x << "," << pos_y << ")"
                    << " " << i << "(" << other_x << "," << other_y << ")"
                    << " -> " << isReachable[i]
                    << " because distance: " << distance
                    << " radius_squared: " << radius_squared << endl;
        }

    } else if (stage == 1){
        if(par("firstInfected")){
            sendAll();
            isInfected=true;
        }
        scheduleClock();
    }
}

void Node::handleMessage(cMessage *msg)
{
    if(isInfected){
        delete(msg);
        return;
    }
    if(!strcmp("clock", msg->getName())){
        collisionCheck = false;
        if(receivedInfection){
            double probability=par("sendingProbability");
            double limit=par("limitProbability");
            EV<<"INFETTO "<<self_id<<endl;
            if(probability<=limit){
                EV<<"INVIATO "<<self_id<<endl;
                isInfected=true;
                sendAll();
            }
        }
        scheduleClock();

    }
    else {
        if(!collisionCheck){
            collisionCheck = true;
            receivedInfection = true;
        }
        else{
            EV<<"COLLISIONE "<<getName()<<endl;
        }

    }

    delete(msg);
}

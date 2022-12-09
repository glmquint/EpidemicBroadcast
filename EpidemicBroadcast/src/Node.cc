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

void Node::sendSelf(){
    cMessage * selfmessage = new cMessage("Probabilita");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,selfmessage);
}

void Node::sendAll(){
    int n = par("numberOfGates");
    int nodeNumber = getIndex();
    for(int i = 0;i<n; ++i ){
       char str[20];
       sprintf(str, "Infetto %s", getName());

        cMessage * mess = new cMessage(str);
        cGate *g = gate("out",i);
        if(g->getIndex()!=nodeNumber)
            send(mess,"out",i);
    }
}

void Node::initialize()
{
    // compute probability

    if(par("firstInfected")){

        double probability=par("sendingProbability");
        double limit=par("limitProbability");

        sendAll();

        isInfected=true;
    }
}

void Node::handleMessage(cMessage *msg)
{
    if(isInfected){
        delete(msg);
        return;
    }
    if(!(strcmp("clock", msg->getName()))){
        collisionCheck = false;
        if(receivedInfection){
            double probability=par("sendingProbability");
            double limit=par("limitProbability");
            EV<<"INFETTO"<<getName()<<endl;
            if(probability<=limit){
                EV<<"INVIATO"<<getName()<<endl;
                isInfected=true;
                sendAll();
            }
        }
    }

    else{
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

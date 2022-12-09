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
        if(g->getIndex()!=nodeNumber) // and gate in lista di adiacenza
            send(mess,"out",i);
    }
}

void Node::initialize(int stage)
{
    if (stage == 0){
        char str[15];
        double pos_x = par("pos_x");
        double pos_y = par("pos_y");
        sprintf(str, "%f %f", pos_x, pos_y);
        cMessage* msg = new cMessage(str);
        //sendDirect(msg, getParentModule()->getSubmodule("oracle", 0), "in", getIndex());
        sendDirect(msg, getModuleByPath("oracle"), "in", getIndex());
    } else if (stage == 1){
        /* abbiamo già la lista di adiacenza
         * se il nodo è il primo infetto
         * far partire l'infezione
         */
        if(par("firstInfected")){
            //double probability=par("sendingProbability");
            //double limit=par("limitProbability");
            sendAll();
            isInfected=true;
        }
    }
    //EV << getIndex() << ": " << par("pos_x") << " " << par("pos_y") << endl;

}

void Node::handleMessage(cMessage *msg)
{
    /* se il messaggio proviene dall'oracle,
     * allora salva la lista di adiacenza
     */
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

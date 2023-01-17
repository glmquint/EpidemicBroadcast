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

#include <assert.h>
#include "StatCollector.h"

Define_Module(StatCollector);

void StatCollector::initialize()
{
    int sumNeighbors = 0;
    char str[8];
    numberOfNodes = par("numberOfNodes");
    nodes = new Node*[numberOfNodes]();
    for (int i = 0; i < numberOfNodes; ++i){
        sprintf(str, "nodeX[%d]", i);
        nodes[i] = check_and_cast<Node*>(getModuleByPath(str));
    }
    for (int i = 0; i < 45; ++i){
        for (int j = 0; j < 5; ++j)
            stats[j][i] = 0;
    }

    collectorSignal[0] = registerSignal("waiting");
    collectorSignal[1] = registerSignal("oneMSG");
    collectorSignal[2] = registerSignal("collision");
    collectorSignal[3] = registerSignal("ready");
    collectorSignal[4] = registerSignal("done");

    /*waitingSignal = registerSignal("waiting");
    oneMSGSignal = registerSignal("oneMSG");
    collisionCollectorSignal = registerSignal("collision");
    readySignal = registerSignal("ready");
    doneSignal = registerSignal("done");
    */
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+((double)par("timer")/2);
    scheduleAt(sim,self);

}

void StatCollector::handleMessage(cMessage *msg)
{
    int64_t t = (int64_t)simTime().raw()/1000000000000;
    for (int i = 0; i < numberOfNodes; ++i){
        Status s = nodes[i]->getStatus();
        stats[s][t]++;
        //EV << t << " " << i << " (" << s << ")" << endl;
    }
    for(int i = 0; i<5; ++i){
        emit(collectorSignal[i], stats[i][t]);
    }
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
    delete(msg);
}

//void StatCollector::registerStatus(char status[9])
//{
//    int64_t t = (int64_t)simTime().raw()/1000000000000;
//    //EV << t << " status: " << status << endl;
//    //status[t.raw()][status]++;
//    if (strcmp(status, "=WAITING="))
//        statsW[t] += 1;
//    else if (strcmp(status, "==READY=="))
//        statsR[t] += 1;
//    else if (strcmp(status, "COLLISION"))
//        statsC[t] += 1;
//}

void StatCollector::finish()
{
    EV << "t W " << "1 " << "C " << "R " << "D sum" << endl;
    int sum, nodesInStatus;
    for (int i = 0; i < 45; ++i){
        //EV << i << " W: " << statsW[i] << " R: " << statsR[i] << " C: " << statsC[i] << endl;
        EV << i;
        sum = 0;
        for (int j = 0; j < 5; ++j){
            nodesInStatus = stats[j][i];
            sum += nodesInStatus;
            EV << " " << nodesInStatus;
        }
        EV << "  " << sum << endl;
        assert(sum == numberOfNodes);
    }
}

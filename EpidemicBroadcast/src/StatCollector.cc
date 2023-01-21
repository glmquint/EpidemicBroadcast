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
    assert(TIME_LIMIT == par("time_limit"));
    int sumNeighbors = 0;
    char str[8];
    numberOfNodes = par("numberOfNodes");
    nodes = new Node*[numberOfNodes]();
    for (int i = 0; i < numberOfNodes; ++i){
        sprintf(str, "nodeX[%d]", i);
        nodes[i] = check_and_cast<Node*>(getModuleByPath(str));
    }
    for (int i = 0; i < TIME_LIMIT; ++i){
        for (int j = 0; j < STATUS_NUMBER; ++j)
            stats[j][i] = 0;
    }
    collectorSignal[Waiting] = registerSignal("waiting");
    collectorSignal[OneMessage] = registerSignal("oneMSG");
    collectorSignal[Collision] = registerSignal("collision");
    collectorSignal[Ready] = registerSignal("ready");
    collectorSignal[Done] = registerSignal("done");
    rateSignal = registerSignal("rate");

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
    for(int i = 0; i<STATUS_NUMBER; ++i){
        double rate = (double)stats[i][t]/numberOfNodes;
        emit(collectorSignal[i], rate);
    }

    if(emitCheck && stats[OneMessage][t]==0 && stats[Ready][t]==0){
        double doneRate = (double)stats[Done][t]/numberOfNodes;
        emit(rateSignal, doneRate);
        emitCheck = false;
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
/*
void StatCollector::finish()
{
   // EV << "t W " << "1 " << "C " << "R " << "D sum" << endl;
    int sum, nodesInStatus;
    for (int i = 0; i < TIME_LIMIT; ++i){
        //EV << i << " W: " << statsW[i] << " R: " << statsR[i] << " C: " << statsC[i] << endl;
        //EV << i;
        sum = 0;
        for (int j = 0; j < STATUS_NUMBER; ++j){
            nodesInStatus = stats[j][i];
            sum += nodesInStatus;
            //EV << " " << nodesInStatus;
        }
        //EV << "  " << sum << endl;
        assert(sum == numberOfNodes);
    }
}*/

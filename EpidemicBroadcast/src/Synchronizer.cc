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


#include "Synchronizer.h"

Define_Module(Synchronizer);

void Synchronizer::initialize()
{
    //cMessage *clock = new cMessage("clock");
    cMessage *self = new cMessage("self");
    /*int n = par("numberOfNodes");
    for(int i = 0; i<n; ++i){
        send(clock, "out", i);
    }*/
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
}

void Synchronizer::handleMessage(cMessage *msg)
{
    int n = par("numberOfNodes");
    for(int i = 0; i<n; ++i){
    cMessage *clock = new cMessage("clock");
        send(clock, "out", i);
    }
    cMessage *self = new cMessage("self");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
    delete(msg);
}

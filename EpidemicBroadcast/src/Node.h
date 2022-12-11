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


#ifndef __EPIDEMICBROADCAST_NODE_H_
#define __EPIDEMICBROADCAST_NODE_H_

#include <omnetpp.h>
#include <stdio.h>
#include <iostream>
#include <numeric>
using namespace omnetpp;


class Node : public cSimpleModule
{
    private:
    bool hasInfected = false;
    bool receivedInfection = false;
    bool collisionCheck = false;
    bool* isReachable;
    bool hasValidMsg = false;
    int self_id;
    int infectionHop = 0;
    /*====statistics=====*/
    bool collisionOccurred = false;
    simsignal_t collisionSignal;
    simsignal_t neighborsSignal;
    simsignal_t endTimeSignal;
    simsignal_t hopSignal;


  protected:
    virtual void initialize() override;
    //virtual int numInitStages() const override;
    virtual void handleMessage(cMessage *msg) override;
    void colorNode(char* color);
    void scheduleClock();
    void sendAll();
};

#endif

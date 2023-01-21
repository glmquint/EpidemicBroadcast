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

#ifndef __EPIDEMICBROADCAST_STATCOLLECTOR_H_
#define __EPIDEMICBROADCAST_STATCOLLECTOR_H_

#include <omnetpp.h>
#include "status.h"
#include "Node.h"

#define TIME_LIMIT 60

using namespace omnetpp;

class StatCollector : public cSimpleModule
{
  //public:
    //void registerStatus(char status[9]);
  private:
    int stats[STATUS_NUMBER][TIME_LIMIT];
    int numberOfNodes;
    Node** nodes;
    bool emitCheck = true;
    //int time_limit = par("time_limit");

    simsignal_t collectorSignal [STATUS_NUMBER];
    simsignal_t rateSignal;
    /*
    simsignal_t waitingSignal;
    simsignal_t oneMSGSignal;
    simsignal_t collisionCollectorSignal;
    simsignal_t readySignal;
    simsignal_t doneSignal;
    */

  protected:
    virtual void initialize() override;
    //virtual void finish() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif

/*
 * StatCollector class definition
 *
 * the macro TIME_LIMIT must be kept equal to the value set in omnetpp.ini configuration file
 * This ensures the statically allocated memory for the 2D array stats is always adequate
 * for the specific simulation campaign's time limit
 */

#ifndef __EPIDEMICBROADCAST_STATCOLLECTOR_H_
#define __EPIDEMICBROADCAST_STATCOLLECTOR_H_

#include <omnetpp.h>
#include "status.h"
#include "Node.h"

#define TIME_LIMIT 200

using namespace omnetpp;

class StatCollector : public cSimpleModule
{
  private:
    int stats[STATUS_NUMBER][TIME_LIMIT];
    int numberOfNodes;
    Node** nodes;
    bool emitCheck = true;

    simsignal_t collectorSignal [STATUS_NUMBER];
    simsignal_t rateSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
};

#endif

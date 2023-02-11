/*
 * Node class definition
 *
 * Node's behavioiur follow a FSM mechanism with 5 states, defined in status.h
 * A complete description is provided in the documentation
 */

#ifndef __EPIDEMICBROADCAST_NODE_H_
#define __EPIDEMICBROADCAST_NODE_H_

#include <omnetpp.h>
#include <stdio.h>
#include <iostream>
#include <numeric>
#include "status.h"
using namespace omnetpp;

class Node : public cSimpleModule
{
    private:
        Status star;
        bool* isReachable;
        int numberOfNodes;
        int self_id;
        int infectionHop = 0;
        double sendingProbability;
        double limitProbability;

    public:
        Status getStatus(); // public for statCollector's status retrieval
    protected:
        virtual void initialize() override;
        virtual void handleMessage(cMessage *msg) override;
        void setupAdjacencyList();
        void colorNode();
        void scheduleClock();
        void sendAll(double time);
        Status lottery();
};

#endif

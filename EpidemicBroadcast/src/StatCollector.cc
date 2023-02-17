/*
 * StatCollector class implementation
 *
 * This utility node has access to all instanced nodes on the floorplan
 * This allows the collection of the number of nodes in a specific
 * status at each time slot
 */

#include <assert.h>
#include "StatCollector.h"

Define_Module(StatCollector);

// omnetpp's initialization stage
void StatCollector::initialize()
{
    assert(TIME_LIMIT == par("time_limit")); // assertion for omnetpp and c++ class configuration alignment
    int sumNeighbors = 0;
    char str[8];
    numberOfNodes = par("numberOfNodes");
    nodes = new Node*[numberOfNodes]();
    for (int i = 0; i < numberOfNodes; ++i){
        sprintf(str, "nodeX[%d]", i);
        nodes[i] = check_and_cast<Node*>(getModuleByPath(str)); // node referencing for stat retrieval
    }
    for (int i = 0; i < TIME_LIMIT; ++i){
        for (int j = 0; j < STATUS_NUMBER; ++j)
            stats[j][i] = 0; // initialized mem setting for each statistcs, for each time slot
    }
    collectorSignal[Waiting] = registerSignal("waiting");
    collectorSignal[OneMessage] = registerSignal("oneMSG");
    collectorSignal[Collision] = registerSignal("collision");
    collectorSignal[Ready] = registerSignal("ready");
    collectorSignal[Done] = registerSignal("done");
    rateSignal = registerSignal("rate");
    cMessage *self = new cMessage("clock");
    // initial time slot is offset by half a time slot
    // this guarantees that stat retrieval is performed after every node status transitioned
    simtime_t sim = simTime()+((double)par("timer")/2);
    scheduleAt(sim,self);

}

// omnetpp's functionality at message reception
// only self messages are expected, which trigger the
// saving of how many nodes are in each status in this time slot
void StatCollector::handleMessage(cMessage *msg)
{
    int64_t t = (int64_t)simTime().raw()/1000000000000; // second
    for (int i = 0; i < numberOfNodes; ++i){
        Status s = nodes[i]->getStatus();
        stats[s][t]++; // cumulative sum for status s at time t
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
    // every other self message has the same frequency as node's
    // but the initial offset will keep them always
    // subsequent to node's status transition
    scheduleAt(sim,self);
    delete(msg);

}

// omnetpp's completion function at the end of simulation
// This performs a sanity check for validity of data retrieved
// It has been commented in the final simulation for performance reasons
void StatCollector::finish()
{
    int sum, nodesInStatus;
    for (int i = 0; i < TIME_LIMIT; ++i){
        sum = 0;
        for (int j = 0; j < STATUS_NUMBER; ++j){
            nodesInStatus = stats[j][i];
            sum += nodesInStatus;
        }
        assert(sum == numberOfNodes);
    }
}

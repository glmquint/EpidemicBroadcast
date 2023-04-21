/*
 * Node class implementation
 *
 * Node's behavioiur follow a FSM mechanism with 5 states, defined in status.h
 * A complete description is provided in the documentation
 */

#include "Node.h"
#include <stdio.h>
#include <iostream>
#include <numeric>
Define_Module(Node);

# define INFECTION_FRMT "Infetto %d"

// omnetpp's initialization stage
void Node::initialize()
{
    // parameter initialization from omnetpp's ini file
    self_id = getIndex();
    limitProbability=par("limitProbability");
    int sumNeighbors = 0;
    numberOfNodes = par("numberOfNodes");

    // discovery of node neighbors that are
    // reachable within the radius R
    setupAdjacencyList();
    for (int i = 0; i < numberOfNodes; ++i){
        sumNeighbors += isReachable[i];
    }
    // patient zero is set by the configuration file
    if(par("firstInfected")){
        sendAll(0.0);
        star = Done;
        colorNode();
    } else { // every other node starts in waiting status
        scheduleClock();
        star = Waiting;
    }
}

// O(n^2) implementation of the adjacency matrix
// Each node computes its bitmask of reachable neighbors
// within a radius R
// For an improved performance, a spatial partitioning algorithm
// could be implemented as a preprocessing stage
void Node::setupAdjacencyList()
{
    isReachable = new bool[numberOfNodes](); // the portion of the adjacency matrix for this node
    char str[8];
    double pos_x = par("pos_x");
    double pos_y = par("pos_y");
    double other_x, other_y;
    double radius_squared = (double)par("radius") * (double)par("radius");
    for (int i = 0; i < numberOfNodes; ++i){ // O(n) called once for each node at initialization
        sprintf(str, "nodeX[%d]", i);
        cModule* other = getModuleByPath(str);
        other_x = other->par("pos_x");
        other_y = other->par("pos_y");
        double distance = (other_x - pos_x)*(other_x - pos_x) +
                (other_y - pos_y)*(other_y - pos_y);
        isReachable[i] = (i != self_id && radius_squared >= distance); // proximity check and auto-linking exclusion
        cDisplayString& connDispStr = gate("out", i)->getDisplayString();
        if (isReachable[i]) // link color coding that hides unused connections
            connDispStr.parse("ls=black");
        else
            connDispStr.parse("ls=red,0;");
    }
}

// omnetpp's implementation of a fixed time slot execution for every node in the graph
void Node::scheduleClock()
{
    cMessage *self = new cMessage("clock");
    simtime_t sim = simTime()+par("timer");
    scheduleAt(sim,self);
}

// infection message transmission to all nodes within radius R
void Node::sendAll(double time)
{
    for(int i = 0; i < numberOfNodes; ++i ){
        if(!isReachable[i]) // adjacency matrix is used to skip unreachable nodes
            continue;
        char str[20];
        sprintf(str, INFECTION_FRMT, infectionHop);
        cMessage * mess = new cMessage(str);
        send(mess,"out",i);
        EV<<"INVIATO "<<str<<endl;
    }
}

// global mapping for node status color coding
char* STAR2COLOR[STATUS_NUMBER] = {(char*)"lightgrey", (char*)"green", (char*)"orange", (char*)"blue", (char*)"red"};

// node color coding updating
void Node::colorNode()
{
    char str[50];
    char* color;
    cDisplayString& nodeDispStr = getDisplayString();
    color = STAR2COLOR[star];
    sprintf(str, "p=$pos_x,$pos_y;i=block/process,%s,50", color);
    nodeDispStr.parse(str);
}

// node status retrieval for stat collection
Status Node::getStatus()
{
    return star;
}

// random experiment as described in requirements
// A RV is extracted from a uniform distribution
// and is compared with a fixed treshold
// Returns the new state the node should transition to,
// either Done (success) or Ready (failed)
Status Node::lottery()
{
    sendingProbability=par("sendingProbability");
    if(sendingProbability<=limitProbability){
        sendAll(simTime().dbl()); // performs the infection message relay
        return Done;
    }
    return Ready;
}

// omnetpp's functionality at message reception
// This implements the FSM behaviour as described in the documentation
void Node::handleMessage(cMessage* msg)
{
    if(!strcmp("clock", msg->getName())){// message is a self clock. This represents the beginning of a new time slot
        switch (star) {
            case OneMessage:
            case Ready: // node has a message and performs the random experiment
                star = lottery(); break;
            case Collision: // node cycles back to listening for incoming messages
                star = Waiting; break;
            default:
                break;
        }
    } else { // message is an infection from another node
        switch (star) {
            case Waiting: // this is the first message in this time slot
                sscanf(msg->getName(), INFECTION_FRMT, &infectionHop);
                infectionHop++;
                EV << "received" << msg->getName() << " -> " << infectionHop << endl;
                star = OneMessage; break;
            case OneMessage: // if node already had a message in this time slot, it collides
                star = Collision; break;
            default:
                break;
        }
    }
    colorNode(); // update the color coding for debugging after a potential status change
    if (star != Done && !strcmp("clock", msg->getName())) // only done nodes are deactivated
        scheduleClock();
    delete(msg);
}

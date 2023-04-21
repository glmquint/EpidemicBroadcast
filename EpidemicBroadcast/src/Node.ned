//
// Node configuration definition
//
// Node's behavioiur follow a FSM mechanism with 5 states, defined in status.h
// A complete description is provided in the documentation
//
 
package epidemicbroadcast;

simple Node
{
    parameters:
        int numberOfNodes = default(4);
        volatile double sendingProbability = uniform(0,1);
        double limitProbability = default(0.5);
        bool firstInfected = default(false);
        double timer = default(1.0);
        double pos_x = default(uniform(0, 500));
        double pos_y = default(uniform(0, 500));
        double radius = default(100.0);
        @display("p=$pos_x,$pos_y");

    gates:
        input in[numberOfNodes];
        output out[numberOfNodes];
}

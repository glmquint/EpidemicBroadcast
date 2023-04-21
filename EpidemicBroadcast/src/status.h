/*
 * definition of the utility enum for available node statuses, implementing the FSM
 */

#ifndef STATUS_INCLUDED
#define STATUS_INCLUDED
enum Status {
    Waiting = 0, OneMessage, Collision, Ready, Done, STATUS_NUMBER
};
#endif

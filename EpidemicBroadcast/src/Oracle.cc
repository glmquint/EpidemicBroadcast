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

#include "Oracle.h"

Define_Module(Oracle);

int Oracle::numInitStages() const { return 2; }

void Oracle::initialize(int stage)
{
    if (stage == 0){
        EV<<"primo stage"<<endl;
        counter = 0;
        EV<<counter<<endl;
        while(counter<4){}
        EV<<counter<<endl;
    }
    if (stage == 1){
        EV<<counter<<endl;
        while(counter<4){}
        EV<<counter<<endl;
        EV << "secondo stage" << endl;
        /* abbiamo già tutte le posizioni
         * calcola liste di adiacenze
         * invia a ciascun nodo la propria lista
         */
    }
}

void Oracle::handleMessage(cMessage *msg)
{
    EV << msg->getName() << endl;
    counter++;
    //EV<<counter<<endl;
     //salva l'i-esima posizione
}

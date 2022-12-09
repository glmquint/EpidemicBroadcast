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

package epidemicbroadcast.simulations;

import epidemicbroadcast.Node;
import epidemicbroadcast.Synchronizer;
import epidemicbroadcast.Oracle;


network Floorplan
{
    parameters:
        int numberOfNodes = default(4);

        @display("bgb=558,430");
    types:
        channel Unnamed
        {
        }

    submodules:
        nodeX[numberOfNodes]: Node {
            @display("p=278,167,m,4,50,50;is=s");
        }
        synchronizer: Synchronizer {
            @display("p=189,321");
        }
        oracle: Oracle{}
    connections allowunconnected:
        for i=0..sizeof(nodeX)-1, for j=0..sizeof(nodeX)-1 {
            nodeX[i].out[j] --> {  delay = 25ms; } --> nodeX[j].in[i] if i!=j ;//&& 
            	//(nodeX[i].radius * nodeX[i].radius) > 
            		//(nodeX[j].x_pos - nodeX[i].x_pos)*(nodeX[j].x_pos - nodeX[i].x_pos) + 
            		//(nodeX[j].y_pos - nodeX[i].y_pos)*(nodeX[j].y_pos - nodeX[i].y_pos);*/
        }


        for i=0..sizeof(nodeX)-1 {
            synchronizer.out[i] --> {  delay = 150ms; } --> nodeX[i].synch;
        }
       
}

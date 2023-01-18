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
import epidemicbroadcast.StatCollector;


network Floorplan
{
    parameters:
        int numberOfNodes = default(4);

        @display("bgb=642,470");

    submodules:
        nodeX[numberOfNodes]: Node {};
        statCollector: StatCollector{}
    connections:// allowunconnected:
        for i=0..sizeof(nodeX)-1, for j=0..sizeof(nodeX)-1 {
            nodeX[i].out[j] --> {  delay = 25ms; } --> nodeX[j].in[i];
        }
}

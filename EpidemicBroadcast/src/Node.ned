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

//
// TODO auto-generated module
//

package epidemicbroadcast;

simple Node
{
    parameters:
        int numberOfNodes = default(4);
        volatile double sendingProbability = uniform(0,1);
        double limitProbability = 0.4;
        bool firstInfected = default(false);
        double timer = default(0.5);
        double pos_x = default(uniform(0, 500));
        double pos_y = default(uniform(0, 500));
        double radius = default(200.0);
        //@display("p=$pos_x,$pos_y;r=$radius;");
        @display("p=$pos_x,$pos_y");
        //=====statistics=======//
        @signal[collision](type=int);
		@statistic[collisionStat](source="collision";record=sum;);


    gates:
        input in[numberOfNodes];
        output out[numberOfNodes];
}

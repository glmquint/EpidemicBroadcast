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

package epidemicbroadcast;

//
// TODO auto-generated module
//
simple StatCollector
{
    parameters:
        int numberOfNodes = default(4);
        double timer = default(1.0);
        
        //=====statistics=======//
        @signal[waiting](type=double);
		@statistic[waitingStat](source="waiting";record=vector;);
		@signal[oneMSG](type=double);
		@statistic[oneMSGStat](source="oneMSG";record=vector;);
		@signal[collision](type=double);
		@statistic[collision](source="collision";record=vector;);
		@signal[ready](type=double);
		@statistic[readyStat](source="ready";record=vector;);
		@signal[done](type=double);
		@statistic[doneStat](source="done";record=vector;);
		
		@signal[rate](type=double);
		@statistic[rateStatistic](source="rate"; record=vector,max;);
		
}

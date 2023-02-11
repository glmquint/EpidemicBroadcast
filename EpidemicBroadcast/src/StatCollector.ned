//
// StatCollector configuration definition
//

package epidemicbroadcast;

simple StatCollector
{
    parameters:
        int numberOfNodes = default(4);
        int time_limit=default(45);
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

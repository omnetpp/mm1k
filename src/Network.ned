simple Source
{
    parameters:
        volatile double sendInterval @unit(s);
        @display("i=block/source");
    gates:
        output out;
}

simple Queue
{
    parameters:
        volatile double serviceTime @unit(s);
        int capacity;
        @display("i=block/queue");
        @signal[queueLength](type=long);
        @signal[dropped](type=long);
        @signal[busy](type=bool);
        @signal[queueingTime](type=simtime_t);
        @statistic[queueLength](source=queueLength; title="queue length"; record=timeavg,vector);
        @statistic[dropped](source=dropped; title="dropped"; record=count);
        @statistic[busy](source=busy; title="server busy"; record=timeavg);
        @statistic[queueingTime](source=queueingTime; title="queueing time"; record=mean,vector);
    gates:
        input in;
        output out;
}

simple Sink
{
    parameters:
        @display("i=block/sink");
        @signal[totalServiceTime](type=simtime_t);
        @signal[totalQueueingTime](type=simtime_t);
        @signal[generation](type=long);
        @signal[lifeTime](type=simtime_t);
        @statistic[totalServiceTime](source=totalServiceTime; title="total service time"; record=mean);
        @statistic[totalQueueingTime](source=totalQueueingTime; title="total queueing time"; record=mean);
        @statistic[generation](source=generation; title="generation"; record=count);
        @statistic[lifeTime](source=lifeTime; title="lifetime"; record=mean,vector);
    gates:
        input in;
}

network Network
{
    parameters:
        double lambda;
        double mu;
        int K;
    submodules:
        source: Source {
            sendInterval = exponential(1s / parent.lambda);
        }
        queue: Queue {
            serviceTime = exponential(1s / parent.mu);
            capacity = parent.K;
        }
        sink: Sink;
    connections:
        source.out --> queue.in;
        queue.out --> sink.in;
}

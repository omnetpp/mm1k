#include "Sink.h"

Define_Module(Sink);

void Sink::initialize()
{
    numReceived = 0;
    totalServiceTimeSignal = registerSignal("totalServiceTime");
    totalQueueingTimeSignal = registerSignal("totalQueueingTime");
    generationSignal = registerSignal("generation");
    lifeTimeSignal = registerSignal("lifeTime");
}

void Sink::handleMessage(cMessage *msg)
{
    numReceived++;
    simtime_t lifeTime = simTime() - msg->getTimestamp();
    emit(lifeTimeSignal, lifeTime);
    emit(generationSignal, 1L);
    delete msg;
}

void Sink::finish()
{
    recordScalar("received", numReceived);
}

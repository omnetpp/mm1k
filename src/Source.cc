#include "Source.h"

Define_Module(Source);

void Source::initialize()
{
    numGenerated = 0;
    sendMessageEvent = new cMessage("sendMessageEvent");
    scheduleAt(simTime(), sendMessageEvent);
}

void Source::handleMessage(cMessage *msg)
{
    ASSERT(msg == sendMessageEvent);

    cMessage *job = new cMessage("job");
    job->setTimestamp();
    numGenerated++;
    send(job, "out");

    scheduleAt(simTime() + par("sendInterval"), sendMessageEvent);
}

void Source::finish()
{
    recordScalar("generated", numGenerated);
}

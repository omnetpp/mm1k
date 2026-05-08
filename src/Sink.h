#ifndef SINK_H
#define SINK_H

#include <omnetpp.h>

using namespace omnetpp;

class Sink : public cSimpleModule
{
  private:
    long numReceived;
    simsignal_t totalServiceTimeSignal;
    simsignal_t totalQueueingTimeSignal;
    simsignal_t generationSignal;
    simsignal_t lifeTimeSignal;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;
};

#endif

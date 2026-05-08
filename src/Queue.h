#ifndef QUEUE_H
#define QUEUE_H

#include <omnetpp.h>

using namespace omnetpp;

class Queue : public cSimpleModule
{
  private:
    int capacity;
    cQueue queue;
    cMessage *endServiceMsg;
    cMessage *jobBeingServed;

    long numDropped;
    long numAccepted;

    simsignal_t queueLengthSignal;
    simsignal_t droppedSignal;
    simsignal_t busySignal;
    simsignal_t queueingTimeSignal;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage *msg) override;
    virtual void finish() override;

    void startService(cMessage *job);
    int getSystemCount();
};

#endif

#include "Queue.h"

Define_Module(Queue);

void Queue::initialize()
{
    capacity = par("capacity");
    jobBeingServed = nullptr;
    numDropped = 0;
    numAccepted = 0;
    endServiceMsg = new cMessage("endService");

    queueLengthSignal = registerSignal("queueLength");
    droppedSignal = registerSignal("dropped");
    busySignal = registerSignal("busy");
    queueingTimeSignal = registerSignal("queueingTime");

    emit(queueLengthSignal, 0L);
    emit(busySignal, false);
}

int Queue::getSystemCount()
{
    return queue.getLength() + (jobBeingServed != nullptr ? 1 : 0);
}

void Queue::handleMessage(cMessage *msg)
{
    if (msg == endServiceMsg) {
        // Service complete: send the served job to sink
        cMessage *finished = jobBeingServed;
        jobBeingServed = nullptr;
        send(finished, "out");

        // Start next job if available
        if (!queue.isEmpty()) {
            cMessage *next = check_and_cast<cMessage *>(queue.pop());
            startService(next);
        }
        else {
            emit(busySignal, false);
            emit(queueLengthSignal, (long)getSystemCount());
        }
    }
    else {
        // Arriving job
        if (getSystemCount() >= capacity) {
            numDropped++;
            emit(droppedSignal, 1L);
            delete msg;
        }
        else {
            numAccepted++;
            if (jobBeingServed == nullptr) {
                // Server idle
                startService(msg);
            }
            else {
                // Server busy, enqueue
                queue.insert(msg);
                emit(queueLengthSignal, (long)getSystemCount());
            }
        }
    }
}

void Queue::startService(cMessage *job)
{
    jobBeingServed = job;
    emit(busySignal, true);
    emit(queueLengthSignal, (long)getSystemCount());

    simtime_t queueingTime = simTime() - job->getTimestamp();
    emit(queueingTimeSignal, queueingTime);

    simtime_t serviceTime = par("serviceTime");
    scheduleAt(simTime() + serviceTime, endServiceMsg);
}

void Queue::finish()
{
    recordScalar("accepted", numAccepted);
    recordScalar("dropped", numDropped);
    double total = numAccepted + numDropped;
    if (total > 0)
        recordScalar("dropProbability", (double)numDropped / total);
}

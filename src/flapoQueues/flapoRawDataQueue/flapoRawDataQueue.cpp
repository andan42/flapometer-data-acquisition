#include "flapoRawDataQueue.h"
#include "flapoData/flapoRawData/flapoRawDataPollCancelled/flapoRawDataPollCancelled.h"

const std::queue<FlapoRawData *> &FlapoRawDataQueue::getQueue() const
{
    return queue;
}

void FlapoRawDataQueue::push(FlapoRawData *data)
{
    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //add data to queue
    queue.push(data);

    //notify client after adding data
    cond.notify_one();

    //release lock is automatically done by unique_lock
}

FlapoRawData *FlapoRawDataQueue::pop()
{
    bool active = true;
    return popCancelleable(&active);
}

FlapoRawData *FlapoRawDataQueue::popCancelleable(bool *active)
{
    //if active is nullptr
    if(active == nullptr)
    {
        //return a cancelled data type
        return new FlapoRawDataPollCancelled(nullptr, "FlapoRawDataQueue popCancelleable() called with nullptr. Call .pop() instead if necessary.");
    }

    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //wait for data if queue is empty
    //cond.wait(lock, [&] { return !queue.empty(); });
    //cond.wait(lock, [&] { return !queue.empty() || kill; });
    cond.wait(lock, [&] 
        {return !queue.empty() || kill || !*active;}
    );

    if(kill || !*active)
    {
        return new FlapoRawDataPollCancelled(nullptr, "FlapoRawDataQueue killed or popCancelleable() inactivated.");
    }

    //retrieve data from queue
    FlapoRawData *data = queue.front();

    //pop data from queue
    queue.pop();

    //release lock is automatically done by unique_lock

    //return data
    return data;
}

void FlapoRawDataQueue::cancellAll()
{
    //set kill flag
    this->kill = true;
    //notify all waiting threads
    cond.notify_all();
}

void FlapoRawDataQueue::notifyAll()
{
    //notify all waiting threads
    cond.notify_all();
}

FlapoRawDataQueue::~FlapoRawDataQueue()
{
    //cancel all pending operations
    cancellAll();

    //delete all data in queue
    while (!queue.empty())
    {
        FlapoRawData *data = queue.front();
        queue.pop();
        delete data;
    }
}

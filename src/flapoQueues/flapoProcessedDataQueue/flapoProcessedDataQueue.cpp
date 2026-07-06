#include "flapoProcessedDataQueue.h"

void FlapoProcessedDataQueue::push(std::shared_ptr<const FlapoProcessedData> data)
{
    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //add data to queue
    dequeue.push_back(data);

    //notify client after adding data
    cond.notify_one();

    //release lock is automatically done by unique_lock
}

std::deque<std::shared_ptr<const FlapoProcessedData>> FlapoProcessedDataQueue::collapseMerge()
{
    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //pre check for nulptrs
    //todo

    //loop over queue
    for (auto it1 = dequeue.begin(); it1 != dequeue.end(); ++it1)
    {
        if (!*it1) continue; //skip null elements

        //loop over elements after current element
        for (auto it2 = std::next(it1); it2 != dequeue.end();)
        {
            //skip null elements without erasing
            if (!*it2)
            {
                ++it2;
                continue;
            }

            //if data can be merged, merge it
            if ((*it1)->canMerge(*(*it2).get()))
            {
                std::shared_ptr<const FlapoProcessedData> merged = (*it1)->merge(*(*it2).get());

                *it1 = merged;
                *it2 = nullptr;
            }
            ++it2;
        }
    } 

    //strip null elements
    for (auto it = dequeue.begin(); it != dequeue.end();)
    {
        if (!*it)
        {
            it = dequeue.erase(it);
        }
        else
        {
            ++it;
        }
    }

    //return a copy of the deque, because as soon as the lock is released, the actual deque could be modified
    std::deque<std::shared_ptr<const FlapoProcessedData>> copy = dequeue;

    //pop all elements from the queue
    dequeue.clear();

    //return
    return copy;

    //release lock is automatically done by unique_lock
}

void FlapoProcessedDataQueue::mergeOrPush(std::shared_ptr<const FlapoProcessedData> data)
{
    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //loop over queue
    bool found = false;
    for (auto &d : dequeue)
    {
        //if data is already in queue, merge it
        if (d->canMerge(*data.get()))
        {
            std::shared_ptr<const FlapoProcessedData> merged = d->merge(*data.get());
            d = merged;
            found = true;
            break;
        }
    }

    //if data is not in queue, push it
    if (!found)
    {
        dequeue.push_back(data);
    }

    //notify client after adding data
    cond.notify_one();
}

std::shared_ptr<const FlapoProcessedData> FlapoProcessedDataQueue::pop()
{
    //acquire lock
    std::unique_lock<std::mutex> lock(mutex);

    //wait for data if queue is empty
    cond.wait(lock, [&] { return !dequeue.empty(); });

    //retrieve data from queue
    std::shared_ptr<const FlapoProcessedData> data = dequeue.front();

    //pop data from queue
    dequeue.pop_front();

    //release lock is automatically done by unique_lock

    //return data
    return data;
}

int FlapoProcessedDataQueue::size()
{
    return dequeue.size();
}

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>

#include "flapoRawData/flapoRawData.h"

class FlapoRawDataQueue
{
private:
    std::queue<FlapoRawData*> queue;
    std::mutex mutex;
    std::condition_variable cond;
    std::atomic<bool> kill;
public:
    //const queue getter
    const std::queue<FlapoRawData*>& getQueue() const;
    void push(FlapoRawData* data);
    FlapoRawData* pop();
    FlapoRawData* popCancelleable(bool* active);
    void cancellAll();
    void notifyAll();

    //constructor
    FlapoRawDataQueue() : kill(false) {}
    //destructor
    ~FlapoRawDataQueue();
};

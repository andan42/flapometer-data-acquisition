#pragma once

#include <condition_variable>
#include <deque>
#include <queue>

#include <memory>

#include "flapoProcessedData/flapoProcessedData.h"

class FlapoProcessedDataQueue
{
private:
    std::deque<std::shared_ptr<const FlapoProcessedData>> dequeue; //pointers are shared accross different queues to avoid making a gazillion copies
    std::mutex mutex;
    std::condition_variable cond;
public:
    //push data POINTER to queue
    void push(std::shared_ptr<const FlapoProcessedData> data);
    //this function 
    std::deque<std::shared_ptr<const FlapoProcessedData>> collapseMerge();
    void mergeOrPush(std::shared_ptr<const FlapoProcessedData> data);
    std::shared_ptr<const FlapoProcessedData> pop();

    //size
    int size();

    //TODO add kill method
};
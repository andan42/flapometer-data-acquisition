#include "flapoProcessingThread.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"
#include "flapoData/flapoRawData/flapoRawDataPollCancelled/flapoRawDataPollCancelled.h"
#include <iostream>

std::vector<std::shared_ptr<const FlapoProcessedData>> FlapoProcessingThread::processRawData()
{
    //get raw data from queue
    FlapoRawData *raw = m_rawDataQueue->popCancelleable(&(this->running));

    //check if the queue was killed by attempting to dynamically cast the raw data to FlapoRawDataPollCancelled
    //if not nullptr, raw is of type FlapoRawDataPollCancelled, which means the queue was killed
    //this is the normal intended way to stop the thread and queue
    {
        FlapoRawDataPollCancelled *cancel = dynamic_cast<FlapoRawDataPollCancelled*>(raw);
        if (cancel != nullptr)
        {
            if((running == false) && cancel->messageIs("FlapoRawDataQueue killed or popCancelleable() inactivated."))
            {
            }
            else
            {
                std::cout << "warning: FlapoRawDataQueue was killed by an unknown cause. FlapoProcessingThread::processRawData() returning nullptr." << std::endl;
            }

            delete raw; //THIS is the right place to delete the raw data. TODO consider using unique_ptr instead of raw pointer, this will automatically delete it, probably right here... but you never know
            return /*empty vector*/ std::vector<std::shared_ptr<const FlapoProcessedData>>();
        }
    }


    if (raw == nullptr)
    {
        std::cout << "warning: rawDataQueue->popCancellable() returned nullptr. FlapoProcessingThread::processRawData() returning nullptr." << std::endl;
        return /*empty vector*/ std::vector<std::shared_ptr<const FlapoProcessedData>>();
    }
    std::vector<std::shared_ptr<const FlapoProcessedData>> processedData = raw->getPolledSensor()->process(raw); //TODO make const correctness

    delete raw; //THIS is the right place to delete the raw data. the processed data is copied and the raw data is not needed anymore
    //IT IS NOT deleted n FlapoSensor::process() because it is not the responsibility of the sensor to delete the raw data. the data belongs to the queue and is then popped
    //TODO use unique_ptr instead of raw pointer, this will automatically delete it, probably right here... but you never know
    return processedData;
}

void FlapoProcessingThread::enqueueProcessedData(std::shared_ptr<const FlapoProcessedData> processedData)
{
    for (auto queue : m_processedDataQueues)
    {
        queue->push(processedData);
        //queue->mergeOrPush(processedData);
        //the issue with merge or push is it creates a copy of the data, which is not necessary
        //instead well do a normal push and collapse the data before uploading it out of the queue
    }
}

void FlapoProcessingThread::start()
{
    thread = new std::thread([this] {
        while (running)
        {
            std::vector<std::shared_ptr<const FlapoProcessedData>> processedDatas = processRawData();
            if (processedDatas.size() > 0)
            {
                //enqueueProcessedData(processedData);
                for (auto data : processedDatas)
                {
                    enqueueProcessedData(data);
                }
            }
            else
            {
                //std::cout << "warning: FlapoProcessingThread::processRawData() returned nullptr" << std::endl;
                //error messages are emitted by processRawData(), avoid redundancy or make it communciate errors back to this function more clearly
            }
        }
    });
}

void FlapoProcessingThread::stop()
{
    running = false;
    m_rawDataQueue->notifyAll();
    thread->join();
}

void FlapoProcessingThread::addProcessedDataQueue(FlapoProcessedDataQueue *processedDataQueue)
{
    m_processedDataQueues.push_back(processedDataQueue);
}

FlapoProcessingThread::FlapoProcessingThread(FlapoRawDataQueue *rawDataQueue) : m_rawDataQueue(rawDataQueue) 
{
    running = true;
    start();
}

FlapoProcessingThread::~FlapoProcessingThread()
{
    stop();
    delete thread;
}

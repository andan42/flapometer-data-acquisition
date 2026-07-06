#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <atomic>

#include <memory>

#include "flapoSensors/flapoSensor.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h" //might try to fwd declare
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h" //might try to fwd declare

class FlapoProcessingThread
{
private:
    std::thread *thread;

    std::vector<std::shared_ptr<const FlapoProcessedData>> processRawData();
    void enqueueProcessedData(std::shared_ptr<const FlapoProcessedData> processedData);

    FlapoRawDataQueue *m_rawDataQueue;
    std::vector<FlapoProcessedDataQueue*> m_processedDataQueues;

    //running bool, not rly thread safe
    bool running;

    void start();
    void stop();
public:
    std::vector<const FlapoSensor*> sensors; //TODO might not be needed

    void addProcessedDataQueue(FlapoProcessedDataQueue *processedDataQueue);

    FlapoProcessingThread(FlapoRawDataQueue *rawDataQueue);
    ~FlapoProcessingThread();
};


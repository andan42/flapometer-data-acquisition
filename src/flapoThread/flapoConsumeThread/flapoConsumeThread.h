#pragma once

#include <thread>
#include <atomic>
#include <condition_variable>

class FlapoProcessedDataQueue; //#include "flapoProcessedDataQueue.h", forward declared
class FlapoConsumer; //#include "flapoConsumer.h", forward declared

class FlapoConsumeThread
{
private:
    const bool consumeOnShutdown = true;
    bool consumeOnStartup = true;
    bool usePollingInterval; //if false, will consume as soon as data is available
    std::thread *thread;
    FlapoConsumer *m_consumer;
    void consumeAllProcessedData(); //calls consumer->consume(processedData)
    FlapoProcessedDataQueue *m_processedDataQueue;

    //running bool, thread safe
    std::atomic<bool> running;
    std::condition_variable m_runningCV;
    std::mutex m_runningMutex;

    void start();
    void stop();
public:
    
    std::chrono::milliseconds m_consumingInterval;


    FlapoConsumeThread(
        FlapoConsumer *consumer,
        FlapoProcessedDataQueue *processedDataQueue,
        std::chrono::milliseconds consumingInterval = std::chrono::milliseconds(1000),
        bool consumeOnStartup = true,
        bool usePollingInterval = true);
    ~FlapoConsumeThread();



};
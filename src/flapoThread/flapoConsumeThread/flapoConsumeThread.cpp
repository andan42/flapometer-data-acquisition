#include "flapoConsumeThread.h"
#include "flapoConsumer/flapoConsumer.h"
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"
#include <iostream>

void FlapoConsumeThread::consumeAllProcessedData()
{

    std::deque<std::shared_ptr<const FlapoProcessedData>> collapsedData = m_processedDataQueue->collapseMerge();

    while(collapsedData.size() > 0)
    {
        std::shared_ptr<const FlapoProcessedData> processedData = collapsedData.front();
        collapsedData.pop_front();
        m_consumer->consume(processedData);
        // thats it. thats all this thread does. it consumes data. it doesnt even know what it is consuming. it just consumes it.
        // std::cout << "Consumed data with sensor label: " << processedData->getSensorLabel() << std::endl;
        processedData->getEnd();
    }
}

void FlapoConsumeThread::start()
{
    thread = new std::thread([this] {
        if (!consumeOnStartup)
        {
            std::unique_lock<std::mutex> lk(m_runningMutex);
            m_runningCV.wait_for(lk, m_consumingInterval, [this]{return !running;});
        }
        while (running)
        {
            consumeAllProcessedData();
            std::unique_lock<std::mutex> lk(m_runningMutex);    
            //m_runningCV.wait_for(lk, m_consumingInterval, [this]{return !running;});
            if (usePollingInterval)
                m_runningCV.wait_for(lk, m_consumingInterval, [this]{return !running;});
            else
                m_runningCV.wait(lk, [this]{return !running;});
        }
    });
}

void FlapoConsumeThread::stop()
{
    if (consumeOnShutdown)
        consumeAllProcessedData();
    running = false;
    m_runningCV.notify_all();
    thread->join();
}

FlapoConsumeThread::FlapoConsumeThread(
    FlapoConsumer *consumer, //TODO implement FlapoConsumer
    FlapoProcessedDataQueue *processedDataQueue,
    std::chrono::milliseconds consumingInterval,
    bool consumeOnStartup,
    bool usePollingInterval)
: m_consumer(consumer), m_processedDataQueue(processedDataQueue), m_consumingInterval(consumingInterval), consumeOnStartup(consumeOnStartup), usePollingInterval(usePollingInterval) //TODO, consumeOnShutdown(consumeOnShutdown)
{
    running = true;
    start();
}

FlapoConsumeThread::~FlapoConsumeThread()
{
    stop();
    delete thread;
}
    
    
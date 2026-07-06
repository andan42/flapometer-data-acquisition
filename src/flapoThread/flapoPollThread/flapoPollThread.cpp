#include "flapoPollThread.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"

void FlapoPollThread::pollAllSensors()
{
   for (auto sensor : sensors)
    {
        FlapoRawData *raw = sensor->poll();
        if (raw != nullptr)
        {
            m_rawDataQueue->push(raw);
        }
    }
}

void FlapoPollThread::start()
{
    thread = new std::thread([this] {
        if (!pollOnStartup)
        {
            std::unique_lock<std::mutex> lk(m_runningMutex);
            m_runningCV.wait_for(lk, m_pollingInterval, [this]{return !running;});
        }
        while (running)
        {
            pollAllSensors();
            std::unique_lock<std::mutex> lk(m_runningMutex);   
            m_runningCV.wait_for(lk, m_pollingInterval, [this]{return !running;});
        }
    });
}

void FlapoPollThread::stop()
{
    if (pollOnShutdown)
        pollAllSensors();
    running = false;
    m_runningCV.notify_all();
    thread->join();
}

FlapoPollThread::FlapoPollThread(
    FlapoRawDataQueue *rawDataQueue, 
    std::chrono::milliseconds pollingInterval, 
    bool pollOnStartup, 
    bool pollOnShutdown)
: m_rawDataQueue(rawDataQueue), m_pollingInterval(pollingInterval), pollOnStartup(pollOnStartup), pollOnShutdown(pollOnShutdown)
{
    running = true;
    start();
}

FlapoPollThread::~FlapoPollThread()
{
    stop();
    delete thread;
}

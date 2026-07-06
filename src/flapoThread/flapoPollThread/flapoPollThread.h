#pragma once

#include <vector>
#include <thread>
#include <chrono>
#include <atomic>
#include <condition_variable>

#include "flapoSensors/flapoSensor.h"
class FlapoRawDataQueue; //#include "flapoRawDataQueue.h", forward declared

class FlapoPollThread
{
private:
    bool pollOnShutdown;
    bool pollOnStartup;
    std::thread *thread;
    void pollAllSensors();
    FlapoRawDataQueue *m_rawDataQueue;

    //running bool, thread safe
    std::atomic<bool> running;
    std::condition_variable m_runningCV;
    std::mutex m_runningMutex;

    void start();
    void stop();
public:
    std::vector<FlapoSensor*> sensors;

    std::chrono::milliseconds m_pollingInterval;


    FlapoPollThread(FlapoRawDataQueue *rawDataQueue,
    std::chrono::milliseconds pollingInterval = std::chrono::milliseconds(1000),
    bool pollOnStartup = false,
    bool pollOnShutdown = true);
    ~FlapoPollThread();
};
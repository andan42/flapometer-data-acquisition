#pragma once

#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"
#include "flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"

#include <iostream>
#include <memory>

#include <chrono>
#include <thread>

void testProcQueue()
{
    FlapoProcessedDataQueue *queue = new FlapoProcessedDataQueue();

    FlapoProcessedData *data = new FlapoProcessedData();
    std::shared_ptr<FlapoProcessedData> dataPtr(data);

    //test push and retrieve
    queue->push(dataPtr);

    FlapoProcessedData *data_ret = queue->pop().get();

    if (data == data_ret)
    {
        std::cout << "testProcQueue() passed" << std::endl;
    }
    else
    {
        std::cout << "testProcQueue() failed" << std::endl;
    }
    //delete data;  //this is managed by the shared_ptr
    
    FlapoProcessedData* data_2  = new FlapoProcessedData();
    std::shared_ptr<FlapoProcessedData> dataPtr_2(data_2);

    //save start time
    const auto start = std::chrono::system_clock::now();

    //test wait for data and retrieve (threaded)
    std::thread thread([queue, dataPtr_2, start] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "testProcQueue() pushing data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;
        queue->push(dataPtr_2);
    });

    std::cout << "testProcQueue() waiting for data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;
    FlapoProcessedData *data_ret_2 = queue->pop().get();
    std::cout << "testProcQueue() retrieved data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count() << std::endl;

    if (data_2 == data_ret_2)
    {
        std::cout << "testProcQueue() passed" << std::endl;
    }
    else
    {
        std::cout << "testProcQueue() failed" << std::endl;
    }


    thread.join();

    delete queue;

    //REPEAT THE SAME THING FOR THE RAW DATA QUEUE
    FlapoRawDataQueue *rawQueue = new FlapoRawDataQueue();

    FlapoRawData *rawData = new FlapoRawDataSpectro(nullptr);

    //test push and retrieve
    rawQueue->push(rawData);

    FlapoRawData *rawData_ret = rawQueue->pop();

    if (rawData == rawData_ret)
    {
        std::cout << "testProcQueue() passed" << std::endl;
    }
    else
    {
        std::cout << "testProcQueue() failed" << std::endl;
    }

    FlapoRawData* rawData_2 = new FlapoRawDataSpectro(nullptr);

    //save start time
    const auto start_2 = std::chrono::system_clock::now();

    //test wait for data and retrieve (threaded)
    std::thread thread_2([rawQueue, rawData_2, start_2] {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        std::cout << "testProcQueue() pushing data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_2).count() << std::endl;
        rawQueue->push(rawData_2);
    });

    std::cout << "testProcQueue() waiting for data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_2).count() << std::endl;
    FlapoRawData *rawData_ret_2 = rawQueue->pop();
    std::cout << "testProcQueue() retrieved data at time (milliseconds elapsed): " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_2).count() << std::endl;

    if (rawData_2 == rawData_ret_2)
    {
        std::cout << "testProcQueue() passed" << std::endl;
    }
    else
    {
        std::cout << "testProcQueue() failed" << std::endl;
    }


    thread_2.join();

    delete rawQueue;

    //REPEAT THE SAME THING FOR THE RAW DATA QUEUE

}
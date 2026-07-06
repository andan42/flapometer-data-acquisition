#pragma once

#include "i2c/DeviceI2C.h"

#include "flapoMax86171/configurator/flapoMax86171Controller.h"

#include "flapoThread/flapoPollThread/flapoPollThread.h"
#include "flapoThread/flapoProcessingThread/flapoProcessingThread.h"


#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"

#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"
#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"

#include "scrappyTests/testHelperSetupMeasurement.h"

#include <iostream>
#include <chrono>
#include <thread>

void testPollAndProcess()
{
    DeviceI2C *device = new DeviceI2C(0x65, 5);
    FlapoMax86171 *afe = new FlapoMax86171(device);
    FlapoMax86171Controller *controller = new FlapoMax86171Controller(afe);

    //pause and reset
    afe->writeRegister(flapoRegistersSpectro::SHDN, 1);
    afe->writeRegister(flapoRegistersSpectro::RESET, 1);

    //set some measurements
    helpSetupMeasurement(controller->getMeasurementController(1), 1, 0x7F);
    helpSetupMeasurement(controller->getMeasurementController(2), 6, 0x7F);
    helpSetupMeasurement(controller->getMeasurementController(4), 7, 0x7F);
    //power down a pd
    controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);

    //set clock
    controller->getClockController()->setClockFrequency(10.0, 0); //sample 10 Hz 

    //create sensor object
    FlapoSensorSpectro *sensor = new FlapoSensorSpectro();
    sensor->poller = new FlapoDataPollSpectro(afe);
    sensor->processor = new FlapoProcessSpectro();

    //set up process
    FlapoProcessSpectro *proc = (FlapoProcessSpectro *)sensor->processor;
    proc->nrPhotodiodes = 1; //because we powered down one
    proc->nrMeasurements = 3; //because we created 1 2 3 
    proc->putStartAndEndAlignmentData = false; //who cares rn
    proc->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0)
    };

    //create queues
    FlapoRawDataQueue *rawDataQueue = new FlapoRawDataQueue();
    FlapoProcessedDataQueue *processedDataQueue = new FlapoProcessedDataQueue();

    //create threads
    FlapoPollThread *pollThread = new FlapoPollThread(rawDataQueue, std::chrono::milliseconds(1000)); //poll at 1hz
    FlapoProcessingThread *processingThread = new FlapoProcessingThread(rawDataQueue);

    //add queues to threads
    pollThread->sensors.push_back(sensor);
    processingThread->addProcessedDataQueue(processedDataQueue);

    //auto save curernt time
    auto start = std::chrono::system_clock::now();

    std::this_thread::sleep_for(std::chrono::milliseconds(3500)); //take 1.5 secs

    //print time elapsed from saved time
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::cout << "elapsed time after sleep: " << elapsed_seconds.count() << "s\n";


    //lets do deletes now so we dont forget
    //do it in reverse

    delete pollThread;
    delete processingThread;

    //print time elapsed from saved time
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "elapsed time after delete proc thread: " << elapsed_seconds.count() << "s\n";


    
    //print time elapsed from saved time
    end = std::chrono::system_clock::now();
    elapsed_seconds = end-start;
    std::cout << "elapsed time after delete poll thread: " << elapsed_seconds.count() << "s\n";

    //lets see the data
    //delete threads first to make sure they dont try to access the queues
    int finalQueueSize = processedDataQueue->size();
    std::cout << "final queue size: " << finalQueueSize << std::endl;
    for(int i = 0; i < finalQueueSize; i++)
    {
        std::cout << "data point " << i << std::endl;
        std::cout << processedDataQueue->pop()->toStr() << std::endl;
    }
    
    delete processedDataQueue;
    delete rawDataQueue;

    //were only resetting here so that the very last poll that happens doesnt lose data
    //afe reset
    afe->writeRegister(flapoRegistersSpectro::RESET, 1);

    delete sensor->processor;
    delete sensor->poller;
    delete sensor;

    delete controller;
    delete afe;
    delete device;
}
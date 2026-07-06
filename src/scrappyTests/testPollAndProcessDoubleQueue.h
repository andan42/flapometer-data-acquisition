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

#include <assert.h>

void testPollAndProcessDoubleQueue()
{
    DeviceI2C *device = new DeviceI2C(0x65, 5);
    FlapoMax86171 *analogFrontEnd = new FlapoMax86171(device);
    FlapoMax86171Controller *controller = new FlapoMax86171Controller(analogFrontEnd);

    //pause and reset
    analogFrontEnd->writeRegister(flapoRegistersSpectro::SHDN, 1);
    analogFrontEnd->writeRegister(flapoRegistersSpectro::RESET, 1);

    //set some measurements
    helpSetupMeasurement(controller->getMeasurementController(1), 1, 0x7F);
    helpSetupMeasurement(controller->getMeasurementController(2), 6, 0x7F);
    helpSetupMeasurement(controller->getMeasurementController(4), 7, 0x7F);
    //power down a photodiode (usually the flapometer start with both powered up)
    controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);

    //set clock
    controller->getClockController()->setClockFrequency(10.0, 0); //sample 10 Hz 

    //create sensor object
    FlapoSensorSpectro *sensor = new FlapoSensorSpectro();
    // poller polls raw data from the sensor (using analogFrontEnd)
    // Processor turns raw data into a reasonable format
    sensor->poller = new FlapoDataPollSpectro(analogFrontEnd);
    sensor->processor = new FlapoProcessSpectro();

    //set up process
    FlapoProcessSpectro *proc = (FlapoProcessSpectro *)sensor->processor;
    proc->nrPhotodiodes = 1; //because we powered down one
    proc->nrMeasurements = 3; //because we created 1 2 3 
    proc->putStartAndEndAlignmentData = false; //who cares rn
    proc->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0),
    };

    //create queues
    FlapoRawDataQueue *rawDataQueue = new FlapoRawDataQueue();
    //FlapoProcessedDataQueue processedDataQueues[2] = {FlapoProcessedDataQueue(), FlapoProcessedDataQueue()};
    std::vector<std::unique_ptr<FlapoProcessedDataQueue>> processedDataQueues;
    processedDataQueues.push_back(std::make_unique<FlapoProcessedDataQueue>());
    processedDataQueues.push_back(std::make_unique<FlapoProcessedDataQueue>());


    //create threads
    FlapoPollThread *pollThread = new FlapoPollThread(rawDataQueue, std::chrono::milliseconds(1000)); //poll at 1hz
    FlapoProcessingThread *processingThread = new FlapoProcessingThread(rawDataQueue);

    //add queues to threads
    pollThread->sensors.push_back(sensor);
    processingThread->addProcessedDataQueue(processedDataQueues[0].get());
    processingThread->addProcessedDataQueue(processedDataQueues[1].get());

    //auto save curernt time
    auto start = std::chrono::system_clock::now();

    std::this_thread::sleep_for(std::chrono::milliseconds(3500)); //take 1.5 secs

    //lets do deletes now so we dont forget
    //do it in reverse

    delete pollThread;
    delete processingThread;

    //lets see the data
    //delete threads first to make sure they dont try to access the queues
   
    
    //assert that the two queues are the same
    
    assert(processedDataQueues[0]->size() == processedDataQueues[1]->size());


    for(int i = 0; i < processedDataQueues[0]->size(); i++)
    {
        assert((processedDataQueues[0]->pop())->isEqual(*processedDataQueues[1]->pop()));
    }



    //delete processedDataQueues;
    delete rawDataQueue;

    //were only resetting here so that the very last poll that happens doesnt lose data
    //afe reset
    analogFrontEnd->writeRegister(flapoRegistersSpectro::RESET, 1);

    delete sensor->processor;
    delete sensor->poller;
    delete sensor;

    delete controller;
    delete analogFrontEnd;
    delete device;
}
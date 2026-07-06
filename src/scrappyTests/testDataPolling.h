#pragma once

#include "i2c/DeviceI2C.h"

#include "flapoMax86171/configurator/flapoMax86171Controller.h"

#include "flapoThread/flapoPollThread/flapoPollThread.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"

#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"

#include <iostream>
#include <chrono>
#include <thread>

void testDataPolling()
{
    DeviceI2C *device = new DeviceI2C(0x64, 5);
    FlapoMax86171 *afe = new FlapoMax86171(device);
    FlapoMax86171Controller *controller = new FlapoMax86171Controller(afe);


    int val = 0;


    //reset
    afe->writeRegister(flapoRegistersSpectro::RESET, 1);
    //enable meas1
    controller->getMeasurementController(1)->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 0x1);
    //set meas1 to pin 7
    controller->getMeasurementController(1)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA, 3);
    //set meas1 current to 0x7F
    controller->getMeasurementController(1)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 0x7F);
    
    
    //enable meas2 
    controller->getMeasurementController(2)->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 0x1);
    //set meas2 to pin 7
    controller->getMeasurementController(2)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA, 3);
    //set meas2 current to 0x7F
    controller->getMeasurementController(2)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 0x7F);

    
    //enable meas6 
    controller->getMeasurementController(6)->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 0x1);
    //
    controller->getMeasurementController(6)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVC, 2);
    //
    controller->getMeasurementController(6)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 0x7F);
    
    //set clock
    controller->getClockController()->setClockFrequency(10.0, 0); 
    if(controller->getAfe() == nullptr)
    {
        std::cout << "controller->getAfe() == nullptr" << std::endl;
    }
    //lets also disable one of the photodiodes just to make sure we get only one int per poll
    controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);

    //create raw data queue
    FlapoRawDataQueue *rawDataQueue = new FlapoRawDataQueue();

    //create poller thread
    FlapoPollThread *pollThread = new FlapoPollThread(rawDataQueue, std::chrono::milliseconds(1000));

    //create sensor poller
    FlapoDataPollSpectro *dataPoll = new FlapoDataPollSpectro(afe);
    //create sensor object
    FlapoSensorSpectro *sensor = new FlapoSensorSpectro();
    sensor->poller = dataPoll;

    //add sensor to polled sensor list
    pollThread->sensors.push_back(sensor);


    std::this_thread::sleep_for(std::chrono::milliseconds(10000));
    //reset
    afe->writeRegister(flapoRegistersSpectro::RESET, 1); //  

    //loop through data queue and see what we got!
    const std::queue<FlapoRawData *> &queueRef = rawDataQueue->getQueue();
    std::queue<FlapoRawData *> queueCopy = queueRef;

    std::cout << "queue size: " << rawDataQueue->getQueue().size() << std::endl;
    std::cout << "queue size: " << queueCopy.size() << std::endl;

    int i = 0;
    while(queueCopy.size() > 0)
    {

        FlapoRawData *rawDataPacket = queueCopy.front();
        queueCopy.pop();
        
        std::cout << "  FlapoRawData #" << i << "   contains " << rawDataPacket->dataCount << " samples" << std::endl; 
        for (int j = 0; j < rawDataPacket->dataCount; j++)
        {
            FlapoMaxSample sample = FlapoMaxSample(rawDataPacket->data[j]);
            std::cout << "      sample: tag: " << sample.getTag() << " val: " << sample.getValue() << std::endl;
        }


        delete rawDataPacket;
        i++;
    }

    delete pollThread;
    delete rawDataQueue;
    delete dataPoll;
    delete controller;
    delete afe;
    delete device;
}
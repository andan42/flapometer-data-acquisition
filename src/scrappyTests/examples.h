#pragma once

#include "i2c/DeviceI2C.h"

#include <iostream>
#include <vector>
#include <chrono>
//#include <thread>
#include <memory>

#include "flapoMax86171/configurator/flapoMax86171Controller.h"
#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"
#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"
#include "flapoThread/flapoPollThread/flapoPollThread.h"
#include "flapoThread/flapoProcessingThread/flapoProcessingThread.h"



namespace flapoExamples{
    //this is how to create a device
    DeviceI2C* createDevice(int address, int bus)
    {
        return new DeviceI2C(address, bus);
    }

    //this is how to create an afe
    FlapoMax86171* createAfe(DeviceI2C *device)
    {
        return new FlapoMax86171(device);
    }

    //this is how to create a controller
    FlapoMax86171Controller* createController(FlapoMax86171 *afe)
    {
        return new FlapoMax86171Controller(afe);
    }

    //this is how to create a data poller
    FlapoDataPollSpectro* createDataPoller(FlapoMax86171 *afe)
    {
        return new FlapoDataPollSpectro(afe);
    }

    //create and setup process
    FlapoProcessSpectro* createDataProcessor()
    {
        FlapoProcessSpectro* ret = new FlapoProcessSpectro();
        ret->nrPhotodiodes = 1; //one will be powered down for simplicity
        ret->nrMeasurements = 2; //we will create 2 measurements for demo
        ret->putStartAndEndAlignmentData = false; //who cares rn
        ret->channelsInOrder = {
            FlapoSpectroChannel(1, 0),
            FlapoSpectroChannel(2, 0)
        }; //enable first two channels to avoid confusion
        return ret;
    }

    //this is how to create a sensor
    FlapoSensorSpectro* createSensor(FlapoDataPollSpectro *poller, FlapoProcessSpectro *processor)
    {
        FlapoSensorSpectro *ret = new FlapoSensorSpectro();
        ret->poller = poller;
        ret->processor = processor;

        return ret;
    }

    //this is how to use the measurement controller of a controller
    FlapoMax86171MeasurementController* getMeasurementController(FlapoMax86171Controller *controller, int channel)
    {
        return controller->getMeasurementController(channel);
        //the measurement controller takes as arguments functions that resolve which measurement is supposed to be done
        //for example:
        //controller->getMeasurementController(3)->writeRegister(flapoRegistersSpectro::MEAS_N_EN, value);
        //where the measurement is "3"
        //the value is "value"
        //and N is a standin for the measurement number, for hte function name
    }

    //this is how to set up a measurement using the controller and its measurement controller
    void setupMeasurement(FlapoMax86171Controller *controller, int measurementNr, int pin_bitcode = 3) //bitfield 3 selects pin 7 which is visible
    {
        //enable the measurement
        controller->getMeasurementController(measurementNr)->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 1);
        //take for example drva and pin 7 (pin 7 is visible wavelength)
        controller->getMeasurementController(measurementNr)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA, pin_bitcode);
        //set the current to 0x7F
        controller->getMeasurementController(measurementNr)->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 0x7F);
    }

    //this is how to use the clock controller of a controller
    FlapoMax86171ClockController* getClockController(FlapoMax86171Controller *controller)
    {
        return controller->getClockController();
    }

    //this is how to set up the clock
    void setupClock(FlapoMax86171ClockController *clock)
    {
        //set the clock to 25Hz
        clock->setClockFrequency(25.0, 0);

        //OR set divider and source manually
        //clock->setClockSource(0); //32000Hz source
        //clock->setClockDivider(1280); //32000Hz / 25Hz = 1280
    }

    //this is how to reset the chip
    void resetChip(FlapoMax86171 *afe)
    {
        afe->writeRegister(flapoRegistersSpectro::RESET, 1);
    }

    //this is how to shutdown the chip
    void shutdownChip(FlapoMax86171 *afe)
    {
        afe->writeRegister(flapoRegistersSpectro::SHDN, 1);
    }

    //this is how to wake up the chip
    void wakeupChip(FlapoMax86171 *afe)
    {
        afe->writeRegister(flapoRegistersSpectro::SHDN, 0);
    }

    //queue and thread stuff

    //this is how to create a RAW data queue
    FlapoRawDataQueue* createRawDataQueue()
    {
        return new FlapoRawDataQueue();
    }

    //this is how to create some PROCESSED data queues
    std::vector<std::unique_ptr<FlapoProcessedDataQueue>> createProcessedDataQueues(int nrQueues)
    {
        std::vector<std::unique_ptr<FlapoProcessedDataQueue>> ret;
        for (int i = 0; i < nrQueues; i++)
        {
            ret.push_back(std::make_unique<FlapoProcessedDataQueue>());
        }
        return ret;
    }

    //this is how to create a poller thread - and add source sensors to it
    FlapoPollThread* createPollThread(
        FlapoRawDataQueue *rawDataQueue,  //queue to put data in
        std::chrono::milliseconds pollInterval, //interval at which to poll
        std::vector<FlapoSensor*> sensors) //sensors to be polled. all get queued to the same queue, regardless of sensor type 
    {
        FlapoPollThread *ret = new FlapoPollThread(rawDataQueue, pollInterval);
        for (int i = 0; i < sensors.size(); i++)
        {
            ret->sensors.push_back(sensors[i]);
        }
        return ret;
    }

    //this is how to create a processing thread	- and add target queues to it
    FlapoProcessingThread* createProcessingThread(
        FlapoRawDataQueue *rawDataQueue, //queue to get data from
        std::vector<FlapoProcessedDataQueue*> processedDataQueues) //queues to put data in
    {
        FlapoProcessingThread *ret = new FlapoProcessingThread(rawDataQueue);
        for (int i = 0; i < processedDataQueues.size(); i++)
        {
            ret->addProcessedDataQueue(processedDataQueues[i]);
        }
        return ret;
    }

    //putting it all together
    void runExample()
    {
        //create device
        DeviceI2C *device = createDevice(0x65, 5);

        //create afe
        FlapoMax86171 *afe = createAfe(device);

        //create controller
        FlapoMax86171Controller *controller = createController(afe);

        //pause and reset
        shutdownChip(afe);
        resetChip(afe);

        //set up measurements  --> in this demo we only do 2 measurements, same as processor is configured by its demo function
        setupMeasurement(controller, 1);
        setupMeasurement(controller, 2, 2); //bitfield 2 on drva selects pin 6 which is infrared, but whatever fuck it

        //power down a pd
        afe->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);

        //set clock
        setupClock(controller->getClockController());

        //create poller
        FlapoDataPollSpectro *poller = createDataPoller(afe);

        //create processor (and set it up)
        FlapoProcessSpectro *processor = createDataProcessor();

        //create sensor
        FlapoSensorSpectro *sensor = createSensor(poller, processor);

        //create raw data queue
        FlapoRawDataQueue *rawDataQueue = createRawDataQueue();

        //create processed data queues
        std::vector<std::unique_ptr<FlapoProcessedDataQueue>> processedDataQueues = createProcessedDataQueues(2);

        //create poll thread
        FlapoPollThread *pollThread = createPollThread(rawDataQueue, std::chrono::milliseconds(1000), { sensor });

        //create processing thread
        FlapoProcessingThread *processingThread = createProcessingThread(rawDataQueue, { processedDataQueues[0].get(), processedDataQueues[1].get() });

        //add queues to threads
        pollThread->sensors.push_back(sensor);
        processingThread->addProcessedDataQueue(processedDataQueues[0].get());
        processingThread->addProcessedDataQueue(processedDataQueues[1].get());

        //record for 3.5 seconds
        std::this_thread::sleep_for(std::chrono::milliseconds(3500)); 

        //deletes. optimally use smart pointers

        delete pollThread;
        delete processingThread;

        delete rawDataQueue;
        //processedDataQueues are smart pointers in this demo, so they delete themselves


        //check data here before deleting queues
        //processed
        int nrProcessedDataQueues = processedDataQueues.size();
        for (int i = 0; i < nrProcessedDataQueues; i++)
        {
            std::cout << ">QUEUE NUMBER " << i << std::endl;
            for (int j = 0; j < processedDataQueues[i]->size(); j++)
            {
                std::cout << " >element" << j << std::endl;
                //std::cout << "  >" << processedDataQueues[i]->pop()->toStr() << std::endl;
            }
        }

        //reset chip.
        afe->writeRegister(flapoRegistersSpectro::RESET, 1);
    
        delete sensor->processor;
        delete sensor->poller;
        delete sensor;

        delete controller;
        delete afe;
        delete device;
    }
};
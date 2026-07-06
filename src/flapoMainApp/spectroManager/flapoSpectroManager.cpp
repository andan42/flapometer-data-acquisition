#include "flapoSpectroManager.h"
#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "i2c/DeviceI2C.h"
#include <iostream>

#include "flapoUtils/i2cutils.h"

FlapoSpectroManager::FlapoSpectroManager(
    int bus, const std::string &sensorSpectroLabel, const FlapoOperationMetadata &operationMetadata)
    : label(sensorSpectroLabel), operationMetadata(operationMetadata)
{
    // create device and controller [spectro]
    int deviceAddress = scanBusFlapDevice(bus);
    if (deviceAddress < 0)
    {
        if (bus == 4)
        {
            deviceAddress = 0x65; // todo implement saving the last known address
        }
        else if (bus == 6)
        {
            deviceAddress = 0x65; // todo implement saving the last known address
        }
        else
        {
            deviceAddress = 0x64; // todo implement saving the last known address
        }
    }

    device = new DeviceI2C(deviceAddress, bus);
    if (device->m_error < 0)
    {
        // todo fail gracefully
        // throw std::runtime_error("Could not create spectro device");
    }
    afe = new FlapoMax86171(device);
    afe->startCaptureCommands();
    controller = new FlapoMax86171Controller(afe);

    chipSetup();

    // set sensor metadata
    sensorClass = new FlapoSensorSpectro(sensorSpectroLabel, operationMetadata);

    // create poller [spectro]
    sensorClass->poller = new FlapoDataPollSpectro(controller->getAfe());
    sensorClass->processor = new FlapoProcessSpectro();

    processorSetup();
    afe->stopCaptureCommands();
}

void FlapoSpectroManager::chipSetup()
{ // pause and reset chip
    controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 1);
    controller->getAfe()->writeRegister(flapoRegistersSpectro::RESET, 1);

    // disable a photodiode (affects all measurements)
    // controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);

    // set up measurements
    int pins[] = {7, 6, 5, 8, 1, 9, 4, 3, 2};
    int intensities[] = {0xA0, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}; // 550 nm needs more light

    setup = new FlapoMax86171Setup(controller, 9, pins, intensities, nullptr); // not a memory leak. this gets deleted in the destructor, and only created in constructor
    setup->init();

    // set up clock
    controller->getClockController()->setClockFrequency(25.0, 0);
}

void FlapoSpectroManager::processorSetup()
{
    // set up processor
    sensorClass->processor->nrPhotodiodes = 2;
    sensorClass->processor->nrMeasurements = 9;
    sensorClass->processor->putStartAndEndAlignmentData = true;
    sensorClass->processor->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(3, 1),
        FlapoSpectroChannel(4, 0),
        FlapoSpectroChannel(4, 1),
        FlapoSpectroChannel(5, 0),
        FlapoSpectroChannel(5, 1),
        FlapoSpectroChannel(6, 0),
        FlapoSpectroChannel(6, 1),
        FlapoSpectroChannel(7, 0),
        FlapoSpectroChannel(7, 1),
        FlapoSpectroChannel(8, 0),
        FlapoSpectroChannel(8, 1),
        FlapoSpectroChannel(9, 0),
        FlapoSpectroChannel(9, 1),
    };
    // temporary way to do this
    sensorClass->m_setup = setup;
    sensorClass->setSensorMetadataIntensities(setup->getIntensities());
}

FlapoSpectroManager::~FlapoSpectroManager()
{
    delete sensorClass->processor;
    delete sensorClass->poller;
    delete sensorClass;
    delete setup;
    delete controller;
    delete afe;
    delete device;
}

void FlapoSpectroManager::start()
{
    controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 0);
}

void FlapoSpectroManager::stop()
{
    controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 1);
}

void FlapoSpectroManager::reset()
{
    controller->getAfe()->writeRegister(flapoRegistersSpectro::RESET, 1);
}

void FlapoSpectroManager::dumpAllRegisters()
{
    uint8_t afeRegisterDump[0x80]; // could be 7F but easier to loop this
    controller->getAfe()->dumpRegisters(afeRegisterDump);
    std::cout << "      0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E" << std::endl;
    std::cout << "      --------------------------------------------------------------------------" << std::endl;
    std::cout << "0"; // whitespace to align
    for (int i = 0; i < 8; i++)
    {
        std::cout << std::hex << i * 0x10 << " : ";
        for (int j = 0; j < 0xF; j++)
        {
            int index = i * 0x10 + j;
            int val = (int)afeRegisterDump[index];
            if (val < 0x10)
                std::cout << " 0x0" << std::hex << val;
            else
                std::cout << " 0x" << std::hex << val;
        }
        std::cout << std::endl;
    }
    std::cout << "      --------------------------------------------------------------------------" << std::endl;
}

// int photodiodes[9][2] = { {0,0},{1,1},{0,0},{1,1},{0,0},{1,1},{0,0},{1,1},{0,0}} ;
// using nullptr automatically selects the right photodiodes
//  {0,0} is PD1 for PPG1 and PD2 for PPG2 -> this should be used in measurements that use leds with  550   650   750   850   950  wavelengths
//  {1,1} is PD3 for PPG1 and PD4 for PPG2 -> this should be used in measurements that use leds with     600   700   800   900     wavelengths
//  {0,1} is PD1 for PPG1 and PD4 for PPG2
//  {1,0} is PD3 for PPG1 and PD2 for PPG2
//  PPG2 is disabled, so only PD1 and PD3 are used. second number is ignored.

// controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);
// this disables PPG2 which is PD2 and PD4 (depending on PDSEL)

// 7=550nm; 6=600nm; 5=650nm; 8=700nm; 1=750nm; 9=800nm; 4=850nm; 3=900nm; 2=950nm
// int pins[] = {7, 6, 5, 8, 1, 9, 4, 3, 2};

// pins 1 2 4 5 7 correspond to PD1 and PD3 => photodiodes[][] = {1,1}
// pins 3 6 8 9 correspond to PD2 and PD4 => photodiodes[][] = {0,0}
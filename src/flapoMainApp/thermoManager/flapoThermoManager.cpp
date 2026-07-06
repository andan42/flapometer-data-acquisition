#include "flapoThermoManager.h"
#include "i2c/DeviceI2C.h"

FlapoThermoManager::FlapoThermoManager(
    const std::string &sensorThermoLabel, const FlapoOperationMetadata &operationMetadata)
    : label(sensorThermoLabel), operationMetadata(operationMetadata)
{
    // create device and controller [thermo]
    device = new DeviceI2C(0x48, 1);
    if (device->m_error < 0)
    {
        // todo fail gracefully
        throw std::runtime_error("Could not create thermo device");
    }
    adc = new FlapoPcf8591(device);

    // set sensor metadata
    sensorClass = new FlapoSensorThermo(sensorThermoLabel, operationMetadata);

    // create poller [thermo]
    sensorClass->poller = new FlapoDataPollThermo(adc);
    sensorClass->processor = new FlapoProcessThermo();

    processorSetup();
}

FlapoThermoManager::~FlapoThermoManager()
{
    delete sensorClass->processor;
    delete sensorClass->poller;
    delete sensorClass;
    delete adc;
    delete device;
}

void FlapoThermoManager::processorSetup()
{
    sensorClass->processor->enableChannel(0);
    sensorClass->processor->enableChannel(1);
}

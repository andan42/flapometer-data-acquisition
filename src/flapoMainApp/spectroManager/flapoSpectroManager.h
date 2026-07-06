#include <string>

#include "flapoMax86171/configurator/flapoMax86171Controller.h"
#include "flapoMax86171/configurator/setup/flapoMax86171Setup.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

class FlapoSpectroManager
{
public:
    DeviceI2C *device;
    FlapoMax86171 *afe;
    FlapoMax86171Controller *controller;
    FlapoMax86171Setup *setup;

    FlapoSensorSpectro *sensorClass;

    const std::string label;
    const FlapoOperationMetadata operationMetadata;

    FlapoSpectroManager(int bus, const std::string &sensorSpectroLabel, const FlapoOperationMetadata &operationMetadata);
    ~FlapoSpectroManager();
    void start();
    void stop();
    void reset();

private:
    void chipSetup();
    void processorSetup();
    void dumpAllRegisters();
};
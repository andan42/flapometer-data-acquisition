#include <string>

#include "flapoSensors/thermo/flapoSensorThermo/flapoSensorThermo.h"
#include "flapoPcf8591/chip/flapoPcf8591.h"

class FlapoThermoManager
{
public:
    DeviceI2C *device;
    FlapoPcf8591 *adc;
    FlapoSensorThermo *sensorClass;

    const std::string label;
    const FlapoOperationMetadata operationMetadata;

    FlapoThermoManager(const std::string &sensorSpectroLabel, const FlapoOperationMetadata &operationMetadata);
    ~FlapoThermoManager();

private:
    void processorSetup();
};
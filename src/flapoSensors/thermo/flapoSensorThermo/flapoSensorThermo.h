#pragma once
#include "flapoSensors/flapoSensor.h"

#include "flapoSensors/thermo/flapoPollThermo/flapoPollThermo.h"
#include "flapoSensors/thermo/flapoProcessThermo/flapoProcessThermo.h"
class FlapoProcessThermo;

#include "flapoMetadata/flapoSensorChannelMetadata/flapoSensorChannelMetadata.h"

#include <memory>
#include <array>

class FlapoSensorThermo : public FlapoSensor
{
private:
    static const FlapoSensorTypeMetadata sensorTypeMetadata;
    //const FlapoSensorChannelMetadata m_sensorChannelMetadata[4];
    const std::array<FlapoSensorChannelMetadata, 4> m_sensorChannelMetadata;
public:

    FlapoProcessThermo* processor;
    FlapoSensorThermo(const std::string& sensorLabel, const FlapoOperationMetadata& operationMetadata);
    ~FlapoSensorThermo();
    FlapoRawData* poll() override;
    std::vector<std::shared_ptr<const FlapoProcessedData>> process(const FlapoRawData* rawData) const override;

    const FlapoSensorChannelMetadata getSensorChannelMetadata(int i) const;
    void setSensorMetadataIntensities(const std::vector<int> intensities) override;
};




#pragma once
#include "flapoSensors/flapoSensor.h"

#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h" 

#include "flapoMetadata/flapoSensorChannelMetadata/flapoSensorChannelMetadata.h"

#include <memory>

class FlapoMax86171Setup;
class FlapoSensorSpectro : public FlapoSensor
{
private:
    static const FlapoSensorTypeMetadata sensorTypeMetadata;
    FlapoSensorChannelMetadata m_sensorChannelMetadata;
public:
    FlapoMax86171Setup* m_setup = nullptr;

    FlapoProcessSpectro* processor; //consider putting this in the parent class as FlapoDataProcess
    FlapoSensorSpectro(const std::string& sensorLabel, FlapoOperationMetadata operation);
    ~FlapoSensorSpectro();
    FlapoRawData* poll() override;
    std::vector<std::shared_ptr<const FlapoProcessedData>> process(const FlapoRawData* rawData) const override;

    const FlapoSensorChannelMetadata getSensorChannelMetadata() const;
    void setSensorMetadataIntensities(const std::vector<int> intensities) override;
};
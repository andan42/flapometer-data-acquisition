#pragma once
#include <string>
#include <vector>
#include "flapoMetadata/flapoSensorMetadata/flapoSensorMetadata.h"

class FlapoSensor;
class FlapoSensorChannelMetadata
{
private:
    const std::string m_channelPostscript;
    FlapoSensorMetadata m_sensor;
    const std::string m_sensorLabelWithPostscript;
public:
    FlapoSensorChannelMetadata(const std::string channelPostscript, const FlapoSensorMetadata sensor);
    FlapoSensorChannelMetadata(const FlapoSensorChannelMetadata& other);
    FlapoSensorChannelMetadata& operator=(const FlapoSensorChannelMetadata& other) = delete;
    FlapoSensorChannelMetadata& operator=(FlapoSensorChannelMetadata&& other) = delete;
    ~FlapoSensorChannelMetadata();

    FlapoSensor* getSensorPtr() const;
    const std::string& getSensorLabel() const;
    const std::string& getSensorType() const;
    const std::string& getOperationLabel() const;
    const std::string& getPatientName() const;
    const std::vector<int>& getIntensities() const;
    void setIntensities(const std::vector<int> intensities);
    bool operator==(const FlapoSensorChannelMetadata& other) const;
    bool operator!=(const FlapoSensorChannelMetadata& other) const;
    bool isEqual(const FlapoSensorChannelMetadata& other) const;
};
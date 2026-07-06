#pragma once
#include <string>
#include <vector>

#include "flapoMetadata/flapoOperationMetadata/flapoOperationMetadata.h"
#include "flapoMetadata/flapoSensorTypeMetadata/flapoSensorTypeMetadata.h"

class FlapoSensor;
class FlapoSensorMetadata
{
private:
    FlapoSensor *const m_sensorPtr;
    const std::string& m_sensorLabel;
    const FlapoOperationMetadata m_operation;
    const FlapoSensorTypeMetadata m_sensorType;
    std::vector<int> m_intensities;

public:
    FlapoSensorMetadata(const std::string& sensorLabel, FlapoSensor *const sensorPtr, FlapoOperationMetadata operation, FlapoSensorTypeMetadata sensorType);
    FlapoSensorMetadata(const FlapoSensorMetadata& other);
    FlapoSensorMetadata &operator=(const FlapoSensorMetadata& other) = delete;
    FlapoSensorMetadata &operator=(FlapoSensorMetadata&& other) = delete;
    ~FlapoSensorMetadata();
    
    void setIntensities(const std::vector<int> intensities);

    FlapoSensor* getSensorPtr() const;
    const std::string& getSensorLabel() const;
    const std::string& getSensorType() const;
    const std::string& getOperationLabel() const;
    const std::string& getPatientName() const;
    const std::vector<int>& getIntensities() const;
    bool operator==(const FlapoSensorMetadata& other) const;
    bool operator!=(const FlapoSensorMetadata& other) const;
    bool isEqual(const FlapoSensorMetadata& other) const;
};


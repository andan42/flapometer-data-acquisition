#pragma once
#include <string>

//#include "flapoMetadata/flapoOperationMetadata/flapoOperationMetadata.h"

class FlapoSensorTypeMetadata
{
private:
    const std::string m_sensorType;
    //const FlapoOperationMetadata m_operation;

public:
    FlapoSensorTypeMetadata(const std::string sensorType /*, const FlapoOperationMetadata operation*/);
    FlapoSensorTypeMetadata(const FlapoSensorTypeMetadata& other);
    FlapoSensorTypeMetadata& operator=(const FlapoSensorTypeMetadata& other) = delete;
    FlapoSensorTypeMetadata& operator=(FlapoSensorTypeMetadata&& other) = delete;
    ~FlapoSensorTypeMetadata();

    const std::string& getSensorType() const;
    const std::string& getOperationLabel() const;
    const std::string& getPatientName() const;
    bool operator==(const FlapoSensorTypeMetadata& other) const;
    bool isEqual(const FlapoSensorTypeMetadata& other) const;
};

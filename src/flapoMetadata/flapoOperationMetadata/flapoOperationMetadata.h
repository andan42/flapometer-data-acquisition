#pragma once
#include <string>

//#include "flapoMetadata/flapoSensorTypeMetadata/flapoSensorTypeMetadata.h"

class FlapoOperationMetadata
{
private:
    const std::string m_operationLabel;
    const std::string m_patientName;
public:
    FlapoOperationMetadata(const std::string operationLabel, const std::string patientName/*, const FlapoSensorTypeMetadata sensorType*/);
    FlapoOperationMetadata(const FlapoOperationMetadata& other);
    FlapoOperationMetadata& operator=(const FlapoOperationMetadata& other) = delete;
    FlapoOperationMetadata& operator=(FlapoOperationMetadata&& other) = delete;
    ~FlapoOperationMetadata();

    const std::string& getOperationLabel() const;
    const std::string& getPatientName() const;
    bool operator==(const FlapoOperationMetadata& other) const;
    bool operator!=(const FlapoOperationMetadata& other) const;
    bool isEqual(const FlapoOperationMetadata& other) const;

};
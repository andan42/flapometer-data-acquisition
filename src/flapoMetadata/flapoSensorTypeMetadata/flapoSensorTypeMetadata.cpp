#include "flapoSensorTypeMetadata.h"

FlapoSensorTypeMetadata::FlapoSensorTypeMetadata(const std::string sensorType/*, const FlapoOperationMetadata operation*/):
    m_sensorType(sensorType)
    //,m_operation(operation)
{
}

FlapoSensorTypeMetadata::FlapoSensorTypeMetadata(const FlapoSensorTypeMetadata &other) :
    m_sensorType(other.m_sensorType)
    //,m_operation(other.m_operation)
{
}

FlapoSensorTypeMetadata::~FlapoSensorTypeMetadata()
{
}

const std::string &FlapoSensorTypeMetadata::getSensorType() const
{
    return m_sensorType;
}

/*const std::string &FlapoSensorTypeMetadata::getOperationLabel() const
{
    return m_operation.getOperationLabel();
}

const std::string &FlapoSensorTypeMetadata::getPatientName() const
{
    return m_operation.getPatientName();
}*/

bool FlapoSensorTypeMetadata::operator==(const FlapoSensorTypeMetadata &other) const
{
    return m_sensorType == other.m_sensorType /*&& m_operation == other.m_operation*/;
}

bool FlapoSensorTypeMetadata::isEqual(const FlapoSensorTypeMetadata &other) const
{
    return m_sensorType == other.m_sensorType /*&& m_operation == other.m_operation*/;
}



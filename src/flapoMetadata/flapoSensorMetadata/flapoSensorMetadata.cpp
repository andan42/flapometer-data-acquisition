#include "flapoSensorMetadata.h"

FlapoSensorMetadata::FlapoSensorMetadata(const std::string& sensorLabel, FlapoSensor* const sensorPtr, FlapoOperationMetadata operation, FlapoSensorTypeMetadata sensorType):
    m_sensorLabel(sensorLabel),
    m_sensorPtr(sensorPtr),
    m_operation(operation),
    m_sensorType(sensorType),
    m_intensities()
{
}

FlapoSensorMetadata::FlapoSensorMetadata(const FlapoSensorMetadata& other):
    m_sensorLabel(other.m_sensorLabel),
    m_sensorPtr(other.m_sensorPtr),
    m_operation(other.m_operation),
    m_sensorType(other.m_sensorType),
    m_intensities(other.m_intensities)
{
}

FlapoSensorMetadata::~FlapoSensorMetadata()
{
}

void FlapoSensorMetadata::setIntensities(const std::vector<int> intensities)
{
    m_intensities = intensities;
}

FlapoSensor *FlapoSensorMetadata::getSensorPtr() const
{
    return m_sensorPtr;
}

const std::string &FlapoSensorMetadata::getSensorLabel() const
{
    return m_sensorLabel;
}

const std::string &FlapoSensorMetadata::getSensorType() const
{
    return m_sensorType.getSensorType();
}

const std::string &FlapoSensorMetadata::getOperationLabel() const
{
    return m_operation.getOperationLabel();
}

const std::string &FlapoSensorMetadata::getPatientName() const
{
    return m_operation.getPatientName();
}

const std::vector<int> &FlapoSensorMetadata::getIntensities() const
{
    return m_intensities;
}

bool FlapoSensorMetadata::operator==(const FlapoSensorMetadata &other) const
{
    //compare non vector fields first
    if(!(
        m_sensorPtr == other.m_sensorPtr &&
        m_sensorLabel == other.m_sensorLabel &&
        m_operation == other.m_operation &&
        m_sensorType == other.m_sensorType))
    {
        return false;
    }
    //compare vectors
    if(m_intensities.size() != other.m_intensities.size())
    {
        return false;
    }
    for(int i = 0; i < m_intensities.size(); i++)
    {
        if(m_intensities[i] != other.m_intensities[i])
        {
            return false;
        }
    }
    return true;
}

bool FlapoSensorMetadata::operator!=(const FlapoSensorMetadata &other) const
{
    return !(*this == other);
}

bool FlapoSensorMetadata::isEqual(const FlapoSensorMetadata &other) const
{
    return m_sensorLabel == other.m_sensorLabel && m_operation == other.m_operation && m_sensorType == other.m_sensorType;
}


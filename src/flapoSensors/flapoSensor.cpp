#include "flapoSensor.h"
#include "flapoUtils/flapoReferenceTime/flapoReferenceTime.h"

void FlapoSensor::setLastTimePolled(std::chrono::steady_clock::time_point timePolled)
{
    m_lastTimePolled = timePolled;
}

FlapoSensor::FlapoSensor(const std::string &sensorLabel, const FlapoOperationMetadata operationMetadata, const FlapoSensorTypeMetadata sensorTypeMetadata)
    : m_sensorMetadata(sensorLabel, this, operationMetadata, sensorTypeMetadata),
      m_lastTimePolled(getUnoffsetReferenceTime()) // initialize to the reference time. first sensor initialization will be the reference time (static function)
{

}

FlapoSensor::~FlapoSensor()
{

}

const FlapoSensorMetadata FlapoSensor::getSensorMetadata() const
{
    return m_sensorMetadata;
}

std::chrono::steady_clock::time_point FlapoSensor::getLastTimePolled() const
{
    return m_lastTimePolled;
}

/*const char *FlapoSensor::getSensorLabel() const
{
    return sensorLabel;
}*/

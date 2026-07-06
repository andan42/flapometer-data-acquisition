#include "flapoSensorChannelMetadata.h"

FlapoSensorChannelMetadata::FlapoSensorChannelMetadata(const std::string channelPostscript, const FlapoSensorMetadata sensor)
    : m_channelPostscript(channelPostscript),
    m_sensor(sensor),
    m_sensorLabelWithPostscript(sensor.getSensorLabel() + channelPostscript)
{
}

FlapoSensorChannelMetadata::FlapoSensorChannelMetadata(const FlapoSensorChannelMetadata & other)
    : m_channelPostscript(other.m_channelPostscript),
    m_sensor(other.m_sensor),
    m_sensorLabelWithPostscript(other.m_sensorLabelWithPostscript)
{
}

FlapoSensorChannelMetadata::~FlapoSensorChannelMetadata()
{
}

FlapoSensor *FlapoSensorChannelMetadata::getSensorPtr() const
{
    return m_sensor.getSensorPtr();
}

const std::string &FlapoSensorChannelMetadata::getSensorLabel() const
{
    return m_sensorLabelWithPostscript;
}

const std::string& FlapoSensorChannelMetadata::getSensorType() const
{
    return m_sensor.getSensorType();
}

const std::string& FlapoSensorChannelMetadata::getOperationLabel() const
{
    return m_sensor.getOperationLabel();
}

const std::string &FlapoSensorChannelMetadata::getPatientName() const
{
    return m_sensor.getPatientName();
}

const std::vector<int> &FlapoSensorChannelMetadata::getIntensities() const
{
    return m_sensor.getIntensities();
}

void FlapoSensorChannelMetadata::setIntensities(const std::vector<int> intensities)
{
    m_sensor.setIntensities(intensities);
}

bool FlapoSensorChannelMetadata::operator==(const FlapoSensorChannelMetadata &other) const
{
    return m_channelPostscript == other.m_channelPostscript && m_sensor == other.m_sensor;
}

bool FlapoSensorChannelMetadata::operator!=(const FlapoSensorChannelMetadata &other) const
{
    return !(*this == other);
}

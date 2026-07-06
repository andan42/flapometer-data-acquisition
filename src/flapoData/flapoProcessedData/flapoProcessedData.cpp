#include "flapoProcessedData.h"

FlapoProcessedData::FlapoProcessedData(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, const FlapoSensorChannelMetadata sensorChannelMetadata)
    : start(start), end(end), m_sensorChannelMetadata(sensorChannelMetadata)
{
}

FlapoProcessedData::~FlapoProcessedData()
{
}

std::chrono::steady_clock::time_point FlapoProcessedData::getStart() const
{
    return start;
}

std::chrono::steady_clock::time_point FlapoProcessedData::getEnd() const
{
    return end;
}

const std::vector<int> FlapoProcessedData::getIntensities() const
{
    return this->m_sensorChannelMetadata.getIntensities();
}

const char *FlapoProcessedData::getSensorLabel() const
{
    return m_sensorChannelMetadata.getSensorLabel().c_str();
}

const char *FlapoProcessedData::getOperationId() const
{
    return m_sensorChannelMetadata.getOperationLabel().c_str();
}

const char *FlapoProcessedData::getPatientId() const
{
    return m_sensorChannelMetadata.getPatientName().c_str();
}

const FlapoSensorChannelMetadata &FlapoProcessedData::getMetadata() const
{
    return m_sensorChannelMetadata;
}

void FlapoProcessedData::setStart(std::chrono::steady_clock::time_point start)
{
    this->start = start;
}

void FlapoProcessedData::setEnd(std::chrono::steady_clock::time_point end)
{
    this->end = end;
}
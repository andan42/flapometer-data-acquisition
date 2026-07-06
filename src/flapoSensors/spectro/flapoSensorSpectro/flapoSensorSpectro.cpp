#include "flapoSensorSpectro.h"

#include <iostream>

FlapoSensorSpectro::FlapoSensorSpectro(const std::string &sensorLabel, const FlapoOperationMetadata operationMetadata)
:
FlapoSensor(sensorLabel, operationMetadata, sensorTypeMetadata),
m_sensorChannelMetadata("", this->getSensorMetadata())
{
}

FlapoSensorSpectro::~FlapoSensorSpectro()
{
}

FlapoRawData *FlapoSensorSpectro::poll()
{
    return this->poller->poll(this);
}

std::vector<std::shared_ptr<const FlapoProcessedData>> FlapoSensorSpectro::process(const FlapoRawData *rawData) const
{
    std::vector<std::shared_ptr<const FlapoProcessedData>> ret;
    ret.push_back(this->processor->process(rawData)); //
    //delete rawData; //todo maybe do it with if!=nullptr //this is not the place to delete it, the thread class does it
    return ret;
}

const FlapoSensorChannelMetadata FlapoSensorSpectro::getSensorChannelMetadata() const
{
    return m_sensorChannelMetadata;
}

void FlapoSensorSpectro::setSensorMetadataIntensities(const std::vector<int> intensities)
{
    m_sensorChannelMetadata.setIntensities(intensities);    
}

const FlapoSensorTypeMetadata FlapoSensorSpectro::sensorTypeMetadata = FlapoSensorTypeMetadata("spectro");
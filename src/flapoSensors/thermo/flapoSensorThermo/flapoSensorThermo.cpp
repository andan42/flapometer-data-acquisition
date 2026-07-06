#include "flapoSensorThermo.h"

#include <iostream>
FlapoSensorThermo::FlapoSensorThermo(const std::string& sensorLabel, const FlapoOperationMetadata& operationMetadata)
: FlapoSensor(sensorLabel, operationMetadata, sensorTypeMetadata),
m_sensorChannelMetadata({FlapoSensorChannelMetadata("_channel1", this->getSensorMetadata()),
                         FlapoSensorChannelMetadata("_channel2", this->getSensorMetadata()),
                         FlapoSensorChannelMetadata("_channel3", this->getSensorMetadata()),
                         FlapoSensorChannelMetadata("_channel4", this->getSensorMetadata())})
{
}

FlapoSensorThermo::~FlapoSensorThermo()
{
}

FlapoRawData *FlapoSensorThermo::poll()
{
    return this->poller->poll(this);
}

std::vector<std::shared_ptr<const FlapoProcessedData>> FlapoSensorThermo::process(const FlapoRawData *rawData) const
{
    std::vector<std::shared_ptr<const FlapoProcessedData>> ret;
    
    //todo implement processor
    ret = this->processor->process(rawData);
    
    //delete rawData; //todo maybe do it with if!=nullptr //this is not the place to delete it, the thread class does it
    return ret;
}

const FlapoSensorChannelMetadata FlapoSensorThermo::getSensorChannelMetadata(int i) const
{
    return m_sensorChannelMetadata[i];
}

void FlapoSensorThermo::setSensorMetadataIntensities(const std::vector<int> intensities)
{
}

const FlapoSensorTypeMetadata FlapoSensorThermo::sensorTypeMetadata = FlapoSensorTypeMetadata("thermo");
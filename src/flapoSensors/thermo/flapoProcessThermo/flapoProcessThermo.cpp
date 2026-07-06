#include "flapoProcessThermo.h"

#include <chrono>
#include <iostream>
#include <string.h>

#include "flapoData/flapoProcessedData/flapoProcessedDataThermo/flapoProcessedDataThermo.h"
#include "flapoData/flapoRawData/flapoRawDataThermo/flapoRawDataThermo.h"
#include "flapoSensors/thermo/flapoSensorThermo/flapoSensorThermo.h"


const std::vector<std::shared_ptr<const FlapoProcessedData>> FlapoProcessThermo::process(const FlapoRawData *raw)
{
    std::vector<std::shared_ptr<const FlapoProcessedData>> result;

    //typecast to thermo raw data
    const FlapoRawDataThermo *rawThermo = dynamic_cast<const FlapoRawDataThermo *>(raw);
    if (rawThermo == nullptr)
    {
        std::cerr << "FlapoProcessThermo::process: raw data is not of type FlapoRawDataThermo" << std::endl;
        return result;
    }

    //get FlapoThermoSensor object
    const FlapoSensorThermo *sensor = dynamic_cast<const FlapoSensorThermo *>(rawThermo->getPolledSensor());

    if(rawThermo->channels == -1) //this is all channel type (4 channels interleaved)
    {
        int measurementCount = rawThermo->dataCount / 4; //4 channels
        int *measurementArray = new int[measurementCount];
    
        //create 4 processed data objects
        for(int i = 0; i < 4; i++)
        {
            if(!m_channelIsEnabled[i])
                continue;
                
            for(int j = 0; j < measurementCount; j++)
            {
                measurementArray[j] = rawThermo->data[i + j * 4];
            }
            std::shared_ptr<FlapoProcessedDataThermo> processedData = std::make_shared<FlapoProcessedDataThermo>(
                rawThermo->getPreviousTimePolled(),
                rawThermo->getTimePolled(),
                sensor->getSensorChannelMetadata(i),
                i,
                measurementCount,
                measurementArray
            );

            result.push_back(processedData);
        }
        delete[] measurementArray;
    }


    return result;
}

void FlapoProcessThermo::disableChannel(int channel)
{
    m_channelIsEnabled[channel] = false;
}

void FlapoProcessThermo::enableChannel(int channel)
{
    m_channelIsEnabled[channel] = true;
}

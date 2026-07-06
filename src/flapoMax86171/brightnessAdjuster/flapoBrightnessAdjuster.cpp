#include "flapoBrightnessAdjuster.h"

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"
#include "flapoMax86171/configurator/setup/flapoMax86171Setup.h"

#include <set>

FlapoBrightnessAdjuster::FlapoBrightnessAdjuster()
{
}

void FlapoBrightnessAdjuster::adjustBrightness(const FlapoProcessedData *data)
{
    //dont forget the indexing issue: the first led has tag 1 rather than 0

    //maximum 19 bit int
    const int maxBrightness = 524287;
    //overflow tag (0xC _____ PPG EXP_OVF DATA)
    const int ppgExpOvf = 0x0C;

    //typecast data to data spectro
    const FlapoProcessedDataSpectro* dataSpectro = dynamic_cast<const FlapoProcessedDataSpectro*>(data);
    if(dataSpectro == nullptr)
    {
        //this is ok because the consumer throws all kinds of data at this
        return;
    }
   //pointer to the sensor
    FlapoSensorSpectro* sensor = dynamic_cast<FlapoSensorSpectro*>(dataSpectro->getMetadata().getSensorPtr());
    if(sensor == nullptr)
    {
        //std::cout << "Sensor is not of type FlapoSensorSpectro. This is an error.." << std::endl;
        //this is not ok because processed data spectro should always be generated from a spectro sensor
        return;
    }
    //pointer to the sensor setup
    FlapoMax86171Setup* setup = sensor->m_setup;

    if(setup == nullptr)
    {
        //std::cout << "Setup is null. This is an error.." << std::endl;
        //this is not ok because the setup should always be set
        return;
    }

    //check for overflow
    std::set<int> overflowChannels;
    for(int i = 0; i < dataSpectro->getChannelCount(); i++)
    {
        for(int j = 0; j < dataSpectro->getSampleCount(); j++)
        {
            if(dataSpectro->getData()[i][j].getTag() == ppgExpOvf)
            {
                //data->getChannels()[i]
                overflowChannels.insert(dataSpectro->getChannels()[i].measurementNr);
                break;
            }
        }
    }

    //adjust brightness for each thing in the set
    if(overflowChannels.size() > 0)
    {    
        setup->pauseChip();
        for(auto it = overflowChannels.begin(); it != overflowChannels.end(); it++)
        {
            //get the led number
            int measurementNr = *it;

            //get the intensity
            int intensity = setup->getIntensity(measurementNr - 1); //subtract 1 because the first led has tag 1 but the first index is 0
        
            //adjust the intensity
            intensity = intensity / 2;

            //set the intensity
            setup->updateIntensity(measurementNr - 1, intensity);
        }
        setup->clearChip(); //only update intensities after there is no data on chip memory. this avoids having data blocs with valid data but misatched intensity metadata
        sensor->setSensorMetadataIntensities(setup->getIntensities());
        setup->resumeChip();
    }
}

#include "flapoAdjusterConsumer.h"

#include "flapoMax86171/brightnessAdjuster/flapoBrightnessAdjuster.h"

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"


FlapoAdjusterConsumer::FlapoAdjusterConsumer(FlapoBrightnessAdjuster *brightnessAdjuster)
    : m_brightnessAdjuster(brightnessAdjuster), deleteBrightnessAdjuster(false)
{
}

FlapoAdjusterConsumer::FlapoAdjusterConsumer()
    : m_brightnessAdjuster(new FlapoBrightnessAdjuster()), deleteBrightnessAdjuster(true)
{
}

FlapoAdjusterConsumer::~FlapoAdjusterConsumer()
{
    if (deleteBrightnessAdjuster)
    {
        delete m_brightnessAdjuster;
    }
}

void FlapoAdjusterConsumer::consume(std::shared_ptr<const FlapoProcessedData> consumedData)
{

    //std cout for debug
    //std::cout << "FlapoAdjusterConsumer::consume() adjusting brightness" << std::endl;

    m_brightnessAdjuster->adjustBrightness(consumedData.get());
}

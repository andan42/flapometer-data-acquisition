#pragma once
#include "flapoConsumer/flapoConsumer.h"

class FlapoBrightnessAdjuster;

class FlapoAdjusterConsumer : public FlapoConsumer
{
private:
    FlapoBrightnessAdjuster *m_brightnessAdjuster;
    bool deleteBrightnessAdjuster;
public:
    FlapoAdjusterConsumer(FlapoBrightnessAdjuster *brightnessAdjuster);
    FlapoAdjusterConsumer();
    ~FlapoAdjusterConsumer();
    void consume(std::shared_ptr<const FlapoProcessedData> consumedData) override;

};
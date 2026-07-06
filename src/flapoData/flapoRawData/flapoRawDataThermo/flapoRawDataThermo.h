#pragma once
#include "flapoRawData/flapoRawData.h"

class FlapoRawDataThermo : public FlapoRawData
{
public:
    FlapoRawDataThermo(FlapoSensor *polledSensor);
    ~FlapoRawDataThermo() override;
    int channels; //0 through 3 for single channel mode, -1 for all channels
};
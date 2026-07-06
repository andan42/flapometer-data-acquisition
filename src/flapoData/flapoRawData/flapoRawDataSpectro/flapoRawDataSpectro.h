#pragma once
#include "flapoRawData/flapoRawData.h"

class FlapoRawDataSpectro : public FlapoRawData
{
public:
    FlapoRawDataSpectro(FlapoSensor *polledSensor);
    ~FlapoRawDataSpectro() override;
    int overflow;
};
#pragma once
#include "flapoRawData/flapoRawData.h"
class FlapoDataPoll {
public:
    virtual FlapoRawData* poll(FlapoSensor *polledSensor) = 0;
    virtual ~FlapoDataPoll() = default;
};
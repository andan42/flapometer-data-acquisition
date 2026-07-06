#pragma once

#include "flapoData/flapoRawData/flapoRawData.h"
#include "flapoData/flapoProcessedData/flapoProcessedData.h"

#include <vector>
#include <memory>


class FlapoProcessThermo
{
private:
    bool m_channelIsEnabled[4] = {false, false, false, false};
public:
    const std::vector<std::shared_ptr<const FlapoProcessedData>> process(const FlapoRawData *raw);
    void enableChannel(int channel);
    void disableChannel(int channel);
};


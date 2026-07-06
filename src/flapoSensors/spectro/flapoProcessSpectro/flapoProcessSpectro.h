#pragma once

#include "flapoData/flapoRawData/flapoRawData.h"
#include "flapoData/flapoProcessedData/flapoProcessedData.h"

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoMaxSample/flapoMaxSample.h"
#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoSpectroChannel/flapoSpectroChannel.h"

#include <vector>
#include <memory>


class FlapoProcessSpectro
{
private:

    //internal subprocessing
    int guessCurrentChannelIndex(const FlapoRawData *raw, int startIndex);
    void internalProcess(const FlapoRawData *raw, FlapoMaxSample ** dataOut, int * dataOutLengths);
public:
    //config/setting related:
    std::vector<FlapoSpectroChannel> channelsInOrder;
    int nrPhotodiodes = -1; //needet for guessing index. if 2 photodiodes present it needs to find 2 data in sequence to match
    int nrMeasurements = -1; //might not be needed
    bool putStartAndEndAlignmentData = true; //if set to true 
    int additionalSafetyMarginPerChannel = 0; //set to value, zero is disabled
    //info related to current state:
    int currentChannelIndex = -1; //this indexes the channelsInOrder[] vector


    //external process call:
    const std::shared_ptr<FlapoProcessedData> process(const FlapoRawData *raw);
};

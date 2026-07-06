#pragma once
#include "flapoProcessedData.h"

#include <chrono>

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoMaxSample/flapoMaxSample.h"
#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoSpectroChannel/flapoSpectroChannel.h"

class FlapoProcessedDataSpectro : public FlapoProcessedData
{
private:
    int channelCount; //0 to 18
    FlapoSpectroChannel* channels;
    int* dataLengths;
    FlapoMaxSample** data;
public:
    FlapoProcessedDataSpectro(
        std::chrono::steady_clock::time_point start,
        std::chrono::steady_clock::time_point end,
        const FlapoSensorChannelMetadata sensorChannelMetadata,
        int channelCount,
        FlapoSpectroChannel* channels, //dont forget to pass by copy, fix efficiency later
        int* dataLengths, //dont forget to pass by copy, fix efficiency later
        FlapoMaxSample** data //dont forget to pass by copy, fix efficiency later
        );
    ~FlapoProcessedDataSpectro();

    int getChannelCount() const override;
    FlapoSpectroChannel* getChannels() const;
    int* getDataLengths() const;
    FlapoMaxSample** getData() const;

    bool operator==(const FlapoProcessedDataSpectro &other) const;

    bool isEqual(const FlapoProcessedData &other) const override;

//    std::string toJson() const override;
//    std::string toStr() const override;

    //data for saving as nist sphere -- these are all overrides of the methods the nist sphere class requires to create a nist sphere from data
    //header
    const char* getSensorType() const override;
    int getSampleCount() const override;
    int getSampleRate() const override;
    int getSampleNBytes() const override;
    const char* getSampleByteFormat() const override; //little endian: 10; big endian: 01
    int getSampleSigBits() const override;
    //data
    int getBytesSize() const override;
    void getBytes(char* bytes) const override;
    //merging data
    bool canMerge(const FlapoProcessedData& other) const override;
    std::shared_ptr<const FlapoProcessedData> merge(const FlapoProcessedData& other) const override;
};

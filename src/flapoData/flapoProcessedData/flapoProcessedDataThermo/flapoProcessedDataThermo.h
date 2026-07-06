#pragma once
#include "flapoProcessedData/flapoProcessedData.h"

#include <chrono>

class FlapoProcessedDataThermo : public FlapoProcessedData
{
private:
    int m_channelNumber; //0 to 3, only one channel. channel in thermo is the same as sensor in spectro
    int m_measurementCount;
    int* m_measurementArray;
public:
    FlapoProcessedDataThermo(
        std::chrono::steady_clock::time_point start,
        std::chrono::steady_clock::time_point end,
        const FlapoSensorChannelMetadata sensorChannelMetadata,
        int channelNumber,
        int measurementCount,
        int* measurementArray
        );
    ~FlapoProcessedDataThermo();

    int getChannelCount() const override; //always 1
    int getChannelNumber() const;

    bool operator==(const FlapoProcessedDataThermo &other) const;

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

#pragma once
#include <chrono>
#include <string>
#include <memory>

#include "flapoMetadata/flapoSensorChannelMetadata/flapoSensorChannelMetadata.h"

/*abstract*/ class FlapoProcessedData
{
private:
    //label
    //const char * const m_sensorLabel;
    const FlapoSensorChannelMetadata m_sensorChannelMetadata;
    //start time
    std::chrono::steady_clock::time_point start; //todo make const
    //end time
    std::chrono::steady_clock::time_point end; //todo make const
public:
    //to json
    //virtual std::string toJson() = 0;
    FlapoProcessedData(std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, const FlapoSensorChannelMetadata sensorChannelMetadata);
    virtual ~FlapoProcessedData();

    std::chrono::steady_clock::time_point getStart() const;
    std::chrono::steady_clock::time_point getEnd() const;
    const std::vector<int> getIntensities() const;

    virtual bool isEqual(const FlapoProcessedData &other) const = 0;

    //virtual std::string toJson() const = 0;
    //virtual std::string toStr() const = 0;

    //data for saving as nist sphere
    //header
    virtual const char* getSensorType() const = 0;
    virtual int getChannelCount() const = 0;
    virtual int getSampleCount() const = 0;
    virtual int getSampleRate() const = 0;
    virtual int getSampleNBytes() const = 0;
    virtual const char* getSampleByteFormat() const = 0;
    virtual int getSampleSigBits() const = 0;
    //sensor label
    const char* getSensorLabel() const;
    //operation id
    const char* getOperationId() const;
    //patient id
    const char* getPatientId() const;

    //data
    virtual int getBytesSize() const = 0;
    virtual void getBytes(char* bytes) const = 0;
    
    //merging data
    virtual bool canMerge(const FlapoProcessedData& other) const = 0;
    virtual std::shared_ptr<const FlapoProcessedData> merge(const FlapoProcessedData& other) const = 0;

    //get metadata
    const FlapoSensorChannelMetadata& getMetadata() const;

    //setters for privates
protected:
    void setStart(std::chrono::steady_clock::time_point start);
    void setEnd(std::chrono::steady_clock::time_point end);
};
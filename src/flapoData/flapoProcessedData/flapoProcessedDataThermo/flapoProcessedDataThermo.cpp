#include "flapoProcessedDataThermo.h"
#include <typeinfo>

#include <cstring>

#include "flapoMetadata/flapoSensorMetadata/flapoSensorMetadata.h"

FlapoProcessedDataThermo::FlapoProcessedDataThermo(
    std::chrono::steady_clock::time_point start,
    std::chrono::steady_clock::time_point end,
    const FlapoSensorChannelMetadata sensorChannelMetadata,
    int channelNumber,
    int measurementCount,
    int *measurementArray
) : FlapoProcessedData(
    start,
    end,
    sensorChannelMetadata)
{
    this->m_channelNumber = channelNumber;
    this->m_measurementCount = measurementCount;
    this->m_measurementArray = new int[measurementCount];
    for (int i = 0; i < measurementCount; i++)
    {
        this->m_measurementArray[i] = measurementArray[i];
    }

    //std::strcpy((char *)this->getSensorLabel(), sensorLabel);
}

FlapoProcessedDataThermo::~FlapoProcessedDataThermo()
{
    delete[] m_measurementArray;
}

int FlapoProcessedDataThermo::getChannelCount() const
{
    return 1;
}

const char *FlapoProcessedDataThermo::getSensorType() const
{
    return "thermo";
}

int FlapoProcessedDataThermo::getSampleCount() const
{
    return m_measurementCount;
}

int FlapoProcessedDataThermo::getSampleRate() const
{
    int durationSecs = std::chrono::duration_cast<std::chrono::seconds>(this->getEnd() - this->getStart()).count();
    if (durationSecs == 0)
    {
        return 0;
    }
    float sampleRatePerSec = (float)getSampleCount() / (float)durationSecs;
    //round
    int ret = (int)(sampleRatePerSec + 0.5f);
    return ret;
}

int FlapoProcessedDataThermo::getSampleNBytes() const
{
    return 1; //1 byte of data (ADC is 8 bit)
}

const char *FlapoProcessedDataThermo::getSampleByteFormat() const
{
    return "10"; //endinaness is irrelevant for a single byte but w/e
}

int FlapoProcessedDataThermo::getSampleSigBits() const
{
    return 8; //8 bit ADC
}

int FlapoProcessedDataThermo::getBytesSize() const
{
    //* this->getSampleCount() * this->getSampleNBytes() * this->getChannelCount()
    return this->getSampleCount(); 
    //*1*1 (channel count and nr bytes are both 1)
}

void FlapoProcessedDataThermo::getBytes(char *bytes) const
{
    int sampleCount = this->getSampleCount();
    for (int i = 0; i < sampleCount; i++)
    {
        bytes[i] = (uint8_t)m_measurementArray[i];
    }
}

bool FlapoProcessedDataThermo::canMerge(const FlapoProcessedData &other) const
{
    if (typeid(other) != typeid(FlapoProcessedDataThermo))
    {
        return false;
    }
    //cast 
    FlapoProcessedDataThermo &otherThermo = (FlapoProcessedDataThermo &)other;

    //TODO check timestamps

    //metadata must be the same
    if (this->getMetadata() != otherThermo.getMetadata())
    {
        return false;
    }

    //channel number must be the same
    if (this->getChannelNumber() != otherThermo.getChannelNumber())
    {
        return false;
    }

    return true;
}

std::shared_ptr<const FlapoProcessedData> FlapoProcessedDataThermo::merge(const FlapoProcessedData &other) const
{
    if (!canMerge(other))
    {
        return nullptr;
    }
    //cast
    const FlapoProcessedDataThermo& otherThermo = (const FlapoProcessedDataThermo&)other;


    //merge data
    std::chrono::steady_clock::time_point resultStart = this->getStart(); //start of first data blob
    std::chrono::steady_clock::time_point resultEnd = otherThermo.getEnd(); //duration of both data blobs
    const FlapoSensorChannelMetadata& resultMetadata = this->getMetadata(); //metadata is the same for both data blobs
    int resultChannelNumber = this->getChannelNumber(); //channel number is the same for both data blobs
    int resultMeasurementCount = this->getSampleCount() + otherThermo.getSampleCount(); //sum of the number of measurements in both data blobs
    int *resultMeasurementArray = new int[resultMeasurementCount]; //allocate memory for the merged data blob
    for (int i = 0; i < this->getSampleCount(); i++)
    {
        resultMeasurementArray[i] = this->m_measurementArray[i]; //copy the first data blob's data
    }
    for (int i = 0; i < otherThermo.getSampleCount(); i++)
    {
        resultMeasurementArray[this->getSampleCount() + i] = otherThermo.m_measurementArray[i]; //copy the second data blob's data
    }

    //create the merged data blob
    std::shared_ptr<FlapoProcessedDataThermo> result = std::make_shared<FlapoProcessedDataThermo>(
        resultStart,
        resultEnd,
        resultMetadata,
        resultChannelNumber,
        resultMeasurementCount,
        resultMeasurementArray
    );

    //delete the temporary data
    delete[] resultMeasurementArray;

    return result;
}

int FlapoProcessedDataThermo::getChannelNumber() const
{
    return m_channelNumber;
}

bool FlapoProcessedDataThermo::operator==(const FlapoProcessedDataThermo &other) const
{
    if (this->getChannelNumber() != other.getChannelNumber())
    {
        return false;
    }
    if (this->getSampleCount() != other.getSampleCount())
    {
        return false;
    }
    for (int i = 0; i < this->getSampleCount(); i++)
    {
        if (this->m_measurementArray[i] != other.m_measurementArray[i])
        {
            return false;
        }
    }
    return true;
}

bool FlapoProcessedDataThermo::isEqual(const FlapoProcessedData &other) const
{
    if (typeid(other) != typeid(FlapoProcessedDataThermo))
    {
        return false;
    }
    return *this == (FlapoProcessedDataThermo &)other;
}

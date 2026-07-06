#include "flapoProcessedDataSpectro.h"
#include <typeinfo>

FlapoProcessedDataSpectro::FlapoProcessedDataSpectro(
        std::chrono::steady_clock::time_point start,
        std::chrono::steady_clock::time_point end,
        const FlapoSensorChannelMetadata sensorChannelMetadata,
        int channelCount,
        FlapoSpectroChannel* channels, //dont forget to pass by copy, fix efficiency later
        int* dataLengths, //dont forget to pass by copy, fix efficiency later
        FlapoMaxSample** data //dont forget to pass by copy, fix efficiency later)
) : FlapoProcessedData(start, end, sensorChannelMetadata)
{
    this->channelCount = channelCount;
    this->channels = new FlapoSpectroChannel[channelCount];
    for (int i = 0; i < channelCount; i++)
    {
        this->channels[i] = channels[i];
    }
    this->dataLengths = new int[channelCount];
    for (int i = 0; i < channelCount; i++)
    {
        this->dataLengths[i] = dataLengths[i];
    }
    this->data = new FlapoMaxSample*[channelCount];
    for (int i = 0; i < channelCount; i++)
    {
        this->data[i] = new FlapoMaxSample[dataLengths[i]];
        for (int j = 0; j < dataLengths[i]; j++)
        {
            this->data[i][j] = data[i][j];
        }
    }
}

FlapoProcessedDataSpectro::~FlapoProcessedDataSpectro()
{
    for (int i = 0; i < channelCount; i++)
    {
        delete[] data[i];
    }
    delete[] data;
    delete[] dataLengths;
    delete[] channels;
}

int FlapoProcessedDataSpectro::getChannelCount() const
{
    return channelCount;
}

const char *FlapoProcessedDataSpectro::getSensorType() const
{
    return "spectro";
}

int FlapoProcessedDataSpectro::getSampleCount() const
{
    //it is possible for each channel to have dif length (if padding data by zero isnt done)
    //give the biggest channel length
    int sampleCount = 0;
    for (int i = 0; i < channelCount; i++)
    {

        if (dataLengths[i] > sampleCount) //longest channel is taken into account
        {
            sampleCount = dataLengths[i];
        }
    }
    return sampleCount;
}

int FlapoProcessedDataSpectro::getSampleRate() const
{
    int durationMillis = std::chrono::duration_cast<std::chrono::milliseconds>(this->getEnd() - this->getStart()).count();
    if (durationMillis == 0) //avoid divide by 0
    {
        return 0;
    }
    float sampleRatePerSec = (float)getSampleCount() / (float)durationMillis * 1000.0f;
    //round
    int ret = (int)(sampleRatePerSec + 0.5f);
    return ret;
}

int FlapoProcessedDataSpectro::getSampleNBytes() const
{
    return 4;
}

const char *FlapoProcessedDataSpectro::getSampleByteFormat() const
{
    return "10";
}

int FlapoProcessedDataSpectro::getSampleSigBits() const
{
    return 20;
}

int FlapoProcessedDataSpectro::getBytesSize() const
{
    int bytesSize = this->getChannelCount() * this->getSampleCount() * this->getSampleNBytes(); 
    //channels * samples * bytes per sample   ---   this is the no of bytes needed to store all the data
    return bytesSize;
}

void FlapoProcessedDataSpectro::getBytes(char *bytes) const
{
    int sampleCount = this->getSampleCount();
    int channelCount = this->getChannelCount();
    int sampleNBytes = this->getSampleNBytes();
    //reorder sample
    //theyre stored by channel (ch0 s0 s1 s2 s3 ... ch1 s0 s1 s2 s3 ...)
    //order by sample time (s0 ch0 ch1 ch2 ch3 ... s1 ch0 ch1 ch2 ch3 ...)
    for (int i = 0; i < sampleCount; i++)
    {
        for (int j = 0; j < channelCount; j++)
        {
            //check if sample exists   ---   this is because not all channels are filled "to the end"
            //channels that have less data get filled with 0 at the end
            if (i >= this->dataLengths[j])
            {
                //if not, fill with 0
                for (int k = 0; k < sampleNBytes; k++)
                {
                    bytes[(i * channelCount + j) * sampleNBytes + k] = 0;
                }
            }
            else
            {
                //if yes, fill with sample
                FlapoMaxSample sample = this->data[j][i];
                int sampleInt = sample.getBitsRaw();
                //convert to little endian bytes
                for (int k = 0; k < sampleNBytes; k++)
                {
                    bytes[(i * channelCount + j) * sampleNBytes + k] = (char)(sampleInt & 0xFF);
                    sampleInt >>= 8;
                }
            }
        }
    }
}

bool FlapoProcessedDataSpectro::canMerge(const FlapoProcessedData &other) const
{
    if (typeid(other) != typeid(FlapoProcessedDataSpectro))
    {
        return false;
    }
    //cast to FlapoProcessedDataSpectro to compare relevant data
    const FlapoProcessedDataSpectro& otherSpectro = (const FlapoProcessedDataSpectro&)other;

    //TODO check time stamp matching, this is annoying cos it requires some margin of error

    //check if metadata matches
    if (this->getMetadata() != otherSpectro.getMetadata())
    {
        return false;
    }

    //check if channel count matches
    if (this->getChannelCount() != otherSpectro.getChannelCount())
    {
        return false;
    }

    //check if channel "metadata" matches
    for (int i = 0; i < this->getChannelCount(); i++)
    {
        if (this->getChannels()[i] != otherSpectro.getChannels()[i])
        {
            return false;
        }
    }

    //data doesnt have to match cos it will be merged 
    return true;
}

std::shared_ptr<const FlapoProcessedData> FlapoProcessedDataSpectro::merge(const FlapoProcessedData &other) const
{   
    //TODO check time stamp matching, this is annoying cos it requires some margin of error
    //if they are in the opposite order, swap them
    /*if (this->getStart() > other.getEnd())
    {
        return ((const FlapoProcessedDataSpectro&)other).merge(*this);
    }*/

    if (!this->canMerge(other))
    {
        return nullptr;
    }
    //cast to FlapoProcessedDataSpectro to merge relevant data
    const FlapoProcessedDataSpectro& otherSpectro = (const FlapoProcessedDataSpectro&)other;


    //merge data
    std::chrono::steady_clock::time_point resultStart = this->getStart(); //start of first data blob
    std::chrono::steady_clock::time_point resultEnd = otherSpectro.getEnd(); //end of second data blob
    const FlapoSensorChannelMetadata& resultMetadata = this->getMetadata(); //compared to be equal
    int resultChannelCount = this->getChannelCount(); //compared to be equal
    FlapoSpectroChannel* resultChannels = new FlapoSpectroChannel[resultChannelCount];
    for (int i = 0; i < resultChannelCount; i++)
    {
        resultChannels[i] = this->getChannels()[i]; //compared to be equal
    }
    int* resultDataLengths = new int[resultChannelCount];
    //check if padding data is present and removable
    //for it to be present and removable, for each channel, either the first blob ends in a dummy or the second blob starts with a dummy, BUT NOT BOTH
    //a dummy is a sample with tag == 15
    //note that if neither of the two blobs have padding data, the bool is false
    //if both blobs have padding data, the bool is also false
    bool paddingPresentAndRemovable = true;
    int accountForPadding = 1;
    {
        FlapoMaxSample lastSampleOfFirstBlob;
        FlapoMaxSample firstSampleOfSecondBlob;
        for (int i = 0; i < resultChannelCount; i++)
        {
            lastSampleOfFirstBlob = this->getData()[i][this->getDataLengths()[i] - 1];
            firstSampleOfSecondBlob = otherSpectro.getData()[i][0]; // valgrind sometiems doesnt like this

            if(     !(
                        (firstSampleOfSecondBlob.getTag() == 15)
                        ^ (lastSampleOfFirstBlob.getTag() == 15)
                    ))
            {
                paddingPresentAndRemovable = false;
                accountForPadding = 0;
                break;
            }
        }
    }


    for (int i = 0; i < resultChannelCount; i++)
    {
        resultDataLengths[i] = this->getDataLengths()[i] + otherSpectro.getDataLengths()[i] - accountForPadding;
    }
    FlapoMaxSample** resultData = new FlapoMaxSample*[resultChannelCount];
    for (int i = 0; i < resultChannelCount; i++)
    {
        resultData[i] = new FlapoMaxSample[resultDataLengths[i]];
        if(!paddingPresentAndRemovable)
        {
            for (int j = 0; j < this->getDataLengths()[i]; j++)
            {
                resultData[i][j] = this->getData()[i][j];
            }
            for (int j = 0; j < otherSpectro.getDataLengths()[i]; j++)
            {
                resultData[i][this->getDataLengths()[i] + j] = otherSpectro.getData()[i][j];
            }
        }
        else
        {
            for (int j = 0; j < this->getDataLengths()[i] - 1; j++)
            {
                resultData[i][j] = this->getData()[i][j];
            }
            for (int j = 1; j < otherSpectro.getDataLengths()[i]; j++)
            {
                resultData[i][this->getDataLengths()[i] + j - 1] = otherSpectro.getData()[i][j];
            }
            if(this->getData()[i][this->getDataLengths()[i] - 1].getTag() == 15)
            {
                resultData[i][this->getDataLengths()[i] - 1] = otherSpectro.getData()[i][0];
            }
            else
            {
                resultData[i][this->getDataLengths()[i] - 1] = this->getData()[i][this->getDataLengths()[i] - 1];
            }
        }
    }

    
    //set new data
    /*this->setStart(resultStart);
    this->setDuration(resultDuration);
    this->channelCount = resultChannelCount;
    this->channels = resultChannels;
    this->dataLengths = resultDataLengths;
    this->data = resultData;/**/

    //create result
    std::shared_ptr<FlapoProcessedData> result = std::make_shared<FlapoProcessedDataSpectro>(
        resultStart,
        resultEnd,
        resultMetadata,
        resultChannelCount,
        resultChannels,
        resultDataLengths,
        resultData
    );

    //delete temp data
    for (int i = 0; i < resultChannelCount; i++)
    {
        delete[] resultData[i];
    }
    delete[] resultData;
    delete[] resultDataLengths;
    delete[] resultChannels;

    return result;
}

FlapoSpectroChannel *FlapoProcessedDataSpectro::getChannels() const
{   
    return channels;
}

int *FlapoProcessedDataSpectro::getDataLengths() const
{
    return dataLengths;
}

FlapoMaxSample **FlapoProcessedDataSpectro::getData() const
{
    return data;
}

bool FlapoProcessedDataSpectro::operator==(const FlapoProcessedDataSpectro &other) const
{
    if (this->channelCount != other.channelCount)
    {
        return false;
    }
    for (int i = 0; i < channelCount; i++)
    {
        if (this->channels[i] != other.channels[i])
        {
            return false;
        }
    }
    for (int i = 0; i < channelCount; i++)
    {
        if (this->dataLengths[i] != other.dataLengths[i])
        {
            return false;
        }
    }
    for (int i = 0; i < channelCount; i++)
    {
        for (int j = 0; j < dataLengths[i]; j++)
        {
            if (this->data[i][j] != other.data[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

bool FlapoProcessedDataSpectro::isEqual(const FlapoProcessedData &other) const
{
    if (typeid(other) != typeid(FlapoProcessedDataSpectro))
    {
        return false;
    }
    return *this == (const FlapoProcessedDataSpectro&)other;
}

/*std::string FlapoProcessedDataSpectro::toJson() const 
{
    return std::string();
}

std::string FlapoProcessedDataSpectro::toStr() const 
{
    auto currentTime = std::chrono::steady_clock::now();
    // str = "FlapoProcessedDataSpectro at " + std::to_string((long long)this) + ":\n";
    std::string str = "start: " + std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(getStart().time_since_epoch() - currentTime.time_since_epoch()).count()) + "ms ago\n";
    str += "duration: " + std::to_string(getDuration().count()) + "\n";
    str += "channelCount: " + std::to_string(channelCount) + "\n";
    //loop through channels
    for (int i = 0; i < channelCount; i++)
    {
        str += "    channel #" + std::to_string(i);
        str + " for " + channels[i].toStr();
        str += " has dataLength: " + std::to_string(dataLengths[i]) + "\n";
        //loop through data
        for (int j = 0; j < dataLengths[i]; j++)
        {
            //str += "data " + std::to_string(j) + ":\n";
            str += "        data: " + data[i][j].toStr() + "\n";
        }
    }

    return str;
}*/

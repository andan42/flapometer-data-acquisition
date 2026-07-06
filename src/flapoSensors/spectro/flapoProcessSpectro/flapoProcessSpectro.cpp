#include "flapoProcessSpectro.h"

#include <chrono>
#include <iostream>

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"
#include "flapoData/flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

int FlapoProcessSpectro::guessCurrentChannelIndex(const FlapoRawData *raw, int startIndex)
{
    int index = startIndex;


    if(nrPhotodiodes == 1)
    {
        while(index < raw->dataCount)
        {
            FlapoMaxSample currentSample = raw->data[index];
            if((currentSample.getTag() <= 9) && (currentSample.getTag() >= 1))
            {
                //found an identifiable sample, lets calculate what inde we mustve been on at startIndex
                int advancedBy = (index - startIndex) % nrMeasurements /* * nrPhotodiodes=1 */;

                //find which chanel matches the tag of currentSample
                int identifiedTagIndex = -1;
                for (int i = 0; i < channelsInOrder.size(); i++)
                {
                    if(channelsInOrder[i].measurementNr == currentSample.getTag())
                    {
                        identifiedTagIndex = i;
                        break;
                    }
                }

                //calculate the index of the channel at startIndex
                int guessedChannelIndex = (identifiedTagIndex - advancedBy + nrMeasurements) % nrMeasurements;

                //return the index
                return guessedChannelIndex;
            }
            index++;

        }
    }
    else if(nrPhotodiodes == 2)
    {
        while(index < raw->dataCount - 1)
        {
            FlapoMaxSample currentSample = raw->data[index];

            int innerIndex = index + 1;
            int offsetOfInnerIndex = 1;
            while(innerIndex < raw->dataCount)
            {
                FlapoMaxSample nextMatchedSample = raw->data[innerIndex];
                if((currentSample.getTag() <= 9) && (currentSample.getTag() >= 1) && (nextMatchedSample.getTag() <= 9) && (nextMatchedSample.getTag() >= 1))
                {
                    //find which two correctly offset channels match the tags of currentSample and nextMatchedSample in that order
                    int identifiedTagIndex = -1;
                    for (int i = 0; i < channelsInOrder.size(); i++)
                    {
                        if((channelsInOrder[i].measurementNr == currentSample.getTag()) && (channelsInOrder[((i + offsetOfInnerIndex) % channelsInOrder.size())].measurementNr == nextMatchedSample.getTag())) 
                        {
                            identifiedTagIndex = i;
                            break;
                        }
                    }

                    if(identifiedTagIndex != -1)
                    {                   
                        //found an identifiable sample, lets calculate what inde we mustve been on at startIndex
                        int advancedBy = (index - startIndex) % (nrMeasurements * nrPhotodiodes);

                        //calculate the index of the channel at startIndex
                        int guessedChannelIndex = (identifiedTagIndex - advancedBy + nrMeasurements * nrPhotodiodes) % (nrMeasurements * nrPhotodiodes);

                        //return the index
                        return guessedChannelIndex;
                    }
                }
                innerIndex++;
                offsetOfInnerIndex++;
            }
            index++;
        }
    }

    if(currentChannelIndex != -1)//this is a failure backup case
    {
        return currentChannelIndex; //cannot find a new index to continue from, return currently used one. this prolly wont happen
    } 
    else
    {
        return 0; //assume it starts at first enabled measurement. this assumptions is somewhat reasonable when the chip powers on and instatnly does an overflow instead of providing a proper tag
    }
}



void FlapoProcessSpectro::internalProcess(const FlapoRawData *raw, FlapoMaxSample ** dataOut, int * dataOutLengths) 
{
    //this function does heavy lifting
    //time to open up the datasheet

    if(currentChannelIndex == -1)
    {
        currentChannelIndex = guessCurrentChannelIndex(raw, 0);
    }

    //allocate pointer memory
    //FlapoMaxSample ** dataOut;
    //int * dataOutLengths;
    int * dataOutStartIndices; //all zero if putStartAndEndAlignmentData is false

    dataOutStartIndices = new int[nrMeasurements * nrPhotodiodes];

    //allocate memory for dataOut  
    if(!putStartAndEndAlignmentData)
    {    
        int extendedSizeTotal = raw->dataCount + currentChannelIndex;
        int extendedSizePerChannel = extendedSizeTotal / (nrMeasurements * nrPhotodiodes) + additionalSafetyMarginPerChannel;
        for (int i = 0; i < nrMeasurements * nrPhotodiodes; i++)
        {
            dataOutLengths[i] = extendedSizePerChannel;
            dataOutStartIndices[i] = 0;
            if(i < currentChannelIndex)
            {
                dataOutLengths[i] -= 1;
            }
            if(i < (extendedSizeTotal % (nrMeasurements * nrPhotodiodes)) )
            {
                dataOutLengths[i] += 1;
            }
            dataOut[i] = new FlapoMaxSample[extendedSizePerChannel];
        }
    }
    else //if putStartAndEndAlignmentData is true
    {
        int extendedSizeTotal = raw->dataCount + currentChannelIndex;
        int extendedSizePerChannel = extendedSizeTotal / (nrMeasurements * nrPhotodiodes) + additionalSafetyMarginPerChannel;
        if (extendedSizeTotal % (nrMeasurements * nrPhotodiodes) != 0)
        {
            extendedSizePerChannel++;
        }

        for (int i = 0; i < nrMeasurements * nrPhotodiodes; i++)
        {
            dataOutLengths[i] = extendedSizePerChannel;
            dataOut[i] = new FlapoMaxSample[extendedSizePerChannel];
            if(i < currentChannelIndex)
            {
                dataOutStartIndices[i] = 1;
                dataOut[i][0] = FlapoMaxSample(-1, 0); //pad with dummy sample
            }
            else
            {
                dataOutStartIndices[i] = 0;
            }
        }
    }

    //int currentChannel = currentChannelIndex; not sure why this necessary
    int indexInRaw = 0;

    //loop raw data and copy to correct channel in dataOut
    while(indexInRaw < raw->dataCount)
    {
        //VERIFY sample tag
        FlapoMaxSample sample = raw->data[indexInRaw];
        if(sample.getTag() == channelsInOrder[currentChannelIndex].measurementNr)
        {
            //normal operation, no action required
        }
        else if((sample.getTag() < 1) || (sample.getTag() > 9))
        {
            //check which flag it is, otherwise normal operation.
            //TODO implement later
            //could be overexposure, alc overflow, picket fence event, or invalid data (read empty chip)

            //operation is still normal, but extra action might be needed to adjust chip settings
        }
        else if((sample.getTag() >= 1 ) && (sample.getTag() <= 9)) 
        {
            //this means measurement happened out of sequence.
            //find new channel index using guessCurrentChannelIndex() and attempt to continue
            //this should not happen unless there is a data overflow in the chips buffer causing loss of data
            //or if the sensor is reset mid measurement
            currentChannelIndex = guessCurrentChannelIndex(raw, indexInRaw);
        } //else doesnt exist

        //copy data
        if(dataOutStartIndices[currentChannelIndex] >= dataOutLengths[currentChannelIndex])
        {
            //even when allocating exactly the correct number of samples for each channel this can happen because:
            //if the chip is interrupted mid measurement it will start over with measurement 1 (channel 0 or 0 and 1)) overflowing that buffer
            //allocating a few extra samples per channel fixes this for a limited number of such issues at the cost of a few extra samples per channel
        }
        else
        {
            dataOut[currentChannelIndex][dataOutStartIndices[currentChannelIndex]] = sample;
        }

        //increment channel index
        dataOutStartIndices[currentChannelIndex] ++;
        
        //increment index
        indexInRaw++;
        //increment channel
        currentChannelIndex++;
        if(currentChannelIndex >= nrMeasurements * nrPhotodiodes)
        {
            currentChannelIndex = 0;
        }
    }

    //pad end, probably only necessary if putStartAndEndAlignmentData is true
    for (int i = 0; i < nrMeasurements * nrPhotodiodes; i++)
    {
        while(dataOutStartIndices[i] < dataOutLengths[i])
        {
            dataOut[i][dataOutStartIndices[i]] = FlapoMaxSample(-1, 0); //pad with dummy sample
            dataOutStartIndices[i]++;
        }
    }

    delete[] dataOutStartIndices;

    return;
}

const std::shared_ptr<FlapoProcessedData> FlapoProcessSpectro::process(const FlapoRawData *rawVirtual)
{
    const FlapoRawDataSpectro *rawSpectro = dynamic_cast<const FlapoRawDataSpectro*>(rawVirtual);
    if(rawSpectro == nullptr)
    {
        std::cout << "FlapoProcessSpectro::process() received a non-spectro raw data object" << std::endl;
        std::cout << "this is a bug" << std::endl;
        //todo make this throw, and replace all references to rawVirtual with rawSpectro
    }
    else
    {
        //spectro specific fields are available
        //rawSpectro->overflow;
    }
    //this function calls internal process and possibly performs some checks

    FlapoMaxSample ** dataOut;
    int * dataOutLengths;
    dataOut = new FlapoMaxSample*[nrMeasurements * nrPhotodiodes]; //length should be equal to channelsInOrder.size()
    dataOutLengths = new int[nrMeasurements * nrPhotodiodes];

    //call internal process
    internalProcess(rawVirtual, dataOut, dataOutLengths);

    //get FlapoSpectroSensor object
    const FlapoSensorSpectro *sensor = dynamic_cast<const FlapoSensorSpectro*>(rawVirtual->getPolledSensor());

    std::shared_ptr<FlapoProcessedData> processed(new FlapoProcessedDataSpectro(
        rawVirtual->getPreviousTimePolled(),
        rawVirtual->getTimePolled(),
        sensor->getSensorChannelMetadata(),
        nrMeasurements * nrPhotodiodes,
        channelsInOrder.data(),
        dataOutLengths,
        dataOut
        ));

    // delete dataOut
    for (int i = 0; i < nrMeasurements * nrPhotodiodes; i++)
    {
        delete[] dataOut[i];
    }

    delete[] dataOut;
    delete[] dataOutLengths;

    return processed;
}
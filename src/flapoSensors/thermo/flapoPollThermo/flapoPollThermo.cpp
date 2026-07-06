#include "flapoPollThermo.h"

#include "i2c/DeviceI2C.h"
#include "flapoPcf8591/chip/flapoPcf8591.h"


#include "flapoRawData/flapoRawDataThermo/flapoRawDataThermo.h"

#include <thread>
#include <iostream>

FlapoDataPollThermo::FlapoDataPollThermo(FlapoPcf8591 *flapoWrapperThermo)
{
    m_flapoWrapperThermo = flapoWrapperThermo;
}

FlapoDataPollThermo::~FlapoDataPollThermo()
{
}
FlapoRawData* FlapoDataPollThermo::poll(FlapoSensor *polledSensor)
{
    //operating in all (4) ADC channel mode aka read with auto increment
    //take 5 samples from each ADC channel
    static const int channel_count = 4;
    static const int sample_count_per_channel = 5;
    int dataCount = channel_count * sample_count_per_channel;
    int * data = new int[dataCount];

    FlapoRawDataThermo *flapoRawDataThermo;
    

    m_flapoWrapperThermo->writeControl(
        true, //required for operating pcf8591 with internal oscillator
        0b00, //single ended mode on all channels/inputs
        true, //auto increment mode
        3); //first channel to read from is 3 (last channel)

    //sleep 10 ms see if it fixes
    m_flapoWrapperThermo->readDummy(); //first measurement is the one that was ADC-ed during the last read operation performed. not useful.
    
    //NOTE THAT FIRST MEASUREMENT COMING UP NOW SHOULD BE GOOD BUT NEVER IS FOR SOME FUCKING GOD DAMN REASON
    //THE CODE THAT FOLLOWS ONLY HAS THE PURPOSE OF FIXING THIS MISTEROUS ISSUE
    //TODO FIX
    m_flapoWrapperThermo->readDummy();
    //THIS CODE IS A BANDAID. NOTHING I CAN DO
    //NOTE THAT I AM STARTING READING FROM CHANNEL 3 (LAST CHANNEL) SO THAT IT LOOPS OVER TO FIRST AND DISCARDS THE SHIT MEASUREMENT

    for(int i = 0; i < dataCount; i++)
    {
        data[i] = m_flapoWrapperThermo->read();
    }
    
    //collect results into return object
    flapoRawDataThermo = new FlapoRawDataThermo(polledSensor);
    flapoRawDataThermo->data = data;
    flapoRawDataThermo->dataCount = dataCount;
    flapoRawDataThermo->channels = -1; //all channel mode specified

    return flapoRawDataThermo;
}
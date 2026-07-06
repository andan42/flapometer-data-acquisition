#pragma once

#include "flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"
#include "flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"
#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"

#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

#include <iostream>

void testDataProcessSpectroDouble()
{
    //TODO check if tests are actually returning the correct values... i eyeballed this
    FlapoProcessSpectro *process = new FlapoProcessSpectro();
    //set up process
    process->nrPhotodiodes = 2;
    process->nrMeasurements = 4;
    process->putStartAndEndAlignmentData = false; //can test with both SHOULD TEST WITH BOTH
    process->additionalSafetyMarginPerChannel = 2; //fuck around and find out
    //process->currentChannelIndex = 0;
    process->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(3, 1),
        FlapoSpectroChannel(4, 0),
        FlapoSpectroChannel(4, 1)
    };

    FlapoSensorSpectro *sensor = new FlapoSensorSpectro();
    sensor->processor = process;

    FlapoRawDataSpectro *raw = new FlapoRawDataSpectro(sensor); //give this args
    std::shared_ptr<FlapoProcessedData> processed; //this is returned by process

    raw->dataCount = 32;
    raw->data = new int[raw->dataCount];
    
    FlapoMaxSample dummySamples[] = {
        FlapoMaxSample(2, 121),
        FlapoMaxSample(3, 130),
        FlapoMaxSample(3, 131),
        FlapoMaxSample(10, 1400), //140
        FlapoMaxSample(4, 141),

        FlapoMaxSample(1, 210),
        FlapoMaxSample(1, 211),
        FlapoMaxSample(2, 220),
        FlapoMaxSample(2, 221),
        /*FlapoMaxSample(3, 230),
        FlapoMaxSample(3, 231),
        FlapoMaxSample(4, 240),
        FlapoMaxSample(4, 241),*/

        FlapoMaxSample(1, 310),
        FlapoMaxSample(1, 311),
        FlapoMaxSample(10, 3200), //320
        FlapoMaxSample(10, 3210), //321
        FlapoMaxSample(3, 330),
        FlapoMaxSample(3, 331),
        FlapoMaxSample(4, 340),
        FlapoMaxSample(4, 341),

        FlapoMaxSample(1, 410),
        FlapoMaxSample(1, 411),
        FlapoMaxSample(2, 420),
        FlapoMaxSample(2, 421),
        FlapoMaxSample(10, 4300), //430
        FlapoMaxSample(3, 431),
        FlapoMaxSample(4, 440),
        FlapoMaxSample(4, 441),
        
        FlapoMaxSample(1, 110),
        FlapoMaxSample(1, 111),
        FlapoMaxSample(2, 120),
        
        FlapoMaxSample(1, 610),
        FlapoMaxSample(1, 611),
        FlapoMaxSample(2, 620),
        FlapoMaxSample(2, 621),


    };

    for (int i = 0; i < raw->dataCount; i++)
    {
        raw->data[i] = dummySamples[i].getBitsRaw();
    }


    processed = process->process(raw);

    std::cout << processed->toStr() << std::endl;


    delete raw;
    delete process;
    delete sensor;
    //delete processed; ///now smart
}

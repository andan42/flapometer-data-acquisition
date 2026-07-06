#pragma once

#include "flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"
#include "flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"
#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"

#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

#include <iostream>

void testDataProcessSpectroSimple()
{

    FlapoProcessSpectro *process = new FlapoProcessSpectro();
    //set up process
    process->nrPhotodiodes = 1;
    process->nrMeasurements = 4;
    process->putStartAndEndAlignmentData = false;
    //process->currentChannelIndex = 0;
    process->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(4, 0)
    };

    FlapoSensorSpectro *sensor = new FlapoSensorSpectro();
    sensor->processor = process;

    FlapoRawDataSpectro *raw = new FlapoRawDataSpectro(sensor); //give this args
    std::shared_ptr<FlapoProcessedData> processed; //this is returned by process

    raw->dataCount = 21;
    raw->data = new int[raw->dataCount];
    
    FlapoMaxSample dummySamples[] = {
        FlapoMaxSample(2, 2),
        FlapoMaxSample(3, 3),
        FlapoMaxSample(4, 4),
        FlapoMaxSample(1, 5),
        FlapoMaxSample(2, 6),
        FlapoMaxSample(3, 7),
        FlapoMaxSample(4, 8),
        FlapoMaxSample(1, 9),
        FlapoMaxSample(2, 10),
        FlapoMaxSample(3, 11),
        FlapoMaxSample(4, 12),
        FlapoMaxSample(1, 13),
        FlapoMaxSample(2, 14),
        FlapoMaxSample(3, 15),
        FlapoMaxSample(4, 16),
        FlapoMaxSample(1, 17),
        FlapoMaxSample(2, 18),
        FlapoMaxSample(3, 19),
        FlapoMaxSample(4, 20),
        FlapoMaxSample(1, 21),
        FlapoMaxSample(2, 22)
    };

    for (int i = 0; i < raw->dataCount; i++)
    {
        raw->data[i] = dummySamples[i].getBitsRaw();
    }


    processed = process->process(raw);

    //std::cout << processed->toStr() << std::endl;


    delete raw;
    delete process;
    delete sensor;
    //delete processed; ///now smart
    //!! TODO figure shit out
}

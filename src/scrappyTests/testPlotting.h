#include "flapoProcessedData/flapoProcessedData.h"
#include "flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"

#include "plotLibrary/plot/plotDeepWrapper.h"

#include <chrono>
#include <math.h>
#include <thread> //put here to let me sleep without errors

void testPlotting(FlapoProcessedDataSpectro *data);
void runPlottingTests()
{

    int len = 20;

    auto timeNow = std::chrono::steady_clock::now();
    auto duration = std::chrono::seconds(1);
    int channelCount = 2;
    int *dataLengths = new int[2]{len, len};

    FlapoSpectroChannel* channels = new FlapoSpectroChannel[2];
    channels[0] = FlapoSpectroChannel(1, 1);
    channels[1] = FlapoSpectroChannel(2, 1);

    FlapoMaxSample **data;
    data = new FlapoMaxSample *[2];


    data[0] = new FlapoMaxSample[len];
    data[1] = new FlapoMaxSample[len];
    //set data to some whatver values
    for (int i = 0; i < len; i++)
    {
        data[0][i] = FlapoMaxSample(1, 100 * sin(i / 1.5));
        data[1][i] = FlapoMaxSample(1, 100 * cos(i / 1.5));
        //is sin degrees or radians?    
    }

    FlapoProcessedDataSpectro *dummyData = new FlapoProcessedDataSpectro(timeNow, duration, channelCount, channels, dataLengths, data);
    testPlotting(dummyData);
    delete dummyData;
}

void testPlotting(FlapoProcessedDataSpectro *data)
{
    int plottedChannelCount = data->getChannelCount();
    int plotSize = 10;

    //plot character dims
    int plotScreenSizeX = 100;
    int plotScreenSizeY = 15;

    PlotDeepWrapper* plot = new PlotDeepWrapper(plotScreenSizeX, plotScreenSizeY, plottedChannelCount, plotSize);

    int *dataLengths = data->getDataLengths();
    FlapoMaxSample **dataData = data->getData();

    int *dataToPlot = new int[plotSize];

    //pop in initial 10 data points
    for (int i = 0; i < plottedChannelCount; i++)
    {
        for (int j = 0; j < plotSize; j++)
        {
            dataToPlot[j] = dataData[i][j].getValue();
        }
        plot->updateData(plotSize, dataToPlot, i);
    }
    plot->plotData();

    //pop in data stepSize by stepSize with 1 second sleep
    int stepSize = 1;
    delete[] dataToPlot;
    dataToPlot = new int[stepSize];
    int offset = plotSize; //this was already plotted
    while(offset < dataLengths[0] - stepSize)
    {
        for (int i = 0; i < plottedChannelCount; i++)
        {
            for (int j = 0; j < stepSize; j++)
            {
                dataToPlot[j] = dataData[i][j + offset].getValue();
            }
            plot->updateData(stepSize, dataToPlot, i);
        }
        offset += stepSize;
        
        plot->plotData();
        std::this_thread::sleep_for(std::chrono::milliseconds(250));
    }


    delete[] dataToPlot;
    delete plot; //wtf
}
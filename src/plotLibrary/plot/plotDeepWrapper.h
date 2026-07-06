#pragma once

class PlotWrapper;

class PlotDeepWrapper
{
    private:
        PlotWrapper * wrapped;
    public:
        PlotDeepWrapper(int _plotSizeX, int _plotSizeY, int _plotCount, int _plottedDataSize, int _inputToPlotRatio = 1); 
        void updateData(int count, int *data, int plot_nr);
        void plotData();
        void resetCursor();
        ~PlotDeepWrapper();

};
#pragma once

#include "plot_lib.hpp"
#include "iterators.hpp"

#include <vector>
#include <iostream>

class PlotWrapper
{
    private:
        //trminal plotting
        plot::TerminalInfo *term;
        plot::RealCanvas<plot::BrailleCanvas> *canvas;
        plot::Margin<plot::Frame<plot::RealCanvas<plot::BrailleCanvas> *>> *layout;
        plot::Rectf *bounds;
        plot::Sizef *size;
        plot::Sizef *pixel;

        //data buffers
        int inputToPlotRatio;
        int * channelSkipCount;

        int plotCount;
        int plottedDataSize;;
        int **plottedData;
        std::vector<plot::Pointf> *plotPoints;
        plot::Color *colors;
    public:
        PlotWrapper(int _plotSizeX, int _plotSizeY, int _plotCount, int _plottedDataSize, int _inputToPlotRatio = 1); 
        void updateData(int count, int *data, int plot_nr);
        void plotData();
        void resetCursor();
        ~PlotWrapper();
};

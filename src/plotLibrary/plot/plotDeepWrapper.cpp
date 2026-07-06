#include "plotDeepWrapper.h"
#include "plotWrapper.h"

PlotDeepWrapper::PlotDeepWrapper(int _plotSizeX, int _plotSizeY, int _plotCount, int _plottedDataSize, int _inputToPlotRatio)
{
    wrapped = new PlotWrapper(_plotSizeX, _plotSizeY, _plotCount, _plottedDataSize, _inputToPlotRatio);
}
void PlotDeepWrapper::updateData(int count, int *data, int plot_nr)
{
    wrapped->updateData(count, data, plot_nr);
}
void PlotDeepWrapper::plotData()
{
    wrapped->plotData();
}
void PlotDeepWrapper::resetCursor()
{
    wrapped->resetCursor();
}
PlotDeepWrapper::~PlotDeepWrapper()
{
    delete wrapped;
}
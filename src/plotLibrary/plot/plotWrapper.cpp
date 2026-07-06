#include "plotWrapper.h"



PlotWrapper::PlotWrapper(int _plotSizeX, int _plotSizeY, int _plotCount, int _plottedDataSize, int _inputToPlotRatio)
{
    //set up terminal plotter
    term = new plot::TerminalInfo();
    term->detect();

    canvas = new plot::RealCanvas<plot::BrailleCanvas>(
        {{0.0f, 1.0f}, {1.0f, -1.0f}},
        plot::Size(_plotSizeX, _plotSizeY),
        *term);

    layout = new plot::Margin<plot::Frame<plot::RealCanvas<plot::BrailleCanvas> *>>(
        plot::frame(canvas, *term));

    bounds = new plot::Rectf(canvas->bounds());
    size = new plot::Sizef(canvas->size());
    pixel = new plot::Sizef(canvas->unmap_size({1,1}));

    //set up data buffers
    this->plotCount = _plotCount;
    this->plottedDataSize = _plottedDataSize;
    //make plotCount vectors
    plottedData = new int*[plotCount];
    plotPoints = new std::vector<plot::Pointf>[plotCount];
    //initialize each vector
    for(int i = 0; i < plotCount; i++)
    {
        plottedData[i] = new int[plottedDataSize];
        plotPoints[i] = std::vector<plot::Pointf>(plottedDataSize, {0,0});
    }
    //set up colors
    colors = new plot::Color[plotCount];
    if(plotCount > 0)
    {
        colors[0] = {1.0f, 0.0f, 0.0f}; //red <3
    }
    if(plotCount > 1)
    {
        colors[1] = {1.0f, 0.0f, 0.0f}; //red <3 
    }
    if(plotCount > 2)
    {
        colors[2] = {0.0f, 1.0f, 0.0f}; //green <3
    }
    if(plotCount > 3)
    {
        colors[3] = {0.0f, 1.0f, 0.0f}; //green <3
    }
    if(plotCount > 4)
    {
        colors[4] = {0.0f, 0.0f, 1.0f}; //blue <3
    }
    if(plotCount > 5)
    {
        colors[5] = {0.0f, 0.0f, 1.0f}; //blue <3
    }
    if(plotCount > 6)
    {
        colors[6] = {1.0f, 1.0f, 0.0f}; //yellow <3
    }
    if(plotCount > 7)
    {
        colors[7] = {1.0f, 1.0f, 0.0f}; //yellow <3
    }
    if(plotCount > 8)
    {
        colors[8] = {1.0f, 0.0f, 1.0f}; //purple <3
    }
    if(plotCount > 9)
    {
        colors[9] = {1.0f, 0.0f, 1.0f}; //purple <3
    }
    if(plotCount > 10)
    {
        colors[10] = {0.0f, 1.0f, 1.0f}; //cyan <3
    }
    if(plotCount > 11)
    {
        colors[11] = {0.0f, 1.0f, 1.0f}; //cyan <3
    }
    if(plotCount > 12)
    {
        colors[12] = {1.0f, 0.5f, 0.0f}; //orange <3
    }
    if(plotCount > 13)
    {
        colors[13] = {1.0f, 0.5f, 0.0f}; //orange <3 <3 <3
    }
    //make all other plots white
    for(int i = 12; i < plotCount; i++)
    {
        colors[i] = {1.0f, 1.0f, 1.0f};  //white <3 <3 <3
    }
    //make them all white cos fuck it. remove this later as needed.
    for(int i = 0; i < plotCount; i++)
    {
        colors[i] = {1.0f, 1.0f, 1.0f};  //white <3 <3 <3 <3
    }

    inputToPlotRatio = _inputToPlotRatio;
    channelSkipCount = new int[plotCount];
    for(int i = 0; i < plotCount; i++)
    {
        channelSkipCount[i] = 0;
    }
}
void PlotWrapper::updateData(int count, int *data, int plot_nr)
{
    //check if plot_nr is valid
    if(plot_nr > plotCount)
    {
        plot_nr = 0;
    }
    //check if count is valid //TODO
    //if(count > plottedDataSize * inputToPlotRatio)
    //{
    //    count = plottedDataSize * inputToPlotRatio;
    //}
    //append and remove old

    int realCount = (count + channelSkipCount[plot_nr] + inputToPlotRatio - 1) / inputToPlotRatio;
    if(channelSkipCount[plot_nr] > 0)
    {
        realCount--;
    }
    if(realCount > plottedDataSize)
    {
        realCount = plottedDataSize;
    }

    //append and remove old
    for(int i = 0; i < plottedDataSize - realCount; i++)
    {
        plottedData[plot_nr][i] = plottedData[plot_nr][i + realCount];
    }
    for(int i = 0; i < realCount; i++)
    {
        plottedData[plot_nr][plottedDataSize - realCount + i] = data[i * inputToPlotRatio + ((inputToPlotRatio - channelSkipCount[plot_nr]) % inputToPlotRatio)];
    }

    channelSkipCount[plot_nr] = (channelSkipCount[plot_nr] + count) % inputToPlotRatio;
}
void PlotWrapper::plotData()
{
    //for each plot
    for(int i = 0; i < plotCount; i++)
    {
        //find min and max
        //int min = *std::min_element(plottedData[i].begin(), plottedData[i].end());
        //int max = *std::max_element(plottedData[i].begin(), plottedData[i].end());
        int min = plottedData[i][0];
        int max = plottedData[i][0];
        for(int j = 1; j < plottedDataSize; j++)
        {
            if(plottedData[i][j] < min)
            {
                min = plottedData[i][j];
            }
            if(plottedData[i][j] > max)
            {
                max = plottedData[i][j];
            }
        }
        //convert to points and scale data
        for(int j = 0; j < plottedDataSize; j++)
        {
            float x = (float)j / (float)plottedDataSize;
            float y = ((float)(plottedData[i][j] - min) / (float)(max - min))
            * (bounds->p2.y - bounds->p1.y) + bounds->p1.y;
            //scale y to be between bounds p1 p2 y
            plotPoints[i][j] = {x, y};
        }
    }
    //plot
    canvas->clear().line(term->foreground_color, { bounds->p1.x, 0.0f }, { bounds->p2.x, 0.0f }, plot::TerminalOp::ClipSrc);
    for(int i = 0; i < plotCount; i++)
    {
        canvas->path(this->colors[i] , plotPoints[i].begin(), plotPoints[i].end());
    }
    //clear screen and draw plot
    for (auto const& line: *layout)
        std::cout << term->clear_line() << line << '\n';

    //clear screen
    std::cout << std::flush;
}
void PlotWrapper::resetCursor()
{
    std::cout << term->move_up(layout->size().y) << std::flush;
}
PlotWrapper::~PlotWrapper()
{
    delete term;
    delete canvas;
    delete layout;
    delete bounds;
    delete size;
    delete pixel;
    delete[] plottedData;
    delete[] plotPoints;
    delete[] colors;

}
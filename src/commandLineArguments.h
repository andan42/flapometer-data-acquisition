#pragma once
//what library is strcmp
#include <cstring>
#include <cstdlib>
#include <iostream>

#define COMMAND_LINE_INT_OPTION_CODE(ARGNAME, SHORT_ARGNAME) \
    else if(strcmp(argv[i], "--" #ARGNAME) == 0 || strcmp(argv[i], "-" #SHORT_ARGNAME) == 0) \
    { \
        ARGNAME = atoi(argv[i+1]); \
    }
#define COMMAND_LINE_STRING_OPTION_CODE(ARGNAME, SHORT_ARGNAME) \
    else if(strcmp(argv[i], "--" #ARGNAME) == 0 || strcmp(argv[i], "-" #SHORT_ARGNAME) == 0) \
    { \
        ARGNAME = argv[i+1]; \
    }

class CLargs {


public:
//constructor
    CLargs(int argc, char *argv[])
    {
        for(int i = 0; i < argc; i++)
        {
            if(strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0)
            {
                std::cout << "Usage: " << argv[0] << " [options]" << std::endl;
                std::cout << "Options:" << std::endl;
                std::cout << "  --bus <bus number> (default: 3)" << std::endl;
                std::cout << "  --plotSizeX <plot size x> (default: 140)" << std::endl;
                std::cout << "  --plotSizeY <plot size y> (default: 18)" << std::endl;
                std::cout << "  --running_time <running time> (default: 10)" << std::endl;
                std::cout << "  --led_current <led number> <led current> (default current: 1)" << std::endl;
                std::cout << "  --clockDiv <clock div> (default: 640)" << std::endl;
                std::cout << "  --plottedSecondCount <plotted second count> (default: 5)" << std::endl;
                std::cout << "  --dataFileName <data file name> (default: data)" << std::endl;
                std::cout << "  --frameSkippyConstant <frame skippy constant> (default: 3)" << std::endl;
                std::cout << "  --help, -h (show this help)" << std::endl;
            }
            else if( (strcmp(argv[i], "--led_current") == 0) || (strcmp(argv[i], "-c") == 0) )
            {
                led_current[atoi(argv[i+1]) - 1] = atoi(argv[i+2]);
            }
            COMMAND_LINE_INT_OPTION_CODE(busNumber, b)
            COMMAND_LINE_INT_OPTION_CODE(addressI2C, a)
            COMMAND_LINE_INT_OPTION_CODE(plotSizeX, psx)
            COMMAND_LINE_INT_OPTION_CODE(plotSizeY, psy)
            COMMAND_LINE_INT_OPTION_CODE(running_time, t)
            COMMAND_LINE_INT_OPTION_CODE(clockDiv, cd)
            COMMAND_LINE_INT_OPTION_CODE(plottedSecondCount, psc)
            COMMAND_LINE_INT_OPTION_CODE(frameSkippyConstant, fsc)
            COMMAND_LINE_STRING_OPTION_CODE(dataFileName, o)
            COMMAND_LINE_STRING_OPTION_CODE(operationName, n)
            COMMAND_LINE_STRING_OPTION_CODE(operationDetails, dets)
            COMMAND_LINE_STRING_OPTION_CODE(apiUrl, url)

        }
    };
    static int samplingChannelsCount;
    static int samplingPhotodiodesCount;
    static int busNumber;
    static int addressI2C;
    static int plotSizeX;
    static int plotSizeY;
    static int running_time;
    static int led_current[9];
    static int clockDiv;
    static int plottedSecondCount;
    static int frameSkippyConstant;
    static const char *dataFileName;
    static const char *operationName;
    static const char *operationDetails;
    static const char *apiUrl;
};
int CLargs::busNumber = 5;
int CLargs::addressI2C = 0x64;
int CLargs::plotSizeX = 140;
int CLargs::plotSizeY = 18;
int CLargs::running_time = 10;
int CLargs::clockDiv = 640;
int CLargs::plottedSecondCount = 5;
int CLargs::frameSkippyConstant = 3;
const char *CLargs::dataFileName = "data";
int CLargs::led_current[9] = {20, 20, 20, 20, 20, 20, 20, 20, 20};
const char *CLargs::operationName = "test";
const char *CLargs::operationDetails = "test details";
const char *CLargs::apiUrl = "http://192.168.1.6:4000";
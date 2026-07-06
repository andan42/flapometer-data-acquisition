#include "flapoRawDataThermo.h"

FlapoRawDataThermo::FlapoRawDataThermo(FlapoSensor *polledSensor) :
FlapoRawData(polledSensor), channels(INT32_MAX) //start in an invalid state, for later error checking
{
}

FlapoRawDataThermo::~FlapoRawDataThermo()
{
    //nothing to do
}
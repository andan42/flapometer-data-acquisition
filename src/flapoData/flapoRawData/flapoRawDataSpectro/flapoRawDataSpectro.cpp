#include "flapoRawDataSpectro.h"

FlapoRawDataSpectro::FlapoRawDataSpectro(FlapoSensor *polledSensor) : 
FlapoRawData(polledSensor), overflow(0)
{
}

FlapoRawDataSpectro::~FlapoRawDataSpectro()
{
    //nothing to do
}

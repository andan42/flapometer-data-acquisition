#include "flapoSpectroChannel.h"

FlapoSpectroChannel::FlapoSpectroChannel(int measurementNr, int photodiodeNr)
 : measurementNr(measurementNr), photodiodeNr(photodiodeNr)
{
    
}

FlapoSpectroChannel::FlapoSpectroChannel()
{
}

bool FlapoSpectroChannel::operator==(const FlapoSpectroChannel &other) const
{
    return measurementNr == other.measurementNr && photodiodeNr == other.photodiodeNr;
}

bool FlapoSpectroChannel::operator!=(const FlapoSpectroChannel &other) const
{
    return !(*this == other);
}

std::string FlapoSpectroChannel::toStr()
{
    //single line just the 2 ints with minimal stng NOT THE  POINTER GOD DAMN IT
    std::string str = "Meas Nr: " + std::to_string(measurementNr) + ", PD Nr: " + std::to_string(photodiodeNr);
    return str;
}
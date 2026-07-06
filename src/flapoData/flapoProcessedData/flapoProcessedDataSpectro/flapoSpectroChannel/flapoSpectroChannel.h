#pragma once
#include <string>
struct FlapoSpectroChannel
{
    int measurementNr;
    int photodiodeNr;
    FlapoSpectroChannel(int measurementNr /*1 through 9*/, int photodiodeNr /*0 or 1*/);
    FlapoSpectroChannel();

    bool operator==(const FlapoSpectroChannel &other) const;
    bool operator!=(const FlapoSpectroChannel &other) const;
    std::string toStr();
};
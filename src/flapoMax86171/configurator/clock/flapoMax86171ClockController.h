#pragma once
#include "flapoMax86171/chip/flapoMax86171.h"



class DeviceI2C;
class FlapoMax86171ClockController
{
private:
    FlapoMax86171 *m_afe;
public:
    FlapoMax86171ClockController(FlapoMax86171 *afe);
    ~FlapoMax86171ClockController();

    //clock 0 is 32kHz, clock 1 is 32.768kHz
    int setClockFrequency(double frequency_hz, int clockSelect);
    double getActualFrequency();

    int setClockDivider(int clockDivider);
    int setClockSource(int clockSelect);
};
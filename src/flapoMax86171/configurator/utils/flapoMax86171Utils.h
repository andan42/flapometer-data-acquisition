#pragma once

//#include "flapoMax86171/constants/flapoMax86171Registers.h"

namespace flapoMax86171Utils{

    //map pin to main drive select register
    char getDriveRegister(int pin);

    //map pin to alternate drive select register
    char getAltDriveRegister(int pin);

    //map pin AND drive select to drive register value
    int getDriveValueAuto(int pin, char driveRegister);

    //map pin to main drive select register value. used internally.
    int getDriveValue(int pin);

    //map pin to alternate drive select register value. used internally.
    int getAltDriveValue(int pin);

    //map wavelength to pin
    int getPin(int wavelength);

    //get closest ppg1 diode for a given pin: PD_1 or PD_3 can be used for PPG1
    int getPpg1Diode(int pin);

    //get closest ppg2 diode for a given pin: PD_2 or PD_4 can be used for PPG2
    int getPpg2Diode(int pin);
};
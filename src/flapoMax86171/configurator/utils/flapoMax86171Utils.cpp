#include "flapoMax86171Utils.h"

char flapoMax86171Utils::getDriveRegister(int pin)
{
    //drvA 1 2 4 7
    //drvB 2 3 5 8
    //drvC 1 3 6 9 
    switch (pin)
    {
    case 1:
        return 'A'; break;
    case 2:
        return 'B'; break;
    case 3:
        return 'C'; break;
    case 4:
        return 'A'; break;
    case 5:
        return 'B'; break;
    case 6:
        return 'C'; break;
    case 7:
        return 'A'; break;
    case 8:
        return 'B'; break;
    case 9:
        return 'C'; break;
    default:
        return -1; break;
    }
}

char flapoMax86171Utils::getAltDriveRegister(int pin)
{
    //drvA 1 2 4 7
    //drvB 2 3 5 8
    //drvC 1 3 6 9 
    switch (pin)
    {
    case 1:
        return 'C'; break;
    case 2:
        return 'A'; break;
    case 3:
        return 'B'; break;
    case 4:
        return 'A'; break;
    case 5:
        return 'B'; break;
    case 6:
        return 'C'; break;
    case 7:
        return 'A'; break;
    case 8:
        return 'B'; break;
    case 9:
        return 'C'; break;
    default:
        return -1; break;
    }
}

int flapoMax86171Utils::getDriveValueAuto(int pin, char driveRegister)
{
    if (driveRegister == getDriveRegister(pin))
    {
        return getDriveValue(pin);
    }
    else if (driveRegister == getAltDriveRegister(pin))
    {
        return getAltDriveValue(pin);
    }
    else
    {
        return -1;
    }
}

int flapoMax86171Utils::getDriveValue(int pin)
{
    //val  0 1 2 3
    //drvA 1 2 4 7
    //drvB 2 3 5 8
    //drvC 1 3 6 9 

    switch (pin)
    {
    case 1:
        return 0; break;
    case 2:
        return 0; break;
    case 3:
        return 1; break;
    case 4:
        return 2; break;
    case 5:
        return 2; break;
    case 6:
        return 2; break;
    case 7:
        return 3; break;
    case 8:
        return 3; break;
    case 9:
        return 3; break;
    default:
        return -1; break;
    }
}

int flapoMax86171Utils::getAltDriveValue(int pin)
{
    //val  0 1 2 3
    //drvA 1 2 4 7
    //drvB 2 3 5 8
    //drvC 1 3 6 9 

    switch (pin)
    {
    case 1:
        return 0; break;
    case 2:
        return 1; break;
    case 3:
        return 1; break;
    case 4:
        return 2; break;
    case 5:
        return 2; break;
    case 6:
        return 2; break;
    case 7:
        return 3; break;
    case 8:
        return 3; break;
    case 9:
        return 3; break;
    default:
        return -1; break;
    }
}

int flapoMax86171Utils::getPin(int wavelength)
{
    //550 600 650 700 750 800 850 900 950
    //7   6   5   8   1   9   4   3   2
    switch (wavelength)
    {
    case 550:
        return 7; break;
    case 600:
        return 6; break;
    case 650:
        return 5; break;
    case 700:
        return 8; break;
    case 750:   
        return 1; break;
    case 800:
        return 9; break;
    case 850:
        return 4; break;
    case 900:
        return 3; break;
    case 950:
        return 2; break;
    default:
        return -1; break;
    }
}

//insider information:
//pd1 and pd2 are next to leds 1 2 4 5 7

//pd3 and pd4 are next to leds 3 6 8 9

//ppg1 can use pd1 = retrun 0 
//or pd3 = return 1
int flapoMax86171Utils::getPpg1Diode(int pin)
{
    if (pin == 1 || pin == 2 || pin == 4 || pin == 5 || pin == 7)
    {
        return 0;
    }
    else if (pin == 3 || pin == 6 || pin == 8 || pin == 9)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

//ppg2 can use pd2 = retrun 0
//or pd4 = return 1
int flapoMax86171Utils::getPpg2Diode(int pin)
{
    if (pin == 1 || pin == 2 || pin == 4 || pin == 5 || pin == 7)
    {
        return 0;
    }
    else if (pin == 3 || pin == 6 || pin == 8 || pin == 9)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

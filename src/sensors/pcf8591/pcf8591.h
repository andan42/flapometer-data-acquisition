#pragma once

#include "i2c/DeviceI2C.h"

static const int PCF8591_BASE_ADDRESS = 0x48;
class Pcf8591
{
    public :
        DeviceI2C * m_deviceI2C;
        int m_channel;
        //Pcf8591(DeviceI2C * deviceI2C, int channel);
        Pcf8591(int address, int busNumber, int channelNumber);     
        ~Pcf8591();
        //delete copy constructor and assignment operator
        Pcf8591(const Pcf8591&) = delete;
        Pcf8591& operator=(const Pcf8591&) = delete;
        //declare move constructor and assignment operator
        Pcf8591(Pcf8591&&);
        Pcf8591& operator=(Pcf8591&&);
        float read();
};


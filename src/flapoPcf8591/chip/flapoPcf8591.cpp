#include "flapoPcf8591/chip/flapoPcf8591.h"

#include "i2c/DeviceI2C.h"
#include "strings.h"

#include <iostream>
#include "flapoPcf8591.h"



FlapoPcf8591::FlapoPcf8591(DeviceI2C *deviceI2C)
{
    m_deviceI2C = deviceI2C;
}

FlapoPcf8591::~FlapoPcf8591()
{
}

int FlapoPcf8591::read()
{
    int ret = m_deviceI2C->readAddresslessByte();
    return ret;
}

void FlapoPcf8591::readDummy()
{
    this->read();
}

void FlapoPcf8591::writeControl(bool outputEnabled, char inputProgramming, bool autoIncrement, char channel)
{
    //control_byte = 0bXOPPXICC
    // = not used
    //O = 0: output disabled, 1: [output enabled] (output must be enabled in order for internal oscilaltor to operate)
    //PP = 00: [4 single ended], 01: 3 differential, 10: mixed, 11: 2 differential
    //I = 0: [auto increment disabled], 1: auto increment enabled
    //CC = channel configuration, 00: AIN0, 01: AIN1, 10: AIN2, 11: AIN3 = m_channe
    uint8_t control_byte = 0
        | (outputEnabled ? 1 << 6 : 0) //
        | ((inputProgramming & 0b11) << 4)
        | (autoIncrement ? 1 << 2 : 0) //TODO make sure this doesnt compile to something with branches
        | (channel & 0b11);

    m_deviceI2C->writeAddresslessByte(control_byte);
    return;
}
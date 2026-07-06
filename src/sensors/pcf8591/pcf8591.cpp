#include "pcf8591.h"

#include <iostream>

Pcf8591::Pcf8591(int address, int busNumber, int channelNumber)
{
    //address = PCF8591_BASE_ADDRESS first 4 bits + 3 hardware selectable bits
    int _address = PCF8591_BASE_ADDRESS | (address & 0b111);
    m_deviceI2C = new DeviceI2C(_address, busNumber);
    m_channel = channelNumber;
}
Pcf8591::~Pcf8591()
{
    delete m_deviceI2C;
}
float Pcf8591::read()
{
    int ret = 0;
    uint8_t control_byte = 0;
    uint8_t values[4] = {0, 0, 0, 0};
    //control_byte = 0b_OPP_ICC
    //_ = not used
    //O = 0: output disabled, 1: [output enabled] (output must be enabled in order for internal oscilaltor to operate)
    //PP = 00: [4 single ended], 01: 3 differential, 10: mixed, 11: 2 differential
    //I = 0: [auto increment disabled], 1: auto increment enabled
    //CC = channel configuration, 00: AIN0, 01: AIN1, 10: AIN2, 11: AIN3 = m_channel
    control_byte = 0b01000000 | (m_channel & 0b11);
    m_deviceI2C->writeAddresslessByte(control_byte);
    
    //conversion happens during read command, and is only available on the next.
    //this means that the first read will be the previous value, and the second read will be the current value
    m_deviceI2C->readAddresslessByte();
    
    //additionally, it seems the first read has hysteresis from previous read. this seems like an issue with the chip. workaround is discard some more bytes //TODO: investigate, maybe this is not needed. hardware mightb e fucked
    m_deviceI2C->readAddresslessByte();

    //read 4 bytes
    for(int i = 0; i < 4; i++)
    {
        values[i] = m_deviceI2C->readAddresslessByte();
    }
    //average the 4 bytes
    for(int i = 0; i < 4; i++)
    {
        ret += values[i];
    }
    ret = ret / 4;
    //std::cout << "channel: " << m_channel << "     read: " << (int)values[0] << " " << (int)values[1] << " " << (int)values[2] << " " << (int)values[3] << " " << std::endl;
    return ret; 
}
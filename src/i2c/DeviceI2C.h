#pragma once
#include <cstdint>
//library for lock guard
#include <mutex>

class DeviceI2C
{
public:
    DeviceI2C(int address, int busNumber);
    ~DeviceI2C();

    uint8_t readAddresslessByte();
    void writeAddresslessByte(uint8_t data);

    int readByte(int reg);
    int writeByte(int reg, int value);
    int readBytes(int reg, int length, uint8_t *data);
    void writeBytes(int reg, int length, uint8_t *data);
    bool isOk();
    void reinit();
    void reinit(int address, int busNumber);
    void searchDevices();

    //this function should set the bit to the value while keeping the other bits intact
    void writeBit(int reg, int bitMask, int value);
    //this function should automatically shift the value to the correct position while keeping the other bits intact
    void writeBitField(int reg, int bitFieldMask, int value);

    // private:
    int m_address;
    int m_busNumber;
    int m_busFile;
    int m_error;
private:
    //lock guard
    std::mutex m_mutex;
};
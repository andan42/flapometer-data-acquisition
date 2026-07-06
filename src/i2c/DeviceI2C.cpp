#include "DeviceI2C.h"


#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
extern "C" {
#include <linux/i2c-dev.h>
#include <i2c/smbus.h>
}
#include <iostream>
#include <string.h>

#include <asm-generic/errno.h>

#include "flapoUtils/i2cutils.h"

DeviceI2C::DeviceI2C(int address, int busNumber): m_address(address), m_busNumber(busNumber), m_error(0)
{
    std::string devPath = "/dev/i2c-";
    devPath.append(std::to_string(busNumber));
    m_busFile = open(devPath.c_str(), O_RDWR);
    if(m_busFile < 0)
    {
        std :: cout << "i2c device setup failed, could not open:" << devPath << std :: endl;
        m_error = -1;
    }
    int retIoctl = ioctl(m_busFile, I2C_SLAVE, m_address);
    if(retIoctl < 0)
    {
        std ::cout << "i2c device setup failed, could not get bus access: " << std :: endl
        << "ioctl(\"" << devPath << "\", 0x" << std::hex << m_address << ") returned: " << retIoctl << std :: endl;
        m_error = -1;
    }
    //check if there is a device at the address
    if(i2c_smbus_read_byte(m_busFile) < 0)
    {
        std :: cout << "i2c device setup failed, could not read from address: " << std::hex << m_address << 
        " (bus: " << m_busNumber << ")" << std :: endl << "No device at address" << std :: endl;
        m_error = -1;
    }
}

DeviceI2C::~DeviceI2C()
{
    close(m_busFile);
}

uint8_t DeviceI2C::readAddresslessByte()
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return i2c_smbus_read_byte(m_busFile);
}

void DeviceI2C::writeAddresslessByte(uint8_t data)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    i2c_smbus_write_byte(m_busFile, data);
}

#include <future>
#include <chrono>

int DeviceI2C::readByte(int reg)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    int data = i2c_smbus_read_byte_data(m_busFile, reg);
    if (data < 0)
    {
        // std::cout << "i2c readByte() returned " << data << std::endl;
    }
    return data;
}

int DeviceI2C::writeByte(int reg, int value)
{
    std::lock_guard<std::mutex> lock(m_mutex);
    return i2c_smbus_write_byte_data(m_busFile, reg, value);
}

int DeviceI2C::readBytes(int reg, int length, uint8_t *data)
// implemented this way because i2c_smbus_read_i2c_block_data does not work, it is limited to 32 bytes...
{
    std::lock_guard<std::mutex> lock(m_mutex);
    ssize_t ret;
    ret = write(m_busFile, &reg, 1);
    if(ret < 0)
    {
        // std :: cout << "i2c readBytes() returned" << ret << std :: endl;
        return ret;
    }
    ret = read(m_busFile, data, length);
    if(ret < 0)
    {
        // std :: cout << "i2c readBytes() returned" << ret << std :: endl;
        return ret;
    }

    return 0;
}

//limited to 32 bytes thanks to i2c_smbus_read_i2c_block_data.. thanks a lot linux
void DeviceI2C::writeBytes(int reg, int length, uint8_t *data)
{
    if(length > 32)
    {
        std :: cout << "i2c writeBytes() failed, length > 32" << std :: endl;
        return;
    }
    std::lock_guard<std::mutex> lock(m_mutex);
    i2c_smbus_write_i2c_block_data(m_busFile, reg, length, data);
}

bool DeviceI2C::isOk()
{
    // PART_ID value:0x2C
    // PART_ID_ADDR:0xFF
    int retIoctl = ioctl(m_busFile, I2C_SLAVE, m_address);
    if (retIoctl < 0)
    {
        // std ::cout << "i2c device setup failed, could not get bus access: " << std ::endl
        //            << "ioctl(\"" << m_busFile << "\", 0x" << std::hex << m_address << ") returned: " << retIoctl << std ::endl;
        std::cout << "i2c device is not ok, could not get bus access!" << std::endl;
        m_error = -1;
        return false;
    }

    int partId = readByte(0xFF);
    if (partId != 0x2C)
    {
        std::cout << "i2c device is not ok, partId is not 0x2C, but 0x" << std::hex << partId << std::endl;
        return false;
    }
    return true;
}

void DeviceI2C::reinit()
{
    // Close the bus if it is already open
    if (m_busFile >= 0)
    {
        close(m_busFile);
    }

    // Reinitialize m_error
    m_error = 0;

    // Reopen the I2C bus
    std::string devPath = "/dev/i2c-";
    devPath.append(std::to_string(m_busNumber));
    m_busFile = open(devPath.c_str(), O_RDWR);

    // Check if the bus is opened successfully
    if (m_busFile < 0)
    {
        std::cout << "i2c device reinit failed, could not open: " << devPath << std::endl;
        m_error = -1;
        return;
    }

    // Set the I2C slave address
    int retIoctl = ioctl(m_busFile, I2C_SLAVE, m_address);
    if (retIoctl < 0)
    {
        std::cout << "i2c device reinit failed, could not set bus access: " << devPath << std::endl;
        m_error = -1;
        return;
    }

    // Optionally check if the device is present at the address
    if (i2c_smbus_read_byte(m_busFile) < 0)
    {
        std::cout << "i2c device reinit failed, no device at address: " << std::hex << m_address << std::endl;
        m_error = -1;
        return;
    }

    // check if the device is ok
    if (!isOk())
    {
        std::cout << "i2c device reinit failed, device is not ok" << std::endl;
        m_error = -1;
        return;
    }

    std::cout << "i2c device reinit successful" << std::endl;
}

void DeviceI2C::reinit(int address, int busNumber)
{
    m_address = address;
    m_busNumber = busNumber;
    reinit();
}

void DeviceI2C::searchDevices()
{
    int addr = scanBusFlapDevice(m_busNumber);
    if (addr < 0)
    {
        return;
    }
    if (addr != m_address)
    {
        std::cout << "different i2c device searchDevices() found device at address: " << std::hex << addr << std::endl;
        std::cout << "reinitializing device" << std::endl;
        this->reinit(addr, m_busNumber);
    }
}

void DeviceI2C::writeBit(int reg, int bitMask, int value)
{
    uint8_t data = (uint8_t)readByte(reg); // todo check for error
    value = (value ? 0xFF : 0x00);
    data = (data & ~bitMask) | (value & bitMask);
    writeByte(reg, data);
}

void DeviceI2C::writeBitField(int reg, int bitFieldMask, int value)
{
    uint8_t data = (uint8_t)readByte(reg); // todo check for error
    value = (value << (ffs(bitFieldMask) - 1));
    data = (data & ~bitFieldMask) | (value & bitFieldMask);
    writeByte(reg, data);
}


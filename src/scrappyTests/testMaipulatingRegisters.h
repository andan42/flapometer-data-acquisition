#include <iostream>
#include <chrono>

#include "DeviceI2C.h"
#include <thread> 

#include "flapoMax86171/constants/flapoMax86171Registers.h"

#include "flapoMax86171/chip/flapoMax86171.h"

#include <bitset>

void testManipulatingRegisters()
{
    
    DeviceI2C *device = new DeviceI2C(0x64, 5);
    FlapoMax86171 *flapoConfiguratorSpectro = new FlapoMax86171(device);

    int * ret = new int;
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS9_EN, ret);
    std::cout << "post  readRegister(MEAS9_EN) outputs: " << *ret << std::endl;
    flapoConfiguratorSpectro->writeRegister(flapoRegistersSpectro::MEAS9_EN, 1);
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS9_EN, ret);
    std::cout << "post readRegister(MEAS9_EN) outputs: " << *ret << std::endl;

    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS8_EN, ret);
    std::cout << "pre readRegister(MEAS8_EN) outputs: " << *ret << std::endl;
    flapoConfiguratorSpectro->writeRegister(flapoRegistersSpectro::MEAS8_EN, 1);
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS8_EN, ret);
    std::cout << "post readRegister(MEAS8_EN) outputs: " << *ret << std::endl;

    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS8_EN, ret);
    std::cout << "pre readRegister(MEAS8_EN) outputs: " << *ret << std::endl;
    flapoConfiguratorSpectro->writeRegister(flapoRegistersSpectro::MEAS8_EN, 0);
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS8_EN, ret);
    std::cout << "post readRegister(MEAS8_EN) outputs: " << *ret << std::endl;

    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS7_EN, ret);
    std::cout << "pre readRegister(MEAS7_EN) outputs: " << *ret << std::endl;
    flapoConfiguratorSpectro->writeRegister(flapoRegistersSpectro::MEAS7_EN, 2);
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS7_EN, ret);
    std::cout << "post readRegister(MEAS7_EN) outputs: " << *ret << std::endl;

    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS6_EN, ret);
    std::cout << "pre readRegister(MEAS6_EN) outputs: " << *ret << std::endl;
    flapoConfiguratorSpectro->writeRegister(flapoRegistersSpectro::MEAS6_EN, 1);
    flapoConfiguratorSpectro->readRegister(flapoRegistersSpectro::MEAS6_EN, ret);
    std::cout << "post readRegister(MEAS6_EN) outputs: " << *ret << std::endl;
}
/*#include <iostream>
#include <bitset>

#include "scrappyTests/testTurningOnLeds.h"
#include "flapoMax86171/configurator/clock/flapoMax86171ClockController.h"

#include <string.h>
#include <cassert>
#include <iomanip>
#include <limits>

#define TEST_LINE_VALUE_EXPECTED(line, value, expected) \
if(value != expected) \
{ \
    std::cout << "Test failed at line " << line << std::endl; \
    std::cout << "Actual Value: " << std::setprecision(std::numeric_limits<double>::max_digits10) << value << std::endl; \
    std::cout << "Expected Value: " << std::setprecision(std::numeric_limits<double>::max_digits10)<< expected << std::endl; \
}

int main(int argc, char** argv)
{
    //find arguments
    int address = -1, busNumber = -1;
    for (int i = 0; i < argc; i++)
    {
        //if arg[i] matches, set value to arg[i + 1]
        if (strcmp(argv[i], "address") == 0)
        {
            address = atoi(argv[i + 1]);
        }
        else if (strcmp(argv[i], "busNumber") == 0)
        {
            busNumber = atoi(argv[i + 1]);
        }
    }
    //check if arguments are valid
    if (address < 0 || busNumber < 0)
    {
        std::cout << "Invalid arguments, please provide address and busNumber in test/args file" << std::endl;
        return -1;
    }

    DeviceI2C *device = new DeviceI2C(address, busNumber);
    FlapoMax86171 *afe = new FlapoMax86171(device);
    FlapoMax86171ClockController *clockController = new FlapoMax86171ClockController(afe);
    //lets write some code for the chip Clock!!
    clockController->setClockDivider(0x7FFF);
    clockController->setClockSource(0);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 0.97659230323191015);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x7F);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0xFF);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x0);
    clockController->setClockSource(1);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 1.000030518509476);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x7F);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0xFF);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x1);

    clockController->setClockDivider(32000);
    clockController->setClockSource(0);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 1);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x7D);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0x00);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x0);
    clockController->setClockSource(1);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 1.024);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x7D);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0x00);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x1);

    clockController->setClockFrequency(32000, 0);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 32000);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x00);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0x01);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x0);
    clockController->setClockFrequency(32000, 1);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 32768);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 0x00);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 0x01);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x1);

    clockController->setClockFrequency(3, 0);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 3.0001875117194823);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 41);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 170);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x0);

    clockController->setClockFrequency(3, 1);
    TEST_LINE_VALUE_EXPECTED(__LINE__, clockController->getActualFrequency(), 3.000183116645303);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &ret); return ret;}(), 42);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &ret); return ret;}(), 170);
    TEST_LINE_VALUE_EXPECTED(__LINE__, [&](){int ret; afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &ret); return ret;}(), 0x1);

    delete clockController;
    delete afe;
    delete device;

    //this allows to visually IRL check if the clock updated by looking at the leds, as activated by the testTurningOnLeds() function
    //keep this off while the tests are automated by `make test`
    //std::cout << "Test Turning On Leds" << std::endl;

    //testTurningOnLeds();

    return 0;
}*/
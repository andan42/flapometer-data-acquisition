#include <iostream>

#include <chrono>
#include <thread>

#include "i2c/DeviceI2C.h"
#include "flapoMax86171/chip/flapoMax86171.h"


void testTurningOnLeds()
{
    DeviceI2C *device = new DeviceI2C(0x65, 5);
    FlapoMax86171 *afe = new FlapoMax86171(device);

    //lets try to turn on a led
    /*
        what to do:
            open kicad check visible wavelength led pin
                led 7 or 6

            check old code to see how to turn on led > was not necessary :>

            check datasheet to make sure im not dumb
        steps:
            >MEAS x _EN
            >MEAS x _DRV y
            >MEAS x DRV y current
    */

    //select pin 6 on drvc (0x2)
    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 1);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVC, 0x2); //select pin 6 on drvC
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVC_PA, 0xFF); //max out

    //sleep 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVC, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVC_PA, 0);

    //select pin 7 on drva (0x3)
    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 1);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA, 0x3); //select pin 7 on drvA
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA_PA, 0xFF); //max out

    //sleep 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA_PA, 0);

    //now do both leds
    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 1);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA, 0x3); //select pin 7 on drvA
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA_PA, 0xFF); //max out

    afe->writeRegister(flapoRegistersSpectro::MEAS2_EN, 1);
    afe->writeRegister(flapoRegistersSpectro::MEAS2_DRVC, 0x2); //select pin 6 on drvC
    afe->writeRegister(flapoRegistersSpectro::MEAS2_DRVC_PA, 0xFF); //max out

    //sleep 10 seconds
    std::this_thread::sleep_for(std::chrono::seconds(10));

    afe->writeRegister(flapoRegistersSpectro::MEAS1_EN, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS1_DRVA_PA, 0);

    afe->writeRegister(flapoRegistersSpectro::MEAS2_EN, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS2_DRVC, 0);
    afe->writeRegister(flapoRegistersSpectro::MEAS2_DRVC_PA, 0);
}
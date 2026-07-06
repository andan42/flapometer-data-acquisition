#pragma once

#include "flapoMax86171/configurator/flapoMax86171Controller.h"

#include "i2c/DeviceI2C.h"

#include <iostream>

void helpSetupMeasurement(FlapoMax86171MeasurementController *controller, int ledPin, int power)
{
    //declare a function variable that returns flapoRegisterSpectro and takes int

    flapoRegister (*reg)(int) = nullptr;
    flapoRegister (*reg_pa)(int) = nullptr;

    int val;
    // Selects the correct led driver for the led pin
    switch (ledPin)
    {
    case 1:
        reg = flapoRegistersSpectro::MEAS_N_DRVA;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVA_PA;
        val = 0;
        break;
    case 2:
        reg = flapoRegistersSpectro::MEAS_N_DRVB;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVB_PA;
        val = 0;
        break;
    case 3:
        reg = flapoRegistersSpectro::MEAS_N_DRVC;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVC_PA;
        val = 1;
        break;
    case 4:
        reg = flapoRegistersSpectro::MEAS_N_DRVA;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVA_PA;
        val = 2;
        break;
    case 5:
        reg = flapoRegistersSpectro::MEAS_N_DRVB;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVB_PA;
        val = 2;
        break;
    case 6:
        reg = flapoRegistersSpectro::MEAS_N_DRVC;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVC_PA;
        val = 2;
        break;
    case 7:
        reg = flapoRegistersSpectro::MEAS_N_DRVA;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVA_PA;
        val = 3;
        break;
    case 8:
        reg = flapoRegistersSpectro::MEAS_N_DRVB;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVB_PA;
        val = 3;
        break;
    case 9:
        reg = flapoRegistersSpectro::MEAS_N_DRVC;
        reg_pa = flapoRegistersSpectro::MEAS_N_DRVC_PA;
        val = 3;
        break;
    default:
        std::cout << "invalid led pin fucka" << std::endl;
        return;
        break; 
    }

    controller->writeRegister(reg, val);
    controller->writeRegister(reg_pa, power);
    controller->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 1);



}
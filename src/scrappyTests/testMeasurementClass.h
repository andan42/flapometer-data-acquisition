#pragma once

#include "flapoMax86171/configurator/flapoMax86171Controller.h"

#include "i2c/DeviceI2C.h"

#include <iostream>

void TestMeasurementClass()
{
    DeviceI2C *device = new DeviceI2C(0x64, 5);
    FlapoMax86171 *afe = new FlapoMax86171(device);
    FlapoMax86171Controller *controller = new FlapoMax86171Controller(afe);

    FlapoMax86171MeasurementController *measurementController1 = controller->getMeasurementController(1);
    FlapoMax86171MeasurementController *measurementController7 = controller->getMeasurementController(7);

    int val = 0;

    //reset chip to ensure old data isnt there for false positives
    afe->writeRegister(flapoRegistersSpectro::RESET, 1);

    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, &val);
    std::cout << "before: 1 - MEAS_N_DRVA_PA: " << val << std::endl;
    measurementController1->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 69);
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, &val);
    std::cout << "after: 1 - MEAS_N_DRVA_PA: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS1_DRVA_PA, &val);
    std::cout << "confirm: 1 - MEAS_N_DRVA_PA: " << val << std::endl;

    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, &val);
    std::cout << "before: 7 - MEAS_N_DRVA_PA: " << val << std::endl;
    measurementController7->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, 69);
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, &val);
    std::cout << "after: 7 - MEAS_N_DRVA_PA: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS7_DRVA_PA, &val);
    std::cout << "confirm: 7 - MEAS_N_DRVA_PA: " << val << std::endl;

    //check with drvc instead of dra on 7
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVC_PA, &val);
    std::cout << "before: 7 - MEAS_N_DRVC_PA: " << val << std::endl;
    measurementController7->writeRegister(flapoRegistersSpectro::MEAS_N_DRVC_PA, 42);
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVC_PA, &val);
    std::cout << "after: 7 - MEAS_N_DRVC_PA: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS7_DRVC_PA, &val);
    std::cout << "confirm: 7 - MEAS_N_DRVC_PA: " << val << std::endl;

    //check with drvc instead of drvc_pa
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVC, &val);
    std::cout << "before: 7 - MEAS_N_DRVC: " << val << std::endl;
    measurementController7->writeRegister(flapoRegistersSpectro::MEAS_N_DRVC, 2);
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_DRVC, &val);
    std::cout << "after: 7 - MEAS_N_DRVC: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS7_DRVC, &val);
    std::cout << "confirm: 7 - MEAS_N_DRVC: " << val << std::endl;

    //test meas_n_en
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "before: 7 - MEAS_N_EN: " << val << std::endl;
    measurementController7->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 1);
    measurementController7->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "after: 7 - MEAS_N_EN: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS7_EN, &val);
    std::cout << "confirm: 7 - MEAS_N_EN: " << val << std::endl;

    //test with measurmentt1 
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "before: 1 - MEAS_N_EN: " << val << std::endl;
    measurementController1->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 1);
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "after: 1 - MEAS_N_EN: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS1_EN, &val);
    std::cout << "confirm: 1 - MEAS_N_EN: " << val << std::endl;

    //put invalid value in meas_n_en
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "before: 1 - MEAS_N_EN: " << val << std::endl;
    measurementController1->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 2);
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "after: 1 - MEAS_N_EN: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS1_EN, &val);
    std::cout << "confirm: 1 - MEAS_N_EN: " << val << std::endl;

    //test setting meas 1 en back to zero
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "before: 1 - MEAS_N_EN: " << val << std::endl;
    measurementController1->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 0);
    measurementController1->readRegister(flapoRegistersSpectro::MEAS_N_EN, &val);
    std::cout << "after: 1 - MEAS_N_EN: " << val << std::endl;
    afe->readRegister(flapoRegistersSpectro::MEAS1_EN, &val);
    std::cout << "confirm: 1 - MEAS_N_EN: " << val << std::endl;
    

}
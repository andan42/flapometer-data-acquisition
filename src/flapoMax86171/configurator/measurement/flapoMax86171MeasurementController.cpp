
#include "flapoMax86171MeasurementController.h"

#include <iostream>

FlapoMax86171MeasurementController::FlapoMax86171MeasurementController(FlapoMax86171 *afe, int measurement_nr)
:m_afe(afe), m_measurement_nr(measurement_nr)
{
    //check if measurement_nr is in range
    if (measurement_nr < 1 || measurement_nr > 9)
    {
        std::cerr << "Measurement number out of range" << std::endl;
    }
}

FlapoMax86171MeasurementController::~FlapoMax86171MeasurementController()
{
}

int FlapoMax86171MeasurementController::readRegister(flapoRegister (*reg)(int), int *val)
{
    return m_afe->readRegister(reg(m_measurement_nr), val);
}

int FlapoMax86171MeasurementController::writeRegister(flapoRegister (*reg)(int), int val)
{
    return m_afe->writeRegister(reg(m_measurement_nr), val);
}

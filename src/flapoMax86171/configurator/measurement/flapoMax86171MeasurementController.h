#pragma once
#include "flapoMax86171/chip/flapoMax86171.h"


class DeviceI2C; //do i need this???     TODO
class FlapoMax86171MeasurementController
{
private:
    FlapoMax86171 *m_afe;
    int m_measurement_nr;
public:
    FlapoMax86171MeasurementController(FlapoMax86171 *afe, int measurement_nr);
    ~FlapoMax86171MeasurementController();
    int readRegister(flapoRegister (*reg)(int), int *val);
    int writeRegister(flapoRegister (*reg)(int), int val);
};

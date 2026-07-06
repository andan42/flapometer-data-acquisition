#pragma once
#include "flapoMax86171/chip/flapoMax86171.h"

#include "flapoMax86171/configurator/clock/flapoMax86171ClockController.h"
#include "flapoMax86171/configurator/measurement/flapoMax86171MeasurementController.h"
class DeviceI2C;
class FlapoMax86171Controller
{
private:
    FlapoMax86171 *m_afe; //afe stands for analog front end
    FlapoMax86171ClockController *clockController;
    FlapoMax86171MeasurementController *measurementController[9];
public:
    FlapoMax86171 * getAfe();
    FlapoMax86171Controller(FlapoMax86171 *afe);
    ~FlapoMax86171Controller();
    FlapoMax86171ClockController* getClockController();
    FlapoMax86171MeasurementController* getMeasurementController(int measurement_nr);

};
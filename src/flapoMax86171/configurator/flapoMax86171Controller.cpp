#include "flapoMax86171Controller.h"

FlapoMax86171 *FlapoMax86171Controller::getAfe()
{
    return m_afe;
}

FlapoMax86171Controller::FlapoMax86171Controller(FlapoMax86171 *afe) : m_afe(afe)
{
    clockController = new FlapoMax86171ClockController(afe);
    for (int i = 0; i < 9; i++)
    {
        measurementController[i] = new FlapoMax86171MeasurementController(afe, i + 1);
    }
}

FlapoMax86171Controller::~FlapoMax86171Controller()
{
    delete clockController;
    for (int i = 0; i < 9; i++)
    {
        delete measurementController[i];
    }
}

FlapoMax86171MeasurementController *FlapoMax86171Controller::getMeasurementController(int measurement_nr)
{
    if (measurement_nr < 1 || measurement_nr > 9)
    {
        return nullptr;
    }
    return measurementController[measurement_nr - 1];
}

FlapoMax86171ClockController *FlapoMax86171Controller::getClockController()
{
    return clockController;
}

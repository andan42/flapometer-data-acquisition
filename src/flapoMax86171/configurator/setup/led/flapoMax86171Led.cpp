#include "flapoMax86171Led.h"

#include "flapoMax86171/configurator/measurement/flapoMax86171MeasurementController.h"
#include "flapoMax86171/configurator/utils/flapoMax86171Utils.h"

FlapoMax86171Led::FlapoMax86171Led(FlapoMax86171MeasurementController *measController, int pin, int intensity)
{
    m_measController = measController;
    m_pin = pin;
    m_intensity = intensity;
    driveRegister =  flapoMax86171Utils::getDriveRegister(pin);
    driveValue = flapoMax86171Utils::getDriveValueAuto(pin, driveRegister);
}

void FlapoMax86171Led::setIntensity()
{
    if(driveRegister == 'A')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA_PA, m_intensity);
    }
    else if(driveRegister == 'B')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVB_PA, m_intensity);
    }
    else if(driveRegister == 'C')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVC_PA, m_intensity);
    }
}

void FlapoMax86171Led::init()
{
    //meas enable
    m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_EN, 1);
    //pick proper drive
    if(driveRegister == 'A')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVA, driveValue);
    }
    else if(driveRegister == 'B')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVB, driveValue);
    }
    else if(driveRegister == 'C')
    {
        m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_DRVC, driveValue);
    }
    //set intensity
    setIntensity();
}

void FlapoMax86171Led::updateIntensity(int intensity)
{
    m_intensity = intensity;
    setIntensity();
}

int FlapoMax86171Led::getIntensity() const
{
    return m_intensity;
}

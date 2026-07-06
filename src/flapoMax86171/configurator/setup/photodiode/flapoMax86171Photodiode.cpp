#include "flapoMax86171Photodiode.h"

#include "flapoMax86171/configurator/measurement/flapoMax86171MeasurementController.h"

FlapoMax86171PhotodiodePair::FlapoMax86171PhotodiodePair(FlapoMax86171MeasurementController* measController, int ppgSelect[2])
{
    m_measController = measController;
    m_ppgSelect[0] = ppgSelect[0];
    m_ppgSelect[1] = ppgSelect[1];
}

void FlapoMax86171PhotodiodePair::init()
{
    m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_PPG1_PDSEL, m_ppgSelect[0]);
    m_measController->writeRegister(flapoRegistersSpectro::MEAS_N_PPG2_PDSEL, m_ppgSelect[1]);
}
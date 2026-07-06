#pragma once

class FlapoMax86171MeasurementController;

class FlapoMax86171PhotodiodePair
{
private:
    FlapoMax86171MeasurementController* m_measController;
    int m_ppgSelect[2];
    
public:
    FlapoMax86171PhotodiodePair(FlapoMax86171MeasurementController* measController, int ppgSelect[2]);
    void init();
};
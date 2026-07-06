#pragma once
#include "flapoSensors/flapoPoll.h"



class FlapoMax86171;

class FlapoDataPollSpectro : public FlapoDataPoll
{
private:
    FlapoMax86171 *m_flapoWrapperSpectro;
    
public:
    FlapoDataPollSpectro(FlapoMax86171 *flapoWrapperSpectro);
    ~FlapoDataPollSpectro();
    FlapoRawData* poll(FlapoSensor *polledSensor) override;
};
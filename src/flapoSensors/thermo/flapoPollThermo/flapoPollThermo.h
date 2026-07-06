#pragma once
#include "flapoSensors/flapoPoll.h" //src/flapoSensors/flapoPoll.h



class FlapoPcf8591;

class FlapoDataPollThermo : public FlapoDataPoll
{
private:
    FlapoPcf8591 *m_flapoWrapperThermo;

public:
    FlapoDataPollThermo(FlapoPcf8591 *flapoWrapperThermo);
    ~FlapoDataPollThermo();
    FlapoRawData* poll(FlapoSensor *polledSensor) override;
};
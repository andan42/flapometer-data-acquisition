#pragma once
#include "flapoData/flapoRawData/flapoRawData.h"

class FlapoRawDataPollCancelled : public FlapoRawData
{
private:
    const char *m_message = nullptr;
public:
    FlapoRawDataPollCancelled(FlapoSensor *polledSensor = nullptr, const char *message = "No cause specified.");
    ~FlapoRawDataPollCancelled() override;
    bool messageIs(const char *message) const;
};

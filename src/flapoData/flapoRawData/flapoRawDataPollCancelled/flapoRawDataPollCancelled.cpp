#include "flapoRawDataPollCancelled.h"
#include <cstring>

FlapoRawDataPollCancelled::FlapoRawDataPollCancelled(FlapoSensor *polledSensor, const char *message)
 : FlapoRawData(polledSensor) //TODO fix this, it usually passes a god damn null pointer here
{
    m_message = message;
}

FlapoRawDataPollCancelled::~FlapoRawDataPollCancelled()
{
    //nothing to do
}

bool FlapoRawDataPollCancelled::messageIs(const char *message) const
{
    //strcmp
    return std::strcmp(m_message, message) == 0;
}

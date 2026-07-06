#include "flapoRawData.h"
#include "flapoSensors/flapoSensor.h"

std::chrono::steady_clock::time_point FlapoRawData::getTimePolled() const
{
    return m_timePolled;
}
std::chrono::steady_clock::time_point FlapoRawData::getPreviousTimePolled() const
{
    return m_previousTimePolled;
}
const FlapoSensor *FlapoRawData::getPolledSensor() const
{
    return m_polledSensor;
}
FlapoRawData::FlapoRawData(FlapoSensor *polledSensor) : m_timePolled(std::chrono::steady_clock::now()),
                                                        m_previousTimePolled(polledSensor ? polledSensor->getLastTimePolled() : std::chrono::steady_clock::time_point{}), // if polled sensor is null and deleted, there will be no subsequent polls so the time doesnt matter
                                                        m_polledSensor(polledSensor),
                                                        dataCount(0),
                                                        data(nullptr)
{
    if(polledSensor)
    {
        polledSensor->setLastTimePolled(m_timePolled);
    }
}

FlapoRawData::~FlapoRawData()
{
    if (data != nullptr)
    {
        delete[] data;
    }
}

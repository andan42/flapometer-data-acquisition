#pragma once
#include <chrono>

class FlapoSensor; //forward declarations needed
class FlapoProcessedData; //forward declarations needed

class FlapoRawData
{
private:
    std::chrono::steady_clock::time_point m_timePolled;
    const std::chrono::steady_clock::time_point m_previousTimePolled; //this is needed for the FlapoProcessedData class, in order to set a start and end time for the samples
    const FlapoSensor *m_polledSensor;
public:
    std::chrono::steady_clock::time_point getTimePolled() const;
    std::chrono::steady_clock::time_point getPreviousTimePolled() const;
    const FlapoSensor *getPolledSensor() const;
    int dataCount;
    int *data;

    FlapoRawData(FlapoSensor *polledSensor);

    virtual ~FlapoRawData();
};
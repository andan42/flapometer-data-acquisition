#pragma once
#include "flapoSensors/flapoPoll.h"
#include "flapoSensors/flapoProcess.h"

#include "flapoMetadata/flapoSensorMetadata/flapoSensorMetadata.h"

#include <memory>
#include <vector>

class FlapoSensor
{
private:
    FlapoSensorMetadata m_sensorMetadata;
    std::chrono::steady_clock::time_point m_lastTimePolled;
    //const char * const sensorLabel;
public:
    void setLastTimePolled(std::chrono::steady_clock::time_point timePolled);
    FlapoSensor(const std::string& sensorLabel, FlapoOperationMetadata operation, const FlapoSensorTypeMetadata sensorType);
    virtual ~FlapoSensor(); //might need this
    //const char * getSensorLabel() const;
    virtual void setSensorMetadataIntensities(const std::vector<int> intensities) = 0;
    const FlapoSensorMetadata getSensorMetadata() const;
    std::chrono::steady_clock::time_point getLastTimePolled() const;

    FlapoDataPoll* poller; //each sensor has a unique poller based on its configuration
    //FlapoDataProcess* processor; //sensors of a certain subtype might not need individualized processors (eg thermistors are all identical)


    //virtual poll
    //virtual process
    virtual FlapoRawData* poll() = 0;
    virtual std::vector<std::shared_ptr<const FlapoProcessedData>> process(const FlapoRawData* rawData) const = 0;
};
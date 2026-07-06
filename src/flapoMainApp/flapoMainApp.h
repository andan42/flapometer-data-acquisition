#pragma once

#include "i2c/DeviceI2C.h"
#include "flapoMax86171/configurator/flapoMax86171Controller.h"
#include "flapoPcf8591/chip/flapoPcf8591.h"


#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"
#include "flapoSensors/thermo/flapoSensorThermo/flapoSensorThermo.h"

#include "flapoSensors/spectro/flapoPollSpectro/flapoPollSpectro.h"
#include "flapoSensors/thermo/flapoPollThermo/flapoPollThermo.h"

#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"
#include "flapoSensors/thermo/flapoProcessThermo/flapoProcessThermo.h"


#include "flapoClient/flapoRequester/flapoRequester.h"
#include "flapoConsumer/flapoClientConsumer/flapoClientConsumer.h"
// #include "flapoConsumer/flapoAdjusterConsumer/flapoAdjusterConsumer.h"
#include "flapoConsumer/flapoSaveConsumer/flapoSaveConsumer.h"

#include "flapoQueues/flapoRawDataQueue/flapoRawDataQueue.h"
#include "flapoQueues/flapoProcessedDataQueue/flapoProcessedDataQueue.h"

#include "flapoThread/flapoPollThread/flapoPollThread.h"
#include "flapoThread/flapoProcessingThread/flapoProcessingThread.h"
#include "flapoThread/flapoConsumeThread/flapoConsumeThread.h"

#include "flapoMax86171/configurator/setup/flapoMax86171Setup.h"
#include "flapoMainApp/spectroManager/flapoSpectroManager.h"
#include "flapoMainApp/thermoManager/flapoThermoManager.h"

#include "flapoClient/flapoRequester/flapoRequester.h"

//todo remove
#include <iostream>

class FlapoMainApp
{
private:
    FlapoRequester *requester;

    FlapoSpectroManager *spectroManagerA;
    std::string sensorSpectroLabelA;

    //FlapoSpectroManager *spectroManagerB;
    //std::string sensorSpectroLabelB;

    //FlapoThermoManager *thermoManager;
    //std::string sensorThermoLabel;

    FlapoOperationMetadata operationMetadata;

    FlapoClientConsumer *clientConsumer;
    FlapoSaveConsumer *saveConsumer;
    //FlapoAdjusterConsumer* adjusterConsumer; //currently disabled xd

    FlapoRawDataQueue* rawDataQueue;

    FlapoProcessedDataQueue* processedDataQueueClient;
    FlapoProcessedDataQueue *processedDataQueueSave;
    // FlapoProcessedDataQueue* processedDataQueueAdjuster;

    FlapoPollThread *primaryPollThread;
    FlapoPollThread *secondaryPollThread;
    FlapoProcessingThread* processingThread;

    FlapoConsumeThread *clientConsumeThread;
    FlapoConsumeThread *saveConsumeThread;
    //FlapoConsumeThread *adjusterConsumeThread;
public:
    FlapoMainApp(const std::string &url, const std::string &opLabel, const std::string &patientName);
    ~FlapoMainApp();
};
#include "flapoMainApp/flapoMainApp.h"
#include "flapoUtils/flapoLastDatabaseTime/flapoLastDatabaseTime.h"
#include "flapoUtils/flapoReferenceTime/flapoReferenceTime.h"

FlapoMainApp::FlapoMainApp(const std::string &url, const std::string &opLabel, const std::string &patientName)
    : operationMetadata(opLabel, patientName)
{
    requester = new FlapoRequester(url);

    int lastDatabaseTime = getLastDatabaseTime(requester, patientName);
    setReferenceTimeOffset(lastDatabaseTime);
    std::cout << "Last database time: " << lastDatabaseTime << std::endl;

    sensorSpectroLabelA = "sensor_spectro_1";
    //sensorSpectroLabelB = "sensor_spectro_2";
    //sensorThermoLabel = "sensor_thermo_1";

    // spectroA
    spectroManagerA = new FlapoSpectroManager(4, sensorSpectroLabelA, operationMetadata); // bus 4 is faster with i2c operation failures for no apparent reason.
    //spectroManagerB = new FlapoSpectroManager(6, sensorSpectroLabelB, operationMetadata);

    // thermo
    //thermoManager = new FlapoThermoManager(sensorThermoLabel, operationMetadata);

    clientConsumer = new FlapoClientConsumer(requester);
    saveConsumer = new FlapoSaveConsumer("/home/pi/flapodata/test/" + opLabel);

    // create queues for raw and processed data
    rawDataQueue = new FlapoRawDataQueue();

    processedDataQueueClient = new FlapoProcessedDataQueue();
    processedDataQueueSave = new FlapoProcessedDataQueue();

    // create threads
    primaryPollThread = new FlapoPollThread(rawDataQueue, std::chrono::milliseconds(200));
    secondaryPollThread = new FlapoPollThread(rawDataQueue, std::chrono::milliseconds(200));

    // sensorA poller is on bus 4 which does not lock up, which is why its being grouped with the thermpoller
    primaryPollThread->sensors.push_back(spectroManagerA->sensorClass);
    //primaryPollThread->sensors.push_back(thermoManager->sensorClass);

    // sensor B poller is on bus 6 which locks up (when it doesnt have devices), so it gets its own poller
    //secondaryPollThread->sensors.push_back(spectroManagerB->sensorClass);

    processingThread = new FlapoProcessingThread(rawDataQueue);
    processingThread->addProcessedDataQueue(processedDataQueueClient);
    processingThread->addProcessedDataQueue(processedDataQueueSave);

    clientConsumeThread = new FlapoConsumeThread(clientConsumer, processedDataQueueClient, std::chrono::milliseconds(2000));
    saveConsumeThread = new FlapoConsumeThread(saveConsumer, processedDataQueueSave, std::chrono::milliseconds(10000)); // 20 seconds

    // unpause chip!! this line has been forgotten this many times: x x NOT THIS TIME
    spectroManagerA->start();
    //spectroManagerB->start();
}

FlapoMainApp::~FlapoMainApp()
{
    spectroManagerA->stop();
    spectroManagerA->reset();
    //spectroManagerB->stop();
    //spectroManagerB->reset();

    //delete threads
    delete primaryPollThread;
    delete secondaryPollThread;
    delete processingThread;
    delete clientConsumeThread;
    delete saveConsumeThread;

    //delete queues
    delete rawDataQueue;
    delete processedDataQueueClient;
    delete processedDataQueueSave;

    //delete sensor pipeline
    delete spectroManagerA;
    //delete spectroManagerB;

    //delete sensor pipeline, cant believe i forgot this, thanks copilot <3
    //delete thermoManager;

    //delete consumer. this has to be done after deleting the queue, iirc the queue calls one last consume on the consumer
    delete clientConsumer;
    delete saveConsumer;

    delete requester;
}

// void FlapoMainApp::chipSetupSpectro()
// {
//     //pause and reset chip
//     controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 1);
//     controller->getAfe()->writeRegister(flapoRegistersSpectro::RESET, 1);

//     //disable a photodiode (affects all measurements)
//     //controller->getAfe()->writeRegister(flapoRegistersSpectro::PPG2_PWRDN, 1);
//     //this disables PPG2 which is PD2 and PD4 (depending on PDSEL)

//     //set up measurements
//     //7=550nm; 6=600nm; 5=650nm; 8=700nm; 1=750nm; 9=800nm; 4=850nm; 3=900nm; 2=950nm
//     int pins[] = {7, 6, 5, 8, 1, 9, 4, 3, 2};
//     int intensities[] = {0xA0, 0x10, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08}; //550 nm needs more light
//     //pins 1 2 4 5 7 correspond to PD1 and PD3 => photodiodes[][] = {1,1}
//     //pins 3 6 8 9 correspond to PD2 and PD4 => photodiodes[][] = {0,0}

//     //int photodiodes[9][2] = { {0,0},{1,1},{0,0},{1,1},{0,0},{1,1},{0,0},{1,1},{0,0}} ;
//     //using nullptr automatically selects the right photodiodes
//     // {0,0} is PD1 for PPG1 and PD2 for PPG2 -> this should be used in measurements that use leds with  550   650   750   850   950  wavelengths
//     // {1,1} is PD3 for PPG1 and PD4 for PPG2 -> this should be used in measurements that use leds with     600   700   800   900     wavelengths
//     // {0,1} is PD1 for PPG1 and PD4 for PPG2
//     // {1,0} is PD3 for PPG1 and PD2 for PPG2
//     // PPG2 is disabled, so only PD1 and PD3 are used. second number is ignored.

//     setup = new FlapoMax86171Setup(controller, 9, pins, intensities, nullptr); //not a memory leak. this gets deleted in the destructor, and only created in constructor
//     setup->init();

//     //set up clock
//     controller->getClockController()->setClockFrequency(25.0, 0);

//     //for debugging: dump all registers:
//     if(false)
//     {
//         uint8_t afeRegisterDump[0x80]; //could be 7F but easier to loop this
//         controller->getAfe()->dumpRegisters(afeRegisterDump);
//         std::cout << "      0x00 0x01 0x02 0x03 0x04 0x05 0x06 0x07 0x08 0x09 0x0A 0x0B 0x0C 0x0D 0x0E" << std::endl;
//         std::cout << "      --------------------------------------------------------------------------" << std::endl;
//         std::cout << "0"; //whitespace to align
//         for(int i = 0; i < 8; i++)
//         {
//             std::cout << std::hex << i * 0x10 << " : ";
//             for(int j = 0; j < 0xF; j++)
//             {
//                 int index = i * 0x10 + j;
//                 int val = (int)afeRegisterDump[index];
//                 if(val < 0x10)
//                     std::cout << " 0x0" << std::hex << val;
//                 else
//                     std::cout << " 0x"<< std::hex << val;
//             }
//             std::cout << std::endl;
//         }
//         std::cout << "      --------------------------------------------------------------------------" << std::endl;
//     }
// }

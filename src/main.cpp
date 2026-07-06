#include <iostream>

#include <bitset>
#include <string.h>


#include <chrono>
#include <thread>

#include "flapoMainApp/flapoMainApp.h"
#include "flapoPcf8591/chip/flapoPcf8591.h"

#include "i2c/DeviceI2C.h"
#include "flapoData/flapoRawData/flapoRawDataThermo/flapoRawDataThermo.h"
#include "flapoSensors/thermo/flapoPollThermo/flapoPollThermo.h"

#include <iostream>
#include <ctime>
#include <string>
#include <sstream>
#include <cstring>

void readChannel(int channel)
{
    DeviceI2C deviceI2C(0x48, 1);
    FlapoPcf8591 flapoPcf8591(&deviceI2C);
    int samplecount = 10;
    int data[samplecount];

    flapoPcf8591.writeControl(true, 0b00, false, channel);
    flapoPcf8591.readDummy();
    for (int i = 0; i < samplecount; i++)
    {
        data[i] = flapoPcf8591.read();
    }
    for (int i = 0; i < samplecount; i++)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

int main(int argc, char **argv)
{
    //--serverAddress
    const char *serverAddress = nullptr;
    // int bus = -1;
    // int i2cAddress = -1;
    const char *operationName = nullptr;
    const char *patientName = nullptr;
    int runTime = 10;
    for (int i = 0; i < argc; i++)
    {
        if (strcmp(argv[i], "--serverAddress") == 0)
        {
            serverAddress = argv[i + 1];
        }
        // if (strcmp(argv[i], "--bus") == 0)
        // {
        //     bus = std::stoi(argv[i + 1]);
        // }
        // if (strcmp(argv[i], "--i2cAddress") == 0)
        // {
        //     // addres is in hex
        //     i2cAddress = std::stoi(argv[i + 1], nullptr, 16);
        // }
        if (strcmp(argv[i], "--operationName") == 0)
        {
            operationName = argv[i + 1];
        }
        if (strcmp(argv[i], "--patientName") == 0)
        {
            patientName = argv[i + 1];
        }
        if (strcmp(argv[i], "--runTime") == 0)
        {
            runTime = std::stoi(argv[i + 1]);
        }
    }
    if (serverAddress == nullptr)
    {
        std::cout << "No server address provided. Exiting." << std::endl;
        return 1;
    }
    bool operationNameProvided = true;
    if (operationName == nullptr)
    {
        operationNameProvided = false;
        // Get the current time
        std::time_t now = std::time(nullptr);
        std::tm *now_tm = std::localtime(&now);

        // Format the date and time
        std::ostringstream oss;
        oss << "test_"
            << (now_tm->tm_year + 1900) << "-"
            << (now_tm->tm_mon + 1) << "-"
            << now_tm->tm_mday << "_"
            << now_tm->tm_hour << "-"
            << now_tm->tm_min << "-"
            << now_tm->tm_sec;

        // Convert to string and assign to operationName
        std::string formattedName = oss.str();
        char *dynamicOperationName = new char[formattedName.length() + 1];
        std::strcpy(dynamicOperationName, formattedName.c_str());
        operationName = dynamicOperationName;
    }
    else //append date anyway so we dont have duplicates
    {
        //operation name provided is now true this is a vestige and might make a mmory leak TODO
        // Get the current time
        std::time_t now = std::time(nullptr);
        std::tm *now_tm = std::localtime(&now);

        // Format the date and time
        std::ostringstream oss;
        oss << operationName << "_"
            << (now_tm->tm_year + 1900) << "-"
            << (now_tm->tm_mon + 1) << "-"
            << now_tm->tm_mday << "_"
            << now_tm->tm_hour << "-"
            << now_tm->tm_min << "-"
            << now_tm->tm_sec;

        // Convert to string and assign to operationName
        std::string formattedName = oss.str();
        char *dynamicOperationName = new char[formattedName.length() + 1];
        std::strcpy(dynamicOperationName, formattedName.c_str());
        operationName = dynamicOperationName;
    }
    // if (patientName == nullptr)
    // {
    //     // use "test_patient" instead
    //     patientName = "test_patient";
    // }
    std::cout << "Starting..." << std::endl;
    FlapoMainApp app(serverAddress, operationName, patientName);
    // run time sleep duration varaible
    std::chrono::seconds sleepDuration(runTime);
    std::this_thread::sleep_for(sleepDuration);
    std::cout << "Exiting." << std::endl;
    //if (operationNameProvided == false)
    if(true) //see other comment. vestige.s
    {
        delete[] operationName;
    }
    return 0;
}

// <3 <3 <3 <3
// <3 <3 <3 <3
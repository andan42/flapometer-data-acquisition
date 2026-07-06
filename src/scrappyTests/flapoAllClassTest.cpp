/*#include <iostream>
#include <string>
#include <string.h>

#include "i2c/DeviceI2C.h"
#include "flapoMax86171/configurator/flapoMax86171Controller.h"


int testFlapoAllClass(int busNumber, int address)
{
    std::cout << "k" << std::endl;
    //afe
    DeviceI2C *device = new DeviceI2C(address, busNumber);
    FlapoMax86171 *afe = new FlapoMax86171(device);
    FlapoMax86171Controller *controller = new FlapoMax86171Controller(afe);

    //create poller processor and 



    delete controller;
    delete afe;
    delete device;

    return 0;
}


int main(int argc, char **argv)
{
    std::string url = "invalid";
    int address = -1;
    int busNumber = -1;
    for (int i = 0; i < argc; i++)
    {
        //if arg[i] matches, set value to arg[i + 1]
        if (strcmp(argv[i], "url") == 0)
        {
            url = argv[i + 1];
        }
        if(strcmp(argv[i], "address") == 0)
        {
            address = std::stoi(argv[i + 1]);
        }
        if(strcmp(argv[i], "bus") == 0)
        {
            busNumber = std::stoi(argv[i + 1]);
        }
    }
    if (url == "invalid")
    {
        std::cout << "Invalid url argument, please provide url in test/args file" << std::endl;
        return 1;
    }
    if (address == -1 || busNumber == -1)
    {
        std::cout << "flapoAllClassTest.cpp main() test failed. Make sure address is set!" << std::endl;
        return 1;
    }

    testFlapoAllClass(busNumber, address);



    return 0;
}*/
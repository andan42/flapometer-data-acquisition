#pragma once

class DeviceI2C;
class FlapoPcf8591
{   
private:
    DeviceI2C *m_deviceI2C;
public:
    FlapoPcf8591(DeviceI2C *deviceI2C);
    ~FlapoPcf8591();
    int read();
    void readDummy();
    void writeControl(bool outputEnabled, char inputProgramming, bool autoIncrement, char channel);
    DeviceI2C *getDeviceI2C() //see if needed TODO
    {
        return m_deviceI2C;
    }
        
};
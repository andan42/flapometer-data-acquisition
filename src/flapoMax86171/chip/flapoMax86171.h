#pragma once
#include "flapoMax86171/constants/flapoMax86171Registers.h"
#include <stdint.h>

#include <list>
struct flapoMax86171Command
{
    const flapoRegister Reg;
    const int Val;
};

class DeviceI2C;
class FlapoMax86171
{
private:
    DeviceI2C *m_deviceI2C;
    static const int AUTOMATICALLY_RECOVER_AFTER = 200;
    int m_recoverCounter = 0;
    bool m_pauseRecoveryCounter = false;

public:
    FlapoMax86171(DeviceI2C *deviceI2C);
    ~FlapoMax86171();
    int readRegister(flapoRegister reg, int *val);
    int multiReadRegister(flapoRegister reg, int count, uint8_t *val);
    int writeRegister(flapoRegister reg, int val);

    void dumpRegisters(uint8_t *out);
    uint8_t savedDump[0x80];
    bool hasValidDump = true;

    DeviceI2C *getDeviceI2C() //see if needed TODO
    {
        tryRecovery();
        return m_deviceI2C;
    }

    std::list<flapoMax86171Command> m_commands;
    bool m_captureCommands = false;
    void startCaptureCommands();
    void stopCaptureCommands();
    void addCommand(flapoRegister reg, int val);
    void replayCommands();
    bool verifyCommands();

    bool shouldTryRecovery = false;
    bool needsRecovery = false;
    void markForRecovery();
    void markRecovered();
    void tryRecovery();

private:
    
};
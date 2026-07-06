#include "flapoMax86171/chip/flapoMax86171.h"
//#include "flapoMax86171/constants/flapoMax86171Registers.h"
#include "i2c/DeviceI2C.h"
#include "strings.h"

#include <iostream>
#include <thread> // Include this header for std::this_thread
#include "flapoMax86171.h"
#include <bitset>

FlapoMax86171::FlapoMax86171(DeviceI2C *deviceI2C)
{
    m_deviceI2C = deviceI2C;
}

FlapoMax86171::~FlapoMax86171()
{
}

int FlapoMax86171::writeRegister(flapoRegister reg, int val)
{
    // commands
    addCommand(reg, val);

    //shift val to reg.bitfield
    val = val << (ffs(reg.BitField) - 1);
    //check if val fits in reg.bitfield
    if(val != (val & reg.BitField))
    {
        std::cout << "writeRegister(" << reg.Address << ", " << val << ")" << std::endl;
        std::cout << std::hex << "failed, val does not fit in bitfield:" << reg.BitField << std::endl;
        this->markForRecovery();
        return -1;
    }
    //read old value at address
    int old_val = m_deviceI2C->readByte(reg.Address);
    if(old_val < 0)
    {
        std::cout << "writeRegister(" << reg.Address << ", ..) failed with error: " << old_val << std::endl;
        this->markForRecovery();
        return old_val;
    }
    //overwrite bitfield with val
    int new_val = (old_val & ~reg.BitField) | val;
    //write new val to address
    int ret = m_deviceI2C->writeByte(reg.Address, new_val);
    return ret;
}

int FlapoMax86171::readRegister(flapoRegister reg, int *val)
{
    tryRecovery();
    //read value at address
    int ret = m_deviceI2C->readByte(reg.Address);
    if (ret < 0)
    {
        std::cout << std::hex << "readRegister(" << reg.Address << ", ..) failed" << std::endl;
        this->markForRecovery();
        *val = 0;
        return ret;
    }
    //mask out bitfield
    *val = ret & reg.BitField;
    //shift val to right
    *val = *val >> (ffs(reg.BitField) - 1);
    return 0;
}

int FlapoMax86171::multiReadRegister(flapoRegister reg, int count, uint8_t *val)
{
    tryRecovery();
    int ret = m_deviceI2C->readBytes(reg.Address, count, (uint8_t *)val);
    if (ret < 0)
    {
        std::cout << std::hex << "multiReadRegister(" << reg.Address << ", ..) failed" << std::endl;
        this->markForRecovery();
        return ret;
    }
    return 0;
}

void FlapoMax86171::dumpRegisters(uint8_t *out)
{
    for(int i=0; i <= 0x7E; i++)
    {
        out[i] = m_deviceI2C->readByte(i);
    }
}

void FlapoMax86171::startCaptureCommands()
{
    m_captureCommands = true;
    // clear command array. not necessary but good practice?
    m_commands.clear();
}

void FlapoMax86171::stopCaptureCommands()
{
    m_captureCommands = false;
    // save a dump of the registers for integrity check on replay
    if (hasValidDump)
    {
        dumpRegisters(savedDump);
    }
}

void FlapoMax86171::addCommand(flapoRegister reg, int val)
{
    if (!m_captureCommands)
    {
        return;
    }
    if (reg == flapoRegistersSpectro::SHDN) // shd and reset do not contribute to device configuration
    {
        return;
    }
    if (reg == flapoRegistersSpectro::RESET)
    {
        return;
    }
    flapoMax86171Command command = {reg, val};
    m_commands.push_back(command);
}

void FlapoMax86171::replayCommands()
{
    writeRegister(flapoRegistersSpectro::SHDN, 1);
    writeRegister(flapoRegistersSpectro::RESET, 1); // pause
    for (flapoMax86171Command command : m_commands) // replay all writeRegisters
    {
        writeRegister(command.Reg, command.Val);
    }
    // check integrity of chip after replay. first with dump
    // if (hasValidDump) // only possible if valid dump was saved
    // {
    //     uint8_t postReplayDump[0x80];
    //     dumpRegisters(postReplayDump);
    //     for (int i = 0; i <= 0x7E; i++)
    //     {
    //         if (postReplayDump[i] != savedDump[i])
    //         {
    //             std::cout << std::hex << "replayCommands dumpcheck failed, register " << i << " does not match" << std::endl;
    //             this->needsRecovery = true;
    //             return;
    //         }
    //     }
    // }

    // then with readRegisters. hoenstly might just do only readRegisters
    for (flapoMax86171Command command : m_commands)
    {
        int val;
        readRegister(command.Reg, &val);
        if (val != command.Val)
        {
            std::cout << std::hex << "replayCommands readcheck failed, register with address " << command.Reg.Address << " and mask " << std::bitset<8>(command.Reg.BitField) << "does not match" << std::endl;
            this->needsRecovery = true;
            writeRegister(flapoRegistersSpectro::SHDN, 1);
            return;
        }
    }

    markRecovered();
    writeRegister(flapoRegistersSpectro::SHDN, 0); // replay
    std::cout << "max86171 bus recovery successful for bus " << m_deviceI2C->m_busNumber << " address " << m_deviceI2C->m_address << std::endl;
}

bool FlapoMax86171::verifyCommands()
{
    for (flapoMax86171Command command : m_commands)
    {
        int val;
        readRegister(command.Reg, &val);
        if (val != command.Val)
        {
            std::cout << std::hex << "replayCommands readcheck failed, register with address " << command.Reg.Address << " and mask " << std::bitset<8>(command.Reg.BitField) << "does not match" << std::endl;
            this->needsRecovery = true;
            writeRegister(flapoRegistersSpectro::SHDN, 1);
            return false;
        }
    }
    return true;
}

void FlapoMax86171::tryRecovery()
{
    if (!shouldTryRecovery)
    {
        shouldTryRecovery = false;
        m_recoverCounter++;
        if (m_recoverCounter > AUTOMATICALLY_RECOVER_AFTER)
        {
            m_recoverCounter = 0;
            if (!verifyCommands())
            {
                m_pauseRecoveryCounter = true;
                std::cout << "detected config incongruency error, trying recovery" << std::endl;
                markForRecovery();
            }
        }
        return;
    }
    shouldTryRecovery = false;
    // try to read byte from i2c device to see if reconnected
    int val;
    bool ret = m_deviceI2C->isOk();
    if (!ret)
    {
        m_deviceI2C->searchDevices();
        return;
    }
    // try 3 more addressless reads to make sure connection is stable. 2ms delay between reads
    for (int i = 0; i < 3; i++)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        // ret = m_deviceI2C->readAddresslessByte();
        if (!m_deviceI2C->isOk())
            return;
    }
    replayCommands();
    shouldTryRecovery = needsRecovery;
}

void FlapoMax86171::markForRecovery()
{
    needsRecovery = true;
    shouldTryRecovery = true;
    m_pauseRecoveryCounter = true;
    if (m_captureCommands)
    {
        hasValidDump = false; // prevent saving a bad initial dump, in case of bad writes during setup.
    }
}

void FlapoMax86171::markRecovered()
{
    needsRecovery = false;
    shouldTryRecovery = false;
    m_pauseRecoveryCounter = false;
}

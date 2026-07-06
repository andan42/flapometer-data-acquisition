#include "flapoMax86171ClockController.h"

#include <iostream>

FlapoMax86171ClockController::FlapoMax86171ClockController(FlapoMax86171 *afe) : m_afe(afe)
{
}

FlapoMax86171ClockController::~FlapoMax86171ClockController()
{
}



int FlapoMax86171ClockController::setClockFrequency(double frequency, int source)
{
    int ret = 0;
    double sourceFrequency = (source == 0) ? 32000.0 : 32768.0;
    //check if frequency is in range for given source
    if (frequency < 0 || frequency > (sourceFrequency))
    {
        std::cout << "setClockFrequency(" << frequency << "): frequency out of range for source" << source << std::endl;
        return -1;  
    }
    //calculate divider
    int divider = (int)(sourceFrequency / frequency);

    //TODO: we should pick the closest frequency to the desired one rather than rounding up (frequency = sourceFrequency / divider)

    //split in bytes
    int dividerH = (divider >> 8) & 0xFF;
    int dividerL = divider & 0xFF;
    //write and check return
    ret = m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_DIV_H, dividerH);
    if (ret < 0)
    {
        std::cout << "setClockFrequency(" << frequency << "): error writing dividerH:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_DIV_L, dividerL);
    if (ret < 0)
    {
        std::cout << "setClockFrequency(" << frequency << "): error writing dividerL:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    //write and check return
    ret = m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_SEL, source);
    if (ret < 0)
    {
        std::cout << "setClockFrequency(" << frequency << "): error writing source:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    return 0;
}

double FlapoMax86171ClockController::getActualFrequency()
{
    int ret = 0;
    //read divider
    int dividerH = 0;
    int dividerL = 0;
    ret = m_afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_H, &dividerH);
    if (ret < 0)
    {
        std::cout << "getActualFrequency(): error reading dividerH:" << ret << "Caused by readRegister()" << std::endl;
        return ret;
    }
    ret = m_afe->readRegister(flapoRegistersSpectro::FR_CLK_DIV_L, &dividerL);
    if (ret < 0)
    {
        std::cout << "getActualFrequency(): error reading dividerL:" << ret << "Caused by readRegister()" << std::endl;
        return ret;
    }
    //combine bytes
    int divider = (dividerH << 8) | dividerL;
    //read clock select
    int source = 0;
    ret = m_afe->readRegister(flapoRegistersSpectro::FR_CLK_SEL, &source);
    if (ret < 0)
    {
        std::cout << "getActualFrequency(): error reading source:" << ret << "Caused by readRegister()" << std::endl;
        return ret;
    }
    //calculate frequency
    double frequency = 0;
    if (source == 0)
    {
        frequency = 32000.0 / divider;
    }
    else if (source == 1)
    {
        frequency = 32768.0 / divider;
    }
    /*else
    {
        std::cout << "getActualFrequency(): error reading source:" << ret << "Caused by readRegister()" << std::endl;
        return -1.0;
    }*/ //if this happens were fucked way before
    //normal return
    return frequency;
}

int FlapoMax86171ClockController::setClockDivider(int divider)
{
    int ret = 0;
    //check if divider is in range
    if (divider < 0 || divider > 0x7FFF)
    {
        std::cout << "setClockDivider(" << divider << "): divider out of range"   << std::endl;
        return -1;  
    }
    //split in bytes
    int dividerH = (divider >> 8) & 0xFF;
    int dividerL = divider & 0xFF;
    //write and check return
    ret = m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_DIV_H, dividerH);
    if (ret < 0)
    {
        std::cout << "setClockDivider(" << divider << "): error writing dividerH:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_DIV_L, dividerL);
    if (ret < 0)
    {
        std::cout << "setClockDivider(" << divider << "): error writing dividerL:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    //normal return
    return 0;
}

int FlapoMax86171ClockController::setClockSource(int source)
{
    int ret = 0;
    //check if source is in range
    if (source < 0 || source > 1)
    {
        std::cout << "setClockSource(" << source << "): source out of range"   << std::endl;
        return -1;  
    }
    //write and check return
    ret = m_afe->writeRegister(flapoRegistersSpectro::FR_CLK_SEL, source);
    if (ret < 0)
    {
        std::cout << "setClockSource(" << source << "): error writing source:" << ret << "Caused by writeRegister()" << std::endl;
        return ret;
    }
    //normal return
    return 0;
}


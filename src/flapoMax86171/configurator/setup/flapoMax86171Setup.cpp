#include "flapoMax86171Setup.h"

#include "flapoMax86171/configurator/flapoMax86171Controller.h"
#include "flapoMax86171/configurator/setup/led/flapoMax86171Led.h"
#include "flapoMax86171/configurator/setup/photodiode/flapoMax86171Photodiode.h"
#include "flapoMax86171/configurator/utils/flapoMax86171Utils.h"

FlapoMax86171Setup::FlapoMax86171Setup(FlapoMax86171Controller *controller, int ledNr, const int *pins, const int *intensities, const int (*selectedPhotodiodes)[2]) :
    m_controller(controller),
    m_nrLeds(ledNr)
{
    leds = new FlapoMax86171Led*[ledNr];
    photodiodes = new FlapoMax86171PhotodiodePair*[ledNr];
    if(intensities != nullptr)
    {
        for(int i = 0; i < ledNr; i++)
        {
            leds[i] = new FlapoMax86171Led(m_controller->getMeasurementController(i + 1), pins[i], intensities[i]);
        }
    }
    else
    {
        for(int i = 0; i < ledNr; i++)
        {
            leds[i] = new FlapoMax86171Led(m_controller->getMeasurementController(i + 1), pins[i], DEFAULT_INTENSITY);
        }
    }

    if(selectedPhotodiodes != nullptr)
    {

        for(int i = 0; i < ledNr; i++)
        {
            int temp[] = {selectedPhotodiodes[i][0], selectedPhotodiodes[i][1]};
            photodiodes[i] = new FlapoMax86171PhotodiodePair(m_controller->getMeasurementController(i + 1), temp);
        }
    }
    else
    {
        for(int i = 0; i < ledNr; i++)
        {
            //identify which photodiode is closest to the led source. this is gonna be the default way to use this function

            int temp[] = {flapoMax86171Utils::getPpg1Diode(pins[i]), flapoMax86171Utils::getPpg2Diode(pins[i])};
            photodiodes[i] = new FlapoMax86171PhotodiodePair(m_controller->getMeasurementController(i + 1), temp);
        }
    }
}

void FlapoMax86171Setup::init()
{
    int SHDN; //pause chip if not paused, then restore
    m_controller->getAfe()->readRegister(flapoRegistersSpectro::SHDN, &SHDN);
    if(SHDN == 0)
    {
        m_controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 1);
    }
    for(int i = 0; i < m_nrLeds; i++)
    {
        leds[i]->init();
        photodiodes[i]->init();
    }
    if(SHDN == 0)
    {
        m_controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 0);
    }
}

void FlapoMax86171Setup::updateIntensity(int ledNr, int intensity)
{
    leds[ledNr]->updateIntensity(intensity);
}

FlapoMax86171Setup::~FlapoMax86171Setup()
{
    for(int i = 0; i < m_nrLeds; i++)
    {
        delete leds[i];
    }
    for(int i = 0; i < m_nrLeds; i++)
    {
        delete photodiodes[i];
    }
    delete[] leds;
    delete[] photodiodes;
}

int FlapoMax86171Setup::getIntensity(int ledNr) const
{
    return leds[ledNr]->getIntensity();
}

std::vector<int> FlapoMax86171Setup::getIntensities() const
{
    std::vector<int> intensities;
    for(int i = 0; i < m_nrLeds; i++)
    {
        intensities.push_back(leds[i]->getIntensity());
    }
    return intensities;
}

void FlapoMax86171Setup::pauseChip()
{
    this->m_controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 1);
}

void FlapoMax86171Setup::clearChip()
{
    int temp;
    int fifo_data_count = 0;
    this->m_controller->getAfe()->readRegister(flapoRegistersSpectro::FIFO_DATA_COUNT_MSB, &temp);
    fifo_data_count = temp << 8;
    this->m_controller->getAfe()->readRegister(flapoRegistersSpectro::FIFO_DATA_COUNT_LSB, &temp);
    fifo_data_count |= temp;
    this->m_controller->getAfe()->readRegister(flapoRegistersSpectro::OVF_COUNTER, &temp);
}

void FlapoMax86171Setup::resumeChip()
{
    this->m_controller->getAfe()->writeRegister(flapoRegistersSpectro::SHDN, 0);
}

int FlapoMax86171Setup::getNrLeds() const
{
    return m_nrLeds;
}

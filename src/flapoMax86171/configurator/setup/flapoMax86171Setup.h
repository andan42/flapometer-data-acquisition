#pragma once
#include <vector>

class FlapoMax86171Controller;
class FlapoMax86171Led;
class FlapoMax86171PhotodiodePair;

class FlapoMax86171Setup
{
private:
    static const int DEFAULT_INTENSITY = 0x7F;
    FlapoMax86171Controller* m_controller;
    int m_nrLeds;
    FlapoMax86171Led** leds;
    FlapoMax86171PhotodiodePair** photodiodes;
public:
    /*
        intensities can be ommited and left to default intensities BUT THIS IS NOT RECOMMENDED (results in DEFAULT_INTENSITY for all leds)

        selectedPhotodiodes is a 2D array of size: nrLeds * 2. 
        IT IS recommended to leave this as nullptr, in which case the closest photodiodes to the leds will be selected automatically
        
        if you need to manually provide photodiodes, it is just an 2Darray of const int[nrLed][2] despite the weird syntax of const int (*const selectedPhotodiodes)[2]
        you can pass something like this:
        const int selectedPhotodiodes[3][2] = {{0, 0}, {1, 1}, {0, 1}}; noting there is only 2 photodiodes to pick from
        */
    FlapoMax86171Setup(FlapoMax86171Controller* controller, int nrLeds, const int* pins, const int* intensities = nullptr, const int (*const selectedPhotodiodes)[2] = nullptr);
    void init();
    void updateIntensity(int ledNr, int intensity);
    void pauseChip();
    void clearChip();
    void resumeChip();
    int getIntensity(int ledNr) const;
    std::vector<int> getIntensities() const;
    int getNrLeds() const;
    ~FlapoMax86171Setup();

};
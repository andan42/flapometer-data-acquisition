#pragma once

class FlapoMax86171MeasurementController;

class FlapoMax86171Led
{
private:
    FlapoMax86171MeasurementController* m_measController;
    int m_pin;  
    char driveRegister; //A, B or C
    int driveValue; //0-3
    int m_intensity;
    void setIntensity();
public:
    FlapoMax86171Led(FlapoMax86171MeasurementController* measController, int pin, int intensity = 0);
    void init();
    void updateIntensity(int intensity);
    int getIntensity() const;
};
#include "flapoMax86171/constants/flapoMax86171Registers.h"
//#include "flapoMax86171/constants/flapoMax86171AdressesMeasurement.h" idk why this gets recommended

#include <iostream>

void testMeasurementRegisterFunc()
{
    std::cout << "flapoRegistersSpectro::MEAS_1_2_BYTE_STEP " << flapoRegistersSpectro::MEAS_1_2_BYTE_STEP << std::endl;

    if(flapoRegistersSpectro::MEAS1_DRVA != flapoRegistersSpectro::MEAS_N_DRVA(1))
    {
        std::cout << "MEAS1_DRVA != MEAS_N_DRVA(1)" << std::endl;
    }
    if(flapoRegistersSpectro::MEAS2_DRVA != flapoRegistersSpectro::MEAS_N_DRVA(2))
    {
        std::cout << "MEAS2_DRVA != MEAS_N_DRVA(2)" << std::endl;// <3 <3 <3 <3
        std::cout << "MEAS2_DRVA(2).Address = " << flapoRegistersSpectro::MEAS_N_DRVA(2).Address << " =/= " << flapoRegistersSpectro::MEAS2_DRVA.Address << std::endl;
        std::cout << "MEAS2_DRVA(2).BitField = " << flapoRegistersSpectro::MEAS_N_DRVA(2).BitField << " =/= " << flapoRegistersSpectro::MEAS2_DRVA.BitField << std::endl;
    }
    if(flapoRegistersSpectro::MEAS1_DRVB != flapoRegistersSpectro::MEAS_N_DRVB(1))
    {
        std::cout << "MEAS1_DRVB != MEAS_N_DRVB(1)" << std::endl;
    }
    if(flapoRegistersSpectro::MEAS2_DRVB != flapoRegistersSpectro::MEAS_N_DRVB(2))
    {
        std::cout << "MEAS2_DRVB != MEAS_N_DRVB(2)" << std::endl;   // <3 <3 <3 <3
        std::cout << "MEAS2_DRVB(2).Address = " << flapoRegistersSpectro::MEAS_N_DRVB(2).Address << " =/= " << flapoRegistersSpectro::MEAS2_DRVB.Address << std::endl;
        std::cout << "MEAS2_DRVB(2).BitField = " << flapoRegistersSpectro::MEAS_N_DRVB(2).BitField << " =/= " << flapoRegistersSpectro::MEAS2_DRVB.BitField << std::endl;
    }
    if(flapoRegistersSpectro::MEAS1_DRVC != flapoRegistersSpectro::MEAS_N_DRVC(1))
    {
        std::cout << "MEAS1_DRVC != MEAS_N_DRVC(1)" << std::endl;
    }
    if(flapoRegistersSpectro::MEAS1_PPG1_DACOFF != flapoRegistersSpectro::MEAS_N_PPG1_DACOFF(1))
    {
        std::cout << "MEAS1_PPG1_DACOFF != MEAS_N_PPG1_DACOFF(1)" << std::endl;
    }
    if(flapoRegistersSpectro::MEAS2_PPG1_DACOFF != flapoRegistersSpectro::MEAS_N_PPG1_DACOFF(2))
    {
        std::cout << "MEAS2_PPG1_DACOFF != MEAS_N_PPG1_DACOFF(2)" << std::endl;// <3 <3 <3 <3 
        std::cout << "MEAS2_PPG1_DACOFF(2).Address = " << flapoRegistersSpectro::MEAS_N_PPG1_DACOFF(2).Address << " =/= " << flapoRegistersSpectro::MEAS2_PPG1_DACOFF.Address << std::endl;
        std::cout << "MEAS2_PPG1_DACOFF(2).BitField = " << flapoRegistersSpectro::MEAS_N_PPG1_DACOFF(2).BitField << " =/= " << flapoRegistersSpectro::MEAS2_PPG1_DACOFF.BitField << std::endl;    
    }
    if(flapoRegistersSpectro::MEAS2_PPG2_DACOFF != flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(2))
    {
        std::cout << "MEAS2_PPG2_DACOFF != MEAS_N_PPG2_DACOFF(2)" << std::endl; // <3 <3 <3 <3
        std::cout << "MEAS2_PPG2_DACOFF(2).Address = " << flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(2).Address << " =/= " << flapoRegistersSpectro::MEAS2_PPG2_DACOFF.Address << std::endl;
        std::cout << "MEAS2_PPG2_DACOFF(2).BitField = " << flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(2).BitField << " =/= " << flapoRegistersSpectro::MEAS2_PPG2_DACOFF.BitField << std::endl;
    }
    if(flapoRegistersSpectro::MEAS3_PPG2_DACOFF != flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(3))
    {
        std::cout << "MEAS3_PPG2_DACOFF != MEAS_N_PPG2_DACOFF(3)" << std::endl;// <3 <3 <3 <3
        std::cout << "MEAS3_PPG2_DACOFF(3).Address = " << flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(3).Address << " =/= " << flapoRegistersSpectro::MEAS3_PPG2_DACOFF.Address << std::endl;
        std::cout << "MEAS3_PPG2_DACOFF(3).BitField = " << flapoRegistersSpectro::MEAS_N_PPG2_DACOFF(3).BitField << " =/= " << flapoRegistersSpectro::MEAS3_PPG2_DACOFF.BitField << std::endl;
    }
    if(flapoRegistersSpectro::MEAS1_LED_SETLNG != flapoRegistersSpectro::MEAS_N_LED_SETLNG(1))
    {
        std::cout << "MEAS1_LED_SETLNG != MEAS_N_LED_SETLNG(1)" << std::endl;
    }
    if(flapoRegistersSpectro::MEAS2_LED_SETLNG != flapoRegistersSpectro::MEAS_N_LED_SETLNG(2))
    {
        std::cout << "MEAS2_LED_SETLNG != MEAS_N_LED_SETLNG(2)" << std::endl;// <3 <3 <3 <3
        std::cout << "MEAS2_LED_SETLNG(2).Address = " << flapoRegistersSpectro::MEAS_N_LED_SETLNG(2).Address << " =/= " << flapoRegistersSpectro::MEAS2_LED_SETLNG.Address << std::endl;
        std::cout << "MEAS2_LED_SETLNG(2).BitField = " << flapoRegistersSpectro::MEAS_N_LED_SETLNG(2).BitField << " =/= " << flapoRegistersSpectro::MEAS2_LED_SETLNG.BitField << std::endl;
    }
    int ret = flapoRegistersSpectro::MEAS1_AMB.Address;
    ret = flapoRegistersSpectro::MEAS4_AMB.Address;
    ret = flapoRegistersSpectro::MEAS9_AMB.Address;
}








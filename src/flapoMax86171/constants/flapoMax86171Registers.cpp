#include "flapoMax86171Registers.h"

#define MEAS_N_register_define(registerName)\
flapoRegister MEAS_N_##registerName(int N)\
{\
    if (N < 1 || N > 9)\
    {\
        return {\
            -1,\
            -1,\
        };\
    }\
    return {\
        MEAS1_##registerName.Address - MEAS_1_2_BYTE_STEP + N * MEAS_1_2_BYTE_STEP,\
        MEAS1_##registerName.BitField,\
    };\
}\



namespace flapoRegistersSpectro
{
    /*flapoRegister MEAS_N_DRVC_PA(int N)
    { 
        if (N < 1 || N > 9) 
        { 
            return { -1, -1, }; 
        } 
        return 
        { 
            MEAS1_DRVC_PA.Address - MEAS_1_2_BYTE_STEP + N * MEAS_1_2_BYTE_STEP, MEAS1_DRVC_PA.BitField, 
        };
    }*/
    MEAS_N_register_define(DRVA);
    MEAS_N_register_define(DRVB);
    MEAS_N_register_define(DRVC);
    MEAS_N_register_define(AMB);
    MEAS_N_register_define(AVER);
    MEAS_N_register_define(TINT);
    MEAS_N_register_define(PPG1_PDSEL);
    MEAS_N_register_define(PPG2_PDSEL);
    MEAS_N_register_define(PPG1_ADC_RGE);
    MEAS_N_register_define(PPG2_ADC_RGE);
    MEAS_N_register_define(LED_RGE);
    MEAS_N_register_define(FILT_SEL);
    MEAS_N_register_define(SINC3_SEL);
    MEAS_N_register_define(PPG1_DACOFF);
    MEAS_N_register_define(PPG2_DACOFF);
    MEAS_N_register_define(LED_SETLNG);
    MEAS_N_register_define(PD_SETLNG);
    MEAS_N_register_define(DRVA_PA);
    MEAS_N_register_define(DRVB_PA);
    MEAS_N_register_define(DRVC_PA);
    flapoRegister MEAS_N_EN(int N)
    {
        switch (N)
        {
        case 1:
            return MEAS1_EN;
        case 2:
            return MEAS2_EN;
        case 3:
            return MEAS3_EN;
        case 4:
            return MEAS4_EN;
        case 5:
            return MEAS5_EN;
        case 6:
            return MEAS6_EN;
        case 7:
            return MEAS7_EN;
        case 8:
            return MEAS8_EN;
        case 9:
            return MEAS9_EN;
        default:
            return { -1, -1, };
        }
    }
}
#undef MEAS_N_register_define
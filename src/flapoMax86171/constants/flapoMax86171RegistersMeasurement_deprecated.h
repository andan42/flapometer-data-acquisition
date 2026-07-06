#pragma once
#include "flapoMax86171Registers.h"



/*    static const flapoRegister MEAS_N_DRVA(int N)
    {
        //n can be 1 through 9
        if (N < 1 || N > 9)
        {
            //this is fucked cos i dont want iostream in a header so i cant print error...
            return {
                -1,
                -1,
            };
        }
        return {
            MEAS1_DRVA.Address - MEAS_1_2_BYTE_STEP + N * MEAS_1_2_BYTE_STEP,
            MEAS1_DRVA.BitField,
        };
    }*/
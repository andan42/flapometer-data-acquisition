#pragma once
//measurement1
/*static const int MEAS1_Selects = 0x18;
static const int MEAS1_Configuration_1 = 0x19;
static const int MEAS1_Configuration_2 = 0x1A;
static const int MEAS1_Configuration_3 = 0x1B;
static const int MEAS1_DRVA_Current = 0x1C;
static const int MEAS1_DRVB_Current = 0x1D;
static const int MEAS1_DRVC_Current = 0x1E;*/

//auto generate the addresses for the 9 measurements    
#define MEASX_ADDRESS_GEN(x)\
static const int MEAS##x##_Selects = 0x18 + 0x08 * (x - 1);\
static const int MEAS##x ##_Configuration_1 = 0x19 + 0x08 * (x - 1);\
static const int MEAS##x ##_Configuration_2 = 0x1A + 0x08 * (x - 1);\
static const int MEAS##x ##_Configuration_3 = 0x1B + 0x08 * (x - 1);\
static const int MEAS##x ##_DRVA_Current = 0x1C + 0x08 * (x - 1);\
static const int MEAS##x ##_DRVB_Current = 0x1D + 0x08 * (x - 1);\
static const int MEAS##x ##_DRVC_Current = 0x1E + 0x08 * (x - 1);\


namespace flapoAddressesSpectro 
{

    //measurement1
    MEASX_ADDRESS_GEN(1);
    //measurement2
    MEASX_ADDRESS_GEN(2);
    //measurement3
    MEASX_ADDRESS_GEN(3);
    //measurement4
    MEASX_ADDRESS_GEN(4);
    //measurement5
    MEASX_ADDRESS_GEN(5);
    //measurement6
    MEASX_ADDRESS_GEN(6);
    //measurement7
    MEASX_ADDRESS_GEN(7);
    //measurement8
    MEASX_ADDRESS_GEN(8);
    //measurement9
    MEASX_ADDRESS_GEN(9);

    //status
    static const int Status_1 = 0x00;
    static const int Status_2 = 0x01;
    static const int Status_3 = 0x02;
    //fifo
    static const int FIFO_Write_Pointer = 0x04;
    static const int FIFO_Read_Pointer = 0x05;
    static const int FIFO_Counter_1 = 0x06;
    static const int FIFO_Counter_2 = 0x07;
    static const int FIFO_Data_Register = 0x08;
    static const int FIFO_Configuration_1 = 0x09;
    static const int FIFO_Configuration_2 = 0x0A;
    //system control
    static const int System_Configuration_1 = 0x0C;
    static const int System_Configuration_2 = 0x0D;
    static const int System_Configuration_3 = 0x0E;
    static const int Photodiode_Bias = 0x0F;
    static const int Pin_Functional_Configuration = 0x10;
    static const int Output_Pin_Configuration = 0x11;
    //frame rate clock
    static const int FR_Clock_Frequency_Select  = 0x15;
    static const int FR_Clock_Divider_MSB = 0x16;
    static const int FR_Clock_Divider_LSB = 0x17;

    //threshold interrupts
    static const int THRESHOLD_MEAS_SEL = 0x68;
    static const int THRESHOLD_HYST = 0x69;
    static const int PPG_HI_THRESHOLD1 = 0x6A;
    static const int PPG_LO_THRESHOLD1 = 0x6B;
    static const int PPG_HI_THRESHOLD2 = 0x6C;
    static const int PPG_LO_THRESHOLD2 = 0x6D;
    //picket fence
    static const int Picket_Fence_Measurement_Select = 0x70;
    static const int Picket_Fence_Configuration = 0x71;
    //interrupt enables
    static const int Interrupt1_Enable_1 = 0x78;
    static const int Interrupt1_Enable_2 = 0x79;
    static const int Interrupt1_Enable_3 = 0x7A;
    static const int Interrupt2_Enable_1 = 0x7C;
    static const int Interrupt2_Enable_2 = 0x7D;
    static const int Interrupt2_Enable_3 = 0x7E;       
    //part id
    static const int Part_ID = 0xFF;
}

#undef MEASX_ADDRESS_GEN
#pragma once
#include "flapoMax86171Addresses.h"

struct flapoRegister
{
    const int Address;
    const int BitField;
    bool operator==(const flapoRegister& other) const
    {
        return Address == other.Address && BitField == other.BitField;
    }
    bool operator!=(const flapoRegister& other) const
    {
        return !(*this == other);
    }
    //const int ResetValue;
};

#define MEASX_REGISTER_GEN(x)\
static const flapoRegister MEAS##x ##_DRVA = {flapoAddressesSpectro::MEAS##x ##_Selects, 0x03};\
static const flapoRegister MEAS##x ##_DRVB = {flapoAddressesSpectro::MEAS##x ##_Selects, 0x0C};\
static const flapoRegister MEAS##x ##_DRVC = {flapoAddressesSpectro::MEAS##x ##_Selects, 0x30};\
static const flapoRegister MEAS##x ##_AMB = {flapoAddressesSpectro::MEAS##x ##_Selects, 0x40};\
static const flapoRegister MEAS##x ##_AVER = {flapoAddressesSpectro::MEAS##x ##_Configuration_1, 0x07};\
static const flapoRegister MEAS##x ##_TINT = {flapoAddressesSpectro::MEAS##x ##_Configuration_1, 0x18};\
static const flapoRegister MEAS##x ##_PPG1_PDSEL = {flapoAddressesSpectro::MEAS##x ##_Configuration_1, 0x20};\
static const flapoRegister MEAS##x ##_PPG2_PDSEL = {flapoAddressesSpectro::MEAS##x ##_Configuration_1, 0x40};\
static const flapoRegister MEAS##x ##_PPG1_ADC_RGE = {flapoAddressesSpectro::MEAS##x ##_Configuration_2, 0x03};\
static const flapoRegister MEAS##x ##_PPG2_ADC_RGE = {flapoAddressesSpectro::MEAS##x ##_Configuration_2, 0x0C};\
static const flapoRegister MEAS##x ##_LED_RGE = {flapoAddressesSpectro::MEAS##x ##_Configuration_2, 0x30};\
static const flapoRegister MEAS##x ##_FILT_SEL = {flapoAddressesSpectro::MEAS##x ##_Configuration_2, 0x40};\
static const flapoRegister MEAS##x ##_SINC3_SEL = {flapoAddressesSpectro::MEAS##x ##_Configuration_2, 0x80};\
static const flapoRegister MEAS##x ##_PPG1_DACOFF = {flapoAddressesSpectro::MEAS##x ##_Configuration_3, 0x03};\
static const flapoRegister MEAS##x ##_PPG2_DACOFF = {flapoAddressesSpectro::MEAS##x ##_Configuration_3, 0x0C};\
static const flapoRegister MEAS##x ##_LED_SETLNG = {flapoAddressesSpectro::MEAS##x ##_Configuration_3, 0x30};\
static const flapoRegister MEAS##x ##_PD_SETLNG = {flapoAddressesSpectro::MEAS##x ##_Configuration_3, 0xC0};\
static const flapoRegister MEAS##x ##_DRVA_PA = {flapoAddressesSpectro::MEAS##x ##_DRVA_Current, 0xFF};\
static const flapoRegister MEAS##x ##_DRVB_PA = {flapoAddressesSpectro::MEAS##x ##_DRVB_Current, 0xFF};\
static const flapoRegister MEAS##x ##_DRVC_PA = {flapoAddressesSpectro::MEAS##x ##_DRVC_Current, 0xFF};\


#define MEAS_N_register_declare(registerName) flapoRegister MEAS_N_##registerName(int N);


namespace flapoRegistersSpectro
{
    //status 1
    static const flapoRegister PWR_RDY =        {flapoAddressesSpectro::Status_1, 0x01};
    static const flapoRegister THRESH1_HILO =   {flapoAddressesSpectro::Status_1, 0x02};
    static const flapoRegister THRESH2_HILO =   {flapoAddressesSpectro::Status_1, 0x04};
    static const flapoRegister EXP_OVF =        {flapoAddressesSpectro::Status_1, 0x08};
    static const flapoRegister ALC_OVF =        {flapoAddressesSpectro::Status_1, 0x10};
    static const flapoRegister FIFO_DATA_RDY =  {flapoAddressesSpectro::Status_1, 0x20};
    static const flapoRegister FRAME_RDY =      {flapoAddressesSpectro::Status_1, 0x40};
    static const flapoRegister A_FULL =         {flapoAddressesSpectro::Status_1, 0x80}; 

    //status 2
    static const flapoRegister INVALID_CFG =    {flapoAddressesSpectro::Status_2, 0x04};
    static const flapoRegister VDD_OOR =        {flapoAddressesSpectro::Status_2, 0x40};
    static const flapoRegister LED9_COMPB =     {flapoAddressesSpectro::Status_2, 0x08};

    //status 3
    static const flapoRegister LED1_COMPB =     {flapoAddressesSpectro::Status_3, 0x01};
    static const flapoRegister LED2_COMPB =     {flapoAddressesSpectro::Status_3, 0x02};
    static const flapoRegister LED3_COMPB =     {flapoAddressesSpectro::Status_3, 0x04};
    static const flapoRegister LED4_COMPB =     {flapoAddressesSpectro::Status_3, 0x08};
    static const flapoRegister LED5_COMPB =     {flapoAddressesSpectro::Status_3, 0x10};
    static const flapoRegister LED6_COMPB =     {flapoAddressesSpectro::Status_3, 0x20};
    static const flapoRegister LED7_COMPB =     {flapoAddressesSpectro::Status_3, 0x40};
    static const flapoRegister LED8_COMPB =     {flapoAddressesSpectro::Status_3, 0x80};

    //fifo write ptr
    static const flapoRegister FIFO_WR_PTR =    {flapoAddressesSpectro::FIFO_Write_Pointer, 0xFF};

    //fifo read ptr
    static const flapoRegister FIFO_RD_PTR =    {flapoAddressesSpectro::FIFO_Read_Pointer, 0xFF};

    //fifo counter 1
    static const flapoRegister OVF_COUNTER =            {flapoAddressesSpectro::FIFO_Counter_1, 0x7F};
    static const flapoRegister FIFO_DATA_COUNT_MSB =    {flapoAddressesSpectro::FIFO_Counter_1, 0x80};

    //fifo counter 2
    static const flapoRegister FIFO_DATA_COUNT_LSB = {flapoAddressesSpectro::FIFO_Counter_2, 0xFF};

    //fifo data
    static const flapoRegister FIFO_DATA = {flapoAddressesSpectro::FIFO_Data_Register, 0xFF};

    //fifo configuration 1
    static const flapoRegister FIFO_A_FULL = {flapoAddressesSpectro::FIFO_Configuration_1, 0xFF};

    //fifo configuration 2
    static const flapoRegister FIFO_RO =        {flapoAddressesSpectro::FIFO_Configuration_2, 0x02};
    static const flapoRegister FIFO_FULL_TYPE = {flapoAddressesSpectro::FIFO_Configuration_2, 0x04};
    static const flapoRegister FIFO_STAT_CLR =  {flapoAddressesSpectro::FIFO_Configuration_2, 0x08};
    static const flapoRegister FLUSH_FIFO =     {flapoAddressesSpectro::FIFO_Configuration_2, 0x10};

    //system configuration 1
    static const flapoRegister RESET =          {flapoAddressesSpectro::System_Configuration_1, 0x01};
    static const flapoRegister SHDN =           {flapoAddressesSpectro::System_Configuration_1, 0x02};
    static const flapoRegister PPG1_PWRDN =     {flapoAddressesSpectro::System_Configuration_1, 0x04};
    static const flapoRegister PPG2_PWRDN =     {flapoAddressesSpectro::System_Configuration_1, 0x08};
    static const flapoRegister SYNC_MODE =      {flapoAddressesSpectro::System_Configuration_1, 0x30};
    static const flapoRegister SW_FORCE_SYNC =  {flapoAddressesSpectro::System_Configuration_1, 0x40};
    static const flapoRegister MEAS9_EN =       {flapoAddressesSpectro::System_Configuration_1, 0x80};

    //system configuration 2
    static const flapoRegister MEAS1_EN = {flapoAddressesSpectro::System_Configuration_2, 0x01};
    static const flapoRegister MEAS2_EN = {flapoAddressesSpectro::System_Configuration_2, 0x02};
    static const flapoRegister MEAS3_EN = {flapoAddressesSpectro::System_Configuration_2, 0x04};
    static const flapoRegister MEAS4_EN = {flapoAddressesSpectro::System_Configuration_2, 0x08};
    static const flapoRegister MEAS5_EN = {flapoAddressesSpectro::System_Configuration_2, 0x10};
    static const flapoRegister MEAS6_EN = {flapoAddressesSpectro::System_Configuration_2, 0x20};
    static const flapoRegister MEAS7_EN = {flapoAddressesSpectro::System_Configuration_2, 0x40};
    static const flapoRegister MEAS8_EN = {flapoAddressesSpectro::System_Configuration_2, 0x80};

    //system configuration 3
    static const flapoRegister MEAS1_CONFIG_SEL =   {flapoAddressesSpectro::System_Configuration_3, 0x01};
    static const flapoRegister COLLECT_RAW_DATA =   {flapoAddressesSpectro::System_Configuration_3, 0x02};
    static const flapoRegister ALC_DISABLE =        {flapoAddressesSpectro::System_Configuration_3, 0x10};

    //photodiode bias
    static const flapoRegister PD1_BIAS = {flapoAddressesSpectro::Photodiode_Bias, 0x03};
    static const flapoRegister PD2_BIAS = {flapoAddressesSpectro::Photodiode_Bias, 0x0C};
    static const flapoRegister PD3_BIAS = {flapoAddressesSpectro::Photodiode_Bias, 0x30};
    static const flapoRegister PD4_BIAS = {flapoAddressesSpectro::Photodiode_Bias, 0xC0};

    //pin functional configuration
    static const flapoRegister TRIG_ICFG = {flapoAddressesSpectro::Pin_Functional_Configuration, 0x01};
    static const flapoRegister INT2_FCFG = {flapoAddressesSpectro::Pin_Functional_Configuration, 0x18};

    //output pin configuration
    static const flapoRegister INT1_OCFG = {flapoAddressesSpectro::Output_Pin_Configuration, 0x06};
    static const flapoRegister INT2_OCFG = {flapoAddressesSpectro::Output_Pin_Configuration, 0x18};

    //fr clock frequency select
    static const flapoRegister FR_CLK_FINE_TUNE =   {flapoAddressesSpectro::FR_Clock_Frequency_Select, 0x1F};
    static const flapoRegister FR_CLK_SEL =         {flapoAddressesSpectro::FR_Clock_Frequency_Select, 0x20};

    //fr clock divider MSB
    static const flapoRegister FR_CLK_DIV_H = {flapoAddressesSpectro::FR_Clock_Divider_MSB, 0x7F};

    //fr clock divider LSB
    static const flapoRegister FR_CLK_DIV_L = {flapoAddressesSpectro::FR_Clock_Divider_LSB, 0xFF};

    //measurement1
    MEASX_REGISTER_GEN(1);

    //measurement2
    MEASX_REGISTER_GEN(2);

    //measurement3
    MEASX_REGISTER_GEN(3);

    //measurement4
    MEASX_REGISTER_GEN(4);

    //measurement5
    MEASX_REGISTER_GEN(5);

    //measurement6
    MEASX_REGISTER_GEN(6);

    //measurement7
    MEASX_REGISTER_GEN(7);

    //measurement8
    MEASX_REGISTER_GEN(8);

    //measurement9
    MEASX_REGISTER_GEN(9);

    //threshold meas sel
    static const flapoRegister THRESH1_MEAS_SEL = {flapoAddressesSpectro::THRESHOLD_MEAS_SEL, 0x0F};
    static const flapoRegister THRESH2_MEAS_SEL = {flapoAddressesSpectro::THRESHOLD_MEAS_SEL, 0xF0};

    //threshold hyst
    static const flapoRegister LEVEL_HYST =         {flapoAddressesSpectro::THRESHOLD_HYST, 0x07};
    static const flapoRegister TIME_HYST =          {flapoAddressesSpectro::THRESHOLD_HYST, 0x18};
    static const flapoRegister THRESH1_PPG_SEL =    {flapoAddressesSpectro::THRESHOLD_HYST, 0x40};
    static const flapoRegister THRESH2_PPG_SEL =    {flapoAddressesSpectro::THRESHOLD_HYST, 0x80};


    //ppg hi threshold1
    static const flapoRegister THRESHOLD1_UPPER = {flapoAddressesSpectro::PPG_HI_THRESHOLD1, 0xFF};

    //ppg lo threshold1
    static const flapoRegister THRESHOLD1_LOWER = {flapoAddressesSpectro::PPG_LO_THRESHOLD1, 0xFF};

    //ppg hi threshold2
    static const flapoRegister THRESHOLD2_UPPER = {flapoAddressesSpectro::PPG_HI_THRESHOLD2, 0xFF};

    //ppg lo threshold2
    static const flapoRegister THRESHOLD2_LOWER = {flapoAddressesSpectro::PPG_LO_THRESHOLD2, 0xFF};

    //picket fence measurement select
    static const flapoRegister PPG1_PF_MEAS_SEL = {flapoAddressesSpectro::Picket_Fence_Measurement_Select, 0x0F};
    static const flapoRegister PPG2_PF_MEAS_SEL = {flapoAddressesSpectro::Picket_Fence_Measurement_Select, 0xF0};

    //picket fence configuration
    static const flapoRegister THRESHOLD_SIGMA_MULT =   {flapoAddressesSpectro::Picket_Fence_Configuration, 0x03};
    static const flapoRegister IIR_INIT_VALUE =         {flapoAddressesSpectro::Picket_Fence_Configuration, 0x0C};
    static const flapoRegister IIR_TC =                 {flapoAddressesSpectro::Picket_Fence_Configuration, 0x30};
    static const flapoRegister PF_ORDER =               {flapoAddressesSpectro::Picket_Fence_Configuration, 0x40};

    //interrupt1 enable 1
    static const flapoRegister LED_TX_EN1 =         {flapoAddressesSpectro::Interrupt1_Enable_1, 0x01};
    static const flapoRegister THRESH1_HILO_EN1 =   {flapoAddressesSpectro::Interrupt1_Enable_1, 0x02};
    static const flapoRegister THRESH2_HILO_EN1 =   {flapoAddressesSpectro::Interrupt1_Enable_1, 0x04};
    static const flapoRegister EXP_OVF_EN1 =        {flapoAddressesSpectro::Interrupt1_Enable_1, 0x08};
    static const flapoRegister ALC_OVF_EN1 =        {flapoAddressesSpectro::Interrupt1_Enable_1, 0x10};
    static const flapoRegister FIFO_DATA_RDY_EN1 =  {flapoAddressesSpectro::Interrupt1_Enable_1, 0x20};
    static const flapoRegister FRAME_RDY_EN1 =      {flapoAddressesSpectro::Interrupt1_Enable_1, 0x40};
    static const flapoRegister A_FULL_EN1 =         {flapoAddressesSpectro::Interrupt1_Enable_1, 0x80};

    //interrupt1 enable 2
    static const flapoRegister INVALID_CFG_EN1 =    {flapoAddressesSpectro::Interrupt1_Enable_2, 0x04};
    static const flapoRegister VDD_OOR_EN1 =        {flapoAddressesSpectro::Interrupt1_Enable_2, 0x40};
    static const flapoRegister LED9_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_2, 0x80};

    //interrupt1 enable 3
    static const flapoRegister LED1_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x01};
    static const flapoRegister LED2_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x02};
    static const flapoRegister LED3_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x04};
    static const flapoRegister LED4_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x08};
    static const flapoRegister LED5_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x10};
    static const flapoRegister LED6_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x20};
    static const flapoRegister LED7_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x40};
    static const flapoRegister LED8_COMPB_EN1 =     {flapoAddressesSpectro::Interrupt1_Enable_3, 0x80};

    //interrupt2 enable 1
    static const flapoRegister LED_TX_EN2 =         {flapoAddressesSpectro::Interrupt2_Enable_1, 0x01};
    static const flapoRegister THRESH1_HILO_EN2 =   {flapoAddressesSpectro::Interrupt2_Enable_1, 0x02};
    static const flapoRegister THRESH2_HILO_EN2 =   {flapoAddressesSpectro::Interrupt2_Enable_1, 0x04};
    static const flapoRegister EXP_OVF_EN2 =        {flapoAddressesSpectro::Interrupt2_Enable_1, 0x08};
    static const flapoRegister ALC_OVF_EN2 =        {flapoAddressesSpectro::Interrupt2_Enable_1, 0x10};
    static const flapoRegister FIFO_DATA_RDY_EN2 =  {flapoAddressesSpectro::Interrupt2_Enable_1, 0x20};
    static const flapoRegister FRAME_RDY_EN2 =      {flapoAddressesSpectro::Interrupt2_Enable_1, 0x40};
    static const flapoRegister A_FULL_EN2 =         {flapoAddressesSpectro::Interrupt2_Enable_1, 0x80};

    //interrupt2 enable 2
    static const flapoRegister INVALID_CFG_EN2 =    {flapoAddressesSpectro::Interrupt2_Enable_2, 0x04};
    static const flapoRegister VDD_OOR_EN2 =        {flapoAddressesSpectro::Interrupt2_Enable_2, 0x40};
    static const flapoRegister LED9_COMPB_EN2 =     {flapoAddressesSpectro::Interrupt2_Enable_2, 0x80};

    //interrupt2 enable 3
    static const flapoRegister LED1_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x01};
    static const flapoRegister LED2_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x02};
    static const flapoRegister LED3_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x04};
    static const flapoRegister LED4_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x08};
    static const flapoRegister LED5_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x10};
    static const flapoRegister LED6_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x20};
    static const flapoRegister LED7_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x40};
    static const flapoRegister LED8_COMPB_EN2 = {flapoAddressesSpectro::Interrupt2_Enable_3, 0x80};

    //part id
    static const flapoRegister PART_ID = {flapoAddressesSpectro::Part_ID, 0xFF};

    static const int MEAS_1_2_BYTE_STEP = flapoAddressesSpectro::MEAS2_Selects - flapoAddressesSpectro::MEAS1_Selects; //hard coded values are not allowed in this file for reliability

    MEAS_N_register_declare(DRVA);
    MEAS_N_register_declare(DRVB);
    MEAS_N_register_declare(DRVC);
    MEAS_N_register_declare(AMB);
    MEAS_N_register_declare(AVER);
    MEAS_N_register_declare(TINT);
    MEAS_N_register_declare(PPG1_PDSEL);
    MEAS_N_register_declare(PPG2_PDSEL);
    MEAS_N_register_declare(PPG1_ADC_RGE);
    MEAS_N_register_declare(PPG2_ADC_RGE);
    MEAS_N_register_declare(LED_RGE);
    MEAS_N_register_declare(FILT_SEL);
    MEAS_N_register_declare(SINC3_SEL);
    MEAS_N_register_declare(PPG1_DACOFF);
    MEAS_N_register_declare(PPG2_DACOFF);
    MEAS_N_register_declare(LED_SETLNG);
    MEAS_N_register_declare(PD_SETLNG);
    MEAS_N_register_declare(DRVA_PA);
    MEAS_N_register_declare(DRVB_PA);
    MEAS_N_register_declare(DRVC_PA);

    flapoRegister MEAS_N_EN(int N);

}//namespace flapoRegistersSpectro



#undef MEASX_REGISTER_GEN
#undef MEAS_N_register_declare



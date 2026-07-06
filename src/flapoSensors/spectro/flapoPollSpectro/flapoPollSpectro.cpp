#include "flapoPollSpectro.h"

#include "i2c/DeviceI2C.h"
#include "flapoMax86171/chip/flapoMax86171.h"

//maybe move to header
#include "flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"

#include <iostream>

FlapoDataPollSpectro::FlapoDataPollSpectro(FlapoMax86171 *flapoWrapperSpectro)
{
    m_flapoWrapperSpectro = flapoWrapperSpectro;
}

FlapoDataPollSpectro::~FlapoDataPollSpectro()
{
}

FlapoRawData* FlapoDataPollSpectro::poll(FlapoSensor *polledSensor)
{
    int fifo_data_count = 0;
    int ovf_counter = 0;
    int * fifo_data = 0;

    int ret = 0;
    int temp = 0;
    uint8_t * dataBytes = 0;
    FlapoRawDataSpectro *flapoRawDataSpectro;

    //read fifo_data_count
    ret = m_flapoWrapperSpectro->readRegister(flapoRegistersSpectro::FIFO_DATA_COUNT_MSB, &temp);
    if(ret < 0)
    {
        std::cout << "readRegister(FIFO_DATA_COUNT_MSB) failed" << std::endl;
        return nullptr;
    }
    fifo_data_count = temp << 8;
    ret = m_flapoWrapperSpectro->readRegister(flapoRegistersSpectro::FIFO_DATA_COUNT_LSB, &temp);
    if(ret < 0)
    {
        std::cout << "readRegister(FIFO_DATA_COUNT_LSB) failed" << std::endl;
        // todo consider creating and deleting a flapoRawDataSpectro object here, to reset last poll timestamp
        return nullptr;
    }
    fifo_data_count |= temp;

    //read ovf_counter
    ret = m_flapoWrapperSpectro->readRegister(flapoRegistersSpectro::OVF_COUNTER, &ovf_counter);
    if(ret < 0)
    {
        std::cout << "readRegister(OVF_COUNTER) failed" << std::endl;
        // todo consider creating and deleting a flapoRawDataSpectro object here, to reset last poll timestamp
        return nullptr;
    }

    //allocate for data
    fifo_data = new int[fifo_data_count];

    //read bytes
    dataBytes = new uint8_t[fifo_data_count * 3];
    ret = m_flapoWrapperSpectro->multiReadRegister(flapoRegistersSpectro::FIFO_DATA, fifo_data_count * 3, dataBytes);
    if (ret < 0)
    {
        std::cout << "readBytes(FIFO_DATA) failed" << std::endl;
        // todo consider creating and deleting a flapoRawDataSpectro object here, to reset last poll timestamp
        return nullptr;
    }

    //convert bytes to data
    for(int i = 0; i < fifo_data_count; i++)
    {
        fifo_data[i] = dataBytes[i * 3] << 16;
        fifo_data[i] |= dataBytes[i * 3 + 1] << 8;
        fifo_data[i] |= dataBytes[i * 3 + 2];
    }

    //delete dataBytes
    delete[] dataBytes;

    //collect results into return class
    flapoRawDataSpectro = new FlapoRawDataSpectro(polledSensor);
    flapoRawDataSpectro->data = fifo_data;
    flapoRawDataSpectro->dataCount = fifo_data_count;
    flapoRawDataSpectro->overflow = ovf_counter;

    //debug: cout overflow if not zero
    if(ovf_counter != 0)
    {
        std::cout << "Overflow: " << ovf_counter << std::endl;
        std::cout << "Bus: " << m_flapoWrapperSpectro->getDeviceI2C()->m_busNumber << " Address: " << m_flapoWrapperSpectro->getDeviceI2C()->m_address << std::endl;
    }
    //debug: print count
    //std::cout << "Data count: " << fifo_data_count << std::endl;

    //return. this is a pointer, so it will be deleted by the caller. data will be deleted by the destructor of FlapoRawData
    return flapoRawDataSpectro;
}
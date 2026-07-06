#pragma once

#include <string>

#include "flapoData/flapoProcessedData/flapoProcessedData.h"
#include <chrono> 
#include <vector>

//google the format "nist sphere" if anything
//we will be using this to store our spectroflapometer data as well as thermal data

//it has a 1024 byteheader, then all of the data is stored in binary



class FlapoNistSphere
{
private:

    //ASCII 7 bit standard, header
    char* m_header;
    bool headerIsSet = false;

    //header contents
    const char * m_headerType = "NIST_1A"; 
    const int m_headerSize = 1024;

    const char * const m_sensorType;
    const char * const m_sensorLabel; //location (left/right/finger/...) or other unique identifier

    const std::chrono::steady_clock::time_point m_startTimestamp;
    const std::chrono::steady_clock::time_point m_endTimestamp;


    const int m_channelCount;
    const int m_sampleCount;
    const int m_sampleRate;
    
    const std::vector<int> m_intensities;

    const int m_sampleNBytes;
    const char * const m_sampleByteFormat; //little endian or big endian: const char* sampleByteOrder = "01";
    const int m_sampleSigBits;



    //const char* sampleCoding = "PCM"; //PCM, IEEE_FLOAT, ALAW, ULAW, PCM_MULAW, OTHER //we dont need thos
    const char * const m_endHead = "end_head";

    //mutable header contents
    const char* m_operationId;
    const char* m_patientId;

    //data (binary)
    int m_dataSize;
    char* m_data;
public:
    //FlapoNistSphere(const FlapoProcessedData* flapoData, const char* operationId, const char* patientId);
    FlapoNistSphere(const FlapoProcessedData* flapoData);
    //FlapoNistSphere(const char* name, const FlapoProcessedData* flapoData, const char* operationId, const char* patientId);
    FlapoNistSphere(const char* name, const FlapoProcessedData* flapoData);
    //flapoNistSphere(const flapoNistSphere& other);
    ~FlapoNistSphere();

    //setters
    void setOperationId(const char* operationId);
    void setPatientId(const char* patientId);

    //name
    std::string m_name;

    //getters
    int getHeaderSize();
    const char* getHeader();
    int getDataSize();
    const char* getData();
    int64_t getStartTimestamp();
    int64_t getEndTimestamp();

    int getBytesSize();
    void getBytes(char* bytes);

    //save to file
    void saveToFile(std::string dir);
};


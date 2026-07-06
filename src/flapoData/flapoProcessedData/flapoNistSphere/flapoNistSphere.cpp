#include "flapoNistSphere.h"
#include <stdexcept>
#include <string.h>
#include "flapoUtils/flapoReferenceTime/flapoReferenceTime.h"

FlapoNistSphere::FlapoNistSphere(const FlapoProcessedData* flapoData) : 
    m_header(new char[1024]),
    m_sensorType(flapoData->getSensorType()),
    m_sensorLabel(flapoData->getSensorLabel()),
    m_startTimestamp(flapoData->getStart()),
    m_endTimestamp(flapoData->getEnd()),
    m_channelCount(flapoData->getChannelCount()),
    m_sampleCount(flapoData->getSampleCount()),
    m_sampleRate(flapoData->getSampleRate()),
    m_intensities(flapoData->getIntensities()),
    m_sampleNBytes(flapoData->getSampleNBytes()),
    m_sampleByteFormat(flapoData->getSampleByteFormat()),
    m_sampleSigBits(flapoData->getSampleSigBits()),
    m_operationId(flapoData->getOperationId()),
    m_patientId(flapoData->getPatientId())
{
    //header
    //m_channelCount = flapoData->getChannelCount();
    //m_sampleCount = flapoData->getSampleCount();
    //m_sampleRate = flapoData->getSampleRate();
    //m_sampleNBytes = flapoData->getSampleNBytes();
    //m_sampleByteFormat = flapoData->getSampleByteFormat();
    //m_sampleSigBits = flapoData->getSampleSigBits();

    //header
    //m_header = new char[m_headerSize];

    //data
    m_dataSize = flapoData->getBytesSize();
    m_data = new char[m_dataSize];
    flapoData->getBytes(m_data);
}

FlapoNistSphere::~FlapoNistSphere()
{
    delete[] m_header;
    delete[] m_data;
}

FlapoNistSphere::FlapoNistSphere(const char *name, const FlapoProcessedData *flapoData)
: FlapoNistSphere(flapoData)
{
    m_name = std::string(name);
}

/*flapoNistSphere::flapoNistSphere(const flapoNistSphere &other)
{
//todo
}*/

void FlapoNistSphere::setOperationId(const char *operationId)
{
    m_operationId = operationId;
    headerIsSet = false;
}

void FlapoNistSphere::setPatientId(const char *patientId)
{
    m_patientId = patientId;
    headerIsSet = false;
}

int FlapoNistSphere::getHeaderSize()
{
    return m_headerSize;
}

const char *FlapoNistSphere::getHeader()
{
    if(headerIsSet)
    {
        return m_header;
    }
    const char* operationId = m_operationId;
    const char* patientId = m_patientId;
    if(operationId == nullptr)
        operationId = "unknown_operation_id";
    if(patientId == nullptr)
        patientId = "unknown_patient_id";
    
    //miliseconds
    int64_t startMs = std::chrono::duration_cast<std::chrono::milliseconds>(m_startTimestamp - getReferenceTime()).count(); // todo replace with->get this method
    int64_t endMs = std::chrono::duration_cast<std::chrono::milliseconds>(m_endTimestamp - getReferenceTime()).count();     // todo replace with->get this method

    //intensities string
    std::string intensitiesStr = "";

    for(int i = 0; i < m_intensities.size(); i++)
    {
        //each intensity is 8 bit, write it as decimal for now, try hex later
        intensitiesStr += "" + std::to_string(m_intensities[i]) + "_";
    }
    //remove the last underscore if it exists
    if(intensitiesStr.length() > 0)
    {
        intensitiesStr.pop_back();
    }

    std::string headerStr = "";
    headerStr += std::string(m_headerType) + "\n"; //NIST_1A
    headerStr += std::to_string(m_headerSize) + "\n"; //1024
    //headerStr += "database_id -s3 ???\n"; //database_id -s3 ???
    //headerStr += "database_version -s3 ???\n"; //database_version -s3 ???
    //headerStr += "utterance_id -s3 ???\n"; //utterance_id -s3 ???
    headerStr += "sensor_type -s" + std::to_string(strlen(m_sensorType)) + " " + m_sensorType + "\n"; //sensor_type -s3 ???
    headerStr += "sensor_label -s" + std::to_string(strlen(m_sensorLabel)) + " " + m_sensorLabel + "\n"; //sensor_label -s3 ???
    headerStr += "channel_count -i " + std::to_string(m_channelCount) + "\n"; //channel_count -i 18
    headerStr += "start_t -i " + std::to_string(startMs) + "\n"; //start_t -i 1000
    headerStr += "end_t -i " + std::to_string(endMs) + "\n"; //end_t -i 2000
    headerStr += "sample_count -i " + std::to_string(m_sampleCount) + "\n"; //sample_count -i 100000
    headerStr += "sample_rate -i " + std::to_string(m_sampleRate) + "\n"; //sample_rate -i 10
    headerStr += "intensities -s" + std::to_string(intensitiesStr.length()) + " " + intensitiesStr + "\n"; //intensities -s100000 ???
    headerStr += "sample_min -i 524287\n"; //sample_min -i 0
    headerStr += "sample_max -i -524288\n"; //sample_max -i 0 //TODO replace with min and max values
    headerStr += "sample_n_bytes -i " + std::to_string(m_sampleNBytes) + "\n"; //sample_n_bytes -i 4
    std::string m_sampleByteFormatStr = std::string(m_sampleByteFormat);
    headerStr += "sample_byte_format -s" + std::to_string(m_sampleByteFormatStr.length()) + " " + m_sampleByteFormatStr + "\n"; //sample_byte_format -s2 01
    headerStr += "sample_sig_bits -i " + std::to_string(m_sampleSigBits) + "\n"; //sample_sig_bits -i 32
    headerStr += "operation_id -s" + std::to_string(strlen(operationId)) + " " + operationId + "\n"; //operation_id -s3 ???
    headerStr += "patient_id -s" + std::to_string(strlen(patientId)) + " " + patientId + "\n"; //patient_id -s3 ???
    headerStr += "end_head\n"; //end_head
    
    if(headerStr.length() > m_headerSize)
    {
        throw std::runtime_error("NIST header too long. " + std::to_string(headerStr.length()) + " bytes is more than the allowed " + std::to_string(m_headerSize));
    }
    //copy the header string into the header
    memcpy(m_header, headerStr.c_str(), headerStr.length());
    //set the rest of the header to null
    memset(m_header + headerStr.length(), 0, m_headerSize - headerStr.length());

    headerIsSet = true;

    return m_header;
}

int FlapoNistSphere::getDataSize()
{
    return m_dataSize;
}

const char *FlapoNistSphere::getData()
{
    return m_data;
}

int64_t FlapoNistSphere::getStartTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_startTimestamp - getReferenceTime()).count();
}

int64_t FlapoNistSphere::getEndTimestamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(m_endTimestamp - getReferenceTime()).count();
}

int FlapoNistSphere::getBytesSize()
{
    return m_headerSize + m_dataSize;
}

void FlapoNistSphere::getBytes(char *bytes)
{
    if(!headerIsSet)
    {
        getHeader();
    }
    memcpy(bytes, m_header, m_headerSize);
    memcpy(bytes + m_headerSize, m_data, m_dataSize);
}

void FlapoNistSphere::saveToFile(std::string dir)
{
    if(!headerIsSet)
    {
        getHeader();
    }
    std::string path = dir + "/" + m_name + ".sph";
    FILE* file = fopen(path.c_str(), "wb");
    if(file == NULL)
    {
        throw std::runtime_error("Could not open file " + path);
    }
    fwrite(m_header, 1, m_headerSize, file);
    fwrite(m_data, 1, m_dataSize, file);
    fclose(file);
}




#include "flapoSensors/spectro/flapoProcessSpectro/flapoProcessSpectro.h"
#include "flapoSensors/spectro/flapoSensorSpectro/flapoSensorSpectro.h"

#include "flapoData/flapoRawData/flapoRawDataSpectro/flapoRawDataSpectro.h"

#include <gtest/gtest.h>

#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"

//test fixture
class FlapoDataProcessSpectroTest : public ::testing::Test
{
protected:
    //metadata
    FlapoOperationMetadata operationMetadata = FlapoOperationMetadata("unknown_operation_id", "unknown_patient_id");
    //other
    FlapoSensorSpectro *flapoSensor;
    FlapoProcessSpectro *flapoDataProcessSpectro;

    FlapoRawDataSpectro *rawDataSpectro;

    FlapoMaxSample rawDataSamples4Channel1PD[10] = {
        FlapoMaxSample(2, 12), //value AB, A is sample, B is LED/measurement. B also matches the TAG (MSB 4 bits of the 3rd byte). no photodiode.
        FlapoMaxSample(3, 13),
        FlapoMaxSample(4, 14),
        FlapoMaxSample(1, 21),
        FlapoMaxSample(2, 22),
        FlapoMaxSample(3, 23),
        FlapoMaxSample(4, 24),
        FlapoMaxSample(1, 31),
        FlapoMaxSample(2, 32),
        FlapoMaxSample(3, 33)
    };
    FlapoMaxSample rawDataSamples2Chanel2PD[7] = {
        FlapoMaxSample(2, 122), //value ABC, A is sample, B is LED/measurement, C is photodiode. B also matches the TAG (MSB 4 bits of the 3rd byte)

        FlapoMaxSample(1, 211),
        FlapoMaxSample(1, 212),
        FlapoMaxSample(2, 221),
        FlapoMaxSample(2, 222),
        
        FlapoMaxSample(1, 311),
        FlapoMaxSample(1, 312)
    };
    FlapoMaxSample rawDataSamples4Channel1PDMissingTags[10] = {
        FlapoMaxSample(0xF, 12), //value AB, A is sample, B is LED/measurement. B also matches the TAG (MSB 4 bits of the 3rd byte). no photodiode.
        FlapoMaxSample(0xF, 13), //algorithm should be able to handle missing tags, based on a single tag
        FlapoMaxSample(0xF, 14), //dont test without alignment cos its the same algorithm as before for that part
        FlapoMaxSample(0xF, 21),
        FlapoMaxSample(2, 22),
        FlapoMaxSample(0xF, 23),
        FlapoMaxSample(4, 24),
        FlapoMaxSample(0xF, 31),
        FlapoMaxSample(0xF, 32),
        FlapoMaxSample(0xF, 33)
    };
    FlapoMaxSample rawDataSamples2Chanel2PDMissingTagsConsecutiveIdentical[7] = {
        FlapoMaxSample(0xF, 122), //value ABC, A is sample, B is LED/measurement, C is photodiode. B also matches the TAG (MSB 4 bits of the 3rd byte)

        FlapoMaxSample(0xF, 211),
        FlapoMaxSample(0xF, 212),
        FlapoMaxSample(0xF, 221),
        FlapoMaxSample(0xF, 222),
        
        FlapoMaxSample(1, 311),
        FlapoMaxSample(1, 312)
    };
    FlapoMaxSample rawDataSamples2Chanel2PDMissingTagsConsecutiveDifferent[7] = {
        FlapoMaxSample(0xF, 122), //value ABC, A is sample, B is LED/measurement, C is photodiode. B also matches the TAG (MSB 4 bits of the 3rd byte)

        FlapoMaxSample(0xF, 211),
        FlapoMaxSample(0xF, 212),
        FlapoMaxSample(0xF, 221),
        FlapoMaxSample(2, 222),
        
        FlapoMaxSample(1, 311),
        FlapoMaxSample(0xF, 312)
    };
    FlapoMaxSample rawDataSamples2Chanel2PDMissingTagsNonconsecutive[7] = {
        FlapoMaxSample(0xF, 122), //value ABC, A is sample, B is LED/measurement, C is photodiode. B also matches the TAG (MSB 4 bits of the 3rd byte)

        FlapoMaxSample(1, 211),
        FlapoMaxSample(0xF, 212),
        FlapoMaxSample(0xF, 221),
        FlapoMaxSample(2, 222),
        
        FlapoMaxSample(0xF, 311),
        FlapoMaxSample(0xF, 312)
    };
    
    
    void SetUp() override
    {
        flapoSensor = new FlapoSensorSpectro("label", operationMetadata); //todo name label properly
        flapoDataProcessSpectro = new FlapoProcessSpectro();
        flapoSensor->processor = flapoDataProcessSpectro;
        
        rawDataSpectro = new FlapoRawDataSpectro(flapoSensor);
    }

    void fillRawData(FlapoMaxSample *samples, int count)
    {
        rawDataSpectro->dataCount = count;
        rawDataSpectro->data = new int[rawDataSpectro->dataCount];

        for (int i = 0; i < rawDataSpectro->dataCount; i++)
        {
            rawDataSpectro->data[i] = samples[i].getBitsRaw();
        }
    }


    void TearDown() override
    {
        delete rawDataSpectro;
        delete flapoSensor;
        delete flapoDataProcessSpectro;
    }
};

//test with single pd and 4 measurements, putStartAndEndAlignmentData = false
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithoutAlignmentData)
{
    flapoDataProcessSpectro->nrPhotodiodes = 1;
    flapoDataProcessSpectro->nrMeasurements = 4;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = false;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(4, 0)
    };

    fillRawData(rawDataSamples4Channel1PD, sizeof(rawDataSamples4Channel1PD) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = {
        //channel 1                 //channel 2                 //channel 3                 //channel 4
        21, 0x00, 0x10, 0x00,     12, 0x00, 0x20, 0x00,     13, 0x00, 0x30, 0x00,     14, 0x00, 0x40, 0x00, 
        31, 0x00, 0x10, 0x00,     22, 0x00, 0x20, 0x00,     23, 0x00, 0x30, 0x00,     24, 0x00, 0x40, 0x00, 
        0x00, 0x00, 0x00, 0x00,   32, 0x00, 0x20, 0x00,     33, 0x00, 0x30, 0x00,     0x00, 0x00, 0x00, 0x00 //filler bytes are 0x00 because there is no padding, the processed data only saves actual recorded bytes
    };
    //endianness is little endian, fourth byte in each sequence is discardable, 3rd byte 4MSBs are tag

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with single pd and 4 measurements, putStartAndEndAlignmentData = true
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithAlignmentData)
{
    flapoDataProcessSpectro->nrPhotodiodes = 1;
    flapoDataProcessSpectro->nrMeasurements = 4;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(4, 0)
    };

    fillRawData(rawDataSamples4Channel1PD, sizeof(rawDataSamples4Channel1PD) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 2                 //channel 3                 //channel 4
        0x00, 0x00, 0xF0, 0xEE,     12, 0x00, 0x20, 0x00,       13, 0x00, 0x30, 0x00,       14, 0x00, 0x40, 0x00, 
        21, 0x00, 0x10, 0x00,       22, 0x00, 0x20, 0x00,       23, 0x00, 0x30, 0x00,       24, 0x00, 0x40, 0x00, 
        31, 0x00, 0x10, 0x00,       32, 0x00, 0x20, 0x00,       33, 0x00, 0x30, 0x00,       0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test  with 2 pd and 2 measurements, putStartAndEndAlignmentData = false
TEST_F(FlapoDataProcessSpectroTest, test2Pd2MeasurementsWithoutAlignmentData)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = false;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PD, sizeof(rawDataSamples2Chanel2PD) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 2);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 2 * 4);
    //todo replace with asserts

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 2 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        211, 0x00, 0x10, 0x00,      212, 0x00, 0x10, 0x00,      221, 0x00, 0x20, 0x00,      122, 0x00, 0x20, 0x00,
        311-256, 1, 0x10, 0x00,     312-256, 1, 0x10, 0x00,     0  , 0x00, 0x00, 0x00,      222, 0x00, 0x20, 0x00,
    };

    for (int i = 0; i < 4 * 2 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}
        
//test with 2 pd and 2 measurements, putStartAndEndAlignmentData = true
TEST_F(FlapoDataProcessSpectroTest, test2Pd2MeasurementsWithAlignmentData)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PD, sizeof(rawDataSamples2Chanel2PD) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);
    //todo replace with asserts

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     122, 0x00, 0x20, 0x00,
        211, 0x00, 0x10, 0x00,      212, 0x00, 0x10, 0x00,      221, 0x00, 0x20, 0x00,      222, 0x00, 0x20, 0x00,
        311-256, 1, 0x10, 0x00,     312-256, 1, 0x10, 0x00,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with single pd and 4 measurements, putStartAndEndAlignmentData = true, missing tags
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithAlignmentDataMissingTags)
{
    flapoDataProcessSpectro->nrPhotodiodes = 1;
    flapoDataProcessSpectro->nrMeasurements = 4;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(3, 0),
        FlapoSpectroChannel(4, 0)
    };

    fillRawData(rawDataSamples4Channel1PDMissingTags, sizeof(rawDataSamples4Channel1PDMissingTags) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 2                 //channel 3                 //channel 4
        0x00, 0x00, 0xF0, 0xEE,     12, 0x00, 0xF0, 0x00,       13, 0x00, 0xF0, 0x00,       14, 0x00, 0xF0, 0x00, 
        21, 0x00, 0xF0, 0x00,       22, 0x00, 0x20, 0x00,       23, 0x00, 0xF0, 0x00,       24, 0x00, 0x40, 0x00, 
        31, 0x00, 0xF0, 0x00,       32, 0x00, 0xF0, 0x00,       33, 0x00, 0xF0, 0x00,       0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with 2 pd and 2 measurements, putStartAndEndAlignmentData = true, missing tags, consecutive identical
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithAlignmentDataMissingTagsConsecutiveIdentical)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PDMissingTagsConsecutiveIdentical, sizeof(rawDataSamples2Chanel2PDMissingTagsConsecutiveIdentical) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);
    //todo replace with asserts

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     122, 0x00, 0xF0, 0x00,
        211, 0x00, 0xF0, 0x00,      212, 0x00, 0xF0, 0x00,      221, 0x00, 0xF0, 0x00,      222, 0x00, 0xF0, 0x00,
        311-256, 1, 0x10, 0x00,     312-256, 1, 0x10, 0x00,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with 2 pd and 2 measurements, putStartAndEndAlignmentData = true, missing tags, consecutive different
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithAlignmentDataMissingTagsConsecutiveDifferent)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PDMissingTagsConsecutiveDifferent, sizeof(rawDataSamples2Chanel2PDMissingTagsConsecutiveDifferent) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);
    //todo replace with asserts

    //assert data
    char * processedDataBytes = new char[/*processed->getBytesSize()*/ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     122, 0x00, 0xF0, 0x00,
        211, 0x00, 0xF0, 0x00,      212, 0x00, 0xF0, 0x00,      221, 0x00, 0xF0, 0x00,      222, 0x00, 0x20, 0x00,
        311-256, 1, 0x10, 0x00,     312-256, 1, 0xF0, 0x00,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with 2 pd and 2 measurements, putStartAndEndAlignmentData = true, missing tags, nonconsecutive
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithAlignmentDataMissingTagsNonconsecutive)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = true;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PDMissingTagsNonconsecutive, sizeof(rawDataSamples2Chanel2PDMissingTagsNonconsecutive) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 3);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 3 * 4);
    //todo replace with asserts

    //assert data
    char * processedDataBytes = new char[ 4 * 3 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE,     122, 0x00, 0xF0, 0x00,
        211, 0x00, 0x10, 0x00,      212, 0x00, 0xF0, 0x00,      221, 0x00, 0xF0, 0x00,      222, 0x00, 0x20, 0x00,
        311-256, 1, 0xF0, 0x00,     312-256, 1, 0xF0, 0x00,     0x00, 0x00, 0xF0, 0xEE,     0x00, 0x00, 0xF0, 0xEE
    };

    for (int i = 0; i < 4 * 3 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}

//test with 2 pd and 2 measurements, putStartAndEndAlignmentData = false, missing tags, nonconsecutive
TEST_F(FlapoDataProcessSpectroTest, testSinglePd4MeasurementsWithoutAlignmentDataMissingTagsNonconsecutive)
{
    flapoDataProcessSpectro->nrPhotodiodes = 2;
    flapoDataProcessSpectro->nrMeasurements = 2;
    flapoDataProcessSpectro->putStartAndEndAlignmentData = false;
    flapoDataProcessSpectro->channelsInOrder = {
        FlapoSpectroChannel(1, 0),
        FlapoSpectroChannel(1, 1),
        FlapoSpectroChannel(2, 0),
        FlapoSpectroChannel(2, 1)
    };

    fillRawData(rawDataSamples2Chanel2PDMissingTagsNonconsecutive, sizeof(rawDataSamples2Chanel2PDMissingTagsNonconsecutive) / sizeof(FlapoMaxSample));

    std::shared_ptr<FlapoProcessedData> processed = flapoDataProcessSpectro->process(rawDataSpectro);

    //assert auxiliary values of processed data first (not data itself)
    ASSERT_EQ(processed->getChannelCount(), 4);
    ASSERT_EQ(processed->getSampleCount(), 2);
    //ASSERT_EQ(processed->getSampleRate(), x);
    ASSERT_EQ(processed->getSampleNBytes(), 4);
    ASSERT_STREQ(processed->getSampleByteFormat(), "10"); //endianess is little endian
    ASSERT_EQ(processed->getSampleSigBits(), 20);
    ASSERT_EQ(processed->getBytesSize(), 4 * 2 * 4);

    //assert data
    char * processedDataBytes = new char[ 4 * 2 * 4];
    processed->getBytes(processedDataBytes);

    char processedDataBytesExpected[] = { //padding bytes are {0x00, 0x00, 0xF0, 0xFF} because they are defined as FlapoMaxSample(-1, 0) in FlapoProcessSpectro::internalProcess
        //channel 1                 //channel 1                 //channel 2                 //channel 2
        211, 0x00, 0x10, 0x00,     212, 0x00, 0xF0, 0x00,     221, 0x00, 0xF0, 0x00,     122, 0x00, 0xF0, 0x00,
        311-256, 1, 0xF0, 0x00,      312-256, 1, 0xF0, 0x00,      0xEE, 0xEE, 0xEE, 0xEE,      222, 0x00, 0x20, 0x00
    };

    for (int i = 0; i < 4 * 2 * 4; i++)
    {
        //ignore 0xEE bytes as undetermined
        if(processedDataBytesExpected[i] == 0xEE)
            continue;
        EXPECT_EQ(processedDataBytes[i], processedDataBytesExpected[i]) << "failed at index " << i;
    }

    delete[] processedDataBytes;
}














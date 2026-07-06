#include <fstream>
#include <string.h>

#include "flapoData/flapoProcessedData/flapoNistSphere/flapoNistSphere.h"
#include "flapoData/flapoProcessedData/flapoProcessedDataSpectro/flapoProcessedDataSpectro.h"
//flapoSensorThermo just to create a dummy
//JUST SENSOR nothing else
#include "flapoSensors/thermo/flapoSensorThermo/flapoSensorThermo.h"
#include <gtest/gtest.h>


//test fixture
class FlapoNistSphereTest : public ::testing::Test
{
protected:
    //metadata
    FlapoOperationMetadata operationMetadata = FlapoOperationMetadata("unknown_operation_id", "unknown_patient_id");
    FlapoSensorTypeMetadata sensorTypeMetadata = FlapoSensorTypeMetadata("spectro");
    FlapoSensorThermo* sensorPtr = new FlapoSensorThermo("test_label", operationMetadata);
    FlapoSensorMetadata sensorMetadata = FlapoSensorMetadata("test_label", sensorPtr, operationMetadata, sensorTypeMetadata);
    FlapoSensorChannelMetadata sensorChannelMetadata = FlapoSensorChannelMetadata("", sensorMetadata);
    //stuff
    FlapoNistSphere *flapoNistSphere;
    FlapoProcessedData *flapoData;
    FlapoSpectroChannel* channels;
    int* dataLengths;
    FlapoMaxSample** data;
    const char expectedHeaderNoPatient[1024] = "NIST_1A\n" //header is valid for both tests because they use the same data, only test different outputs on it
        "1024\n"
        "sensor_type -s7 spectro\n"
        "sensor_label -s10 test_label\n"
        "channel_count -i 18\n"
        "sample_count -i 10\n"
        "sample_rate -i 20\n"
        "sample_min -i 524287\n"
        "sample_max -i -524288\n"
        "sample_n_bytes -i 4\n"
        "sample_byte_format -s2 10\n"
        "sample_sig_bits -i 20\n"
        "operation_id -s20 unknown_operation_id\n" //this is the value when the field is set to nullptr
        "patient_id -s18 unknown_patient_id\n" //
        "end_head\n";
    void SetUp() override
    {
        auto start = std::chrono::steady_clock::now();
        auto duration = std::chrono::steady_clock::duration(500000000); //0.5 sec
        auto end = start + duration;
        int channelCount = 18;
        channels = new FlapoSpectroChannel[18];
        dataLengths = new int[18];
        data = new FlapoMaxSample*[18];

        //20 samples per second, 0.5 sec demo rec
        for(int i = 0; i < 18; i++)
        {
            dataLengths[i] = 10;
            data[i] = new FlapoMaxSample[10];
            for(int j = 0; j < 10; j++)
            {
                //tags 1, 1, 2, 2, 3, 3, 4, 4, 5, 5 etc for a sample group
                //data goes 10 20 30 40 50 60 70 80 90 100
                data[i][j] = FlapoMaxSample(i / 2 + 1, -20 + j * 10);
            }
        }

        flapoData = new FlapoProcessedDataSpectro(
            start,
            end,
            sensorChannelMetadata,
            channelCount,
            channels,
            dataLengths,
            data
        );
    }

    void TearDown() override
    {
        delete flapoData;
        delete[] channels;
        delete[] dataLengths;
        delete[] data;
    }

};

TEST_F(FlapoNistSphereTest, HeaderTest)
{
    flapoNistSphere = new FlapoNistSphere("test", flapoData);

    const char* actualHeader = flapoNistSphere->getHeader();

    EXPECT_STREQ(actualHeader, expectedHeaderNoPatient);
    delete flapoNistSphere;
}

/*TEST_F(FlapoNistSphereTest, SettersTest)
{
    flapoNistSphere = new FlapoNistSphere(flapoData);
    flapoNistSphere->setOperationId("test_operation_id");
    flapoNistSphere->setPatientId("Test Patient Id");

    const char* actualHeader = flapoNistSphere->getHeader();
    EXPECT_STREQ(actualHeader, expectedHeaderWithPatient);
    delete flapoNistSphere;
}

TEST_F(FlapoNistSphereTest, ConstructorWithOperationIdTest)
{
    flapoNistSphere = new FlapoNistSphere(flapoData);

    const char* actualHeader = flapoNistSphere->getHeader();
    EXPECT_STREQ(actualHeader, expectedHeaderWithPatient);
    delete flapoNistSphere;
}*/

TEST_F(FlapoNistSphereTest, SetterTestReversed)
{
    flapoNistSphere = new FlapoNistSphere(flapoData);
    flapoNistSphere->setPatientId(nullptr);
    flapoNistSphere->setOperationId(nullptr);

    const char* actualHeader = flapoNistSphere->getHeader();
    EXPECT_STREQ(actualHeader, expectedHeaderNoPatient);
    delete flapoNistSphere;
}

TEST_F(FlapoNistSphereTest, DataTest)
{
    flapoNistSphere = new FlapoNistSphere("test", flapoData);

    int actualDataSize = flapoNistSphere->getDataSize();
    ASSERT_EQ(actualDataSize, 720);
    const char* actualData = flapoNistSphere->getData();
    int c = 0;
    for(int i = 0; i < 10; i++)
    {
        for(int j = 0; j < 18; j++)
        {
            int expectedInt = data[j][i].getBitsRaw();
            int actualInt = 0;
            for(int k = 0; k < 4; k++)
            {
                actualInt |= (actualData[c] & 0xFF) << (k * 8);
                c++;
            }
            EXPECT_EQ(actualInt, expectedInt);
        }
    }
    delete flapoNistSphere;
}

TEST_F(FlapoNistSphereTest, AllBytesTest)
{
    flapoNistSphere = new FlapoNistSphere("test", flapoData);

    int actualBytesSize = flapoNistSphere->getBytesSize();
    ASSERT_EQ(actualBytesSize, 1744);

    char* actualBytes = new char[actualBytesSize];
    flapoNistSphere->getBytes(actualBytes);
    //bytes should be header[1024] + data[720]
    
    EXPECT_TRUE(memcmp(actualBytes, expectedHeaderNoPatient, 1024) == 0);
    EXPECT_TRUE(memcmp(actualBytes + 1024, flapoNistSphere->getData(), 720) == 0);
    delete[] actualBytes;
    delete flapoNistSphere;
}

TEST_F(FlapoNistSphereTest, SaveToFileTest)
{
    flapoNistSphere = new FlapoNistSphere("test", flapoData);
    flapoNistSphere->saveToFile("./test/build/");
    //check that the file exists
    std::ifstream file("./test/build/test.sph");
    ASSERT_TRUE(file.good());
    file.close();
    //check that the file contents are correct
    file.open("./test/build/test.sph");
    char* actualBytes = new char[1744];
    file.read(actualBytes, 1744);
    file.close();
    EXPECT_TRUE(memcmp(actualBytes, flapoNistSphere->getHeader(), 1024) == 0);
    EXPECT_TRUE(memcmp(actualBytes + 1024, flapoNistSphere->getData(), 720) == 0);
    //delete file
    remove("./test/build/test.sph");

    delete flapoNistSphere;
}


#pragma once
#include <iostream>
#include <curl/curl.h>

/*
how to use
>check api connection
>check if operation exists
>make new operation if it does not exist
>loop
    >create new recording
    >get last recording number
    >add sensor data to last recording
    >end recording
>end operation


*/
class flapoApi {
public:
    CURL *curl;
    const char* url;
    const char* operation;

    static bool printVerbose;
    static bool printDebug;
    static bool printResponse;

    int currentRecordingNumber;

    flapoApi(const char * operation, const char* url);
    void setUrl(const char* url);
    ~flapoApi();
public:

    //returns 0 if connection is ok and -1 if error
    int checkApiConnection();
    //returns 0 if operation does not exist and 1 if it does. -1 if error
    int checkOperationExists();
    //returns operation_number or -1 if no recording exists. -2 if error
    int getLastRecordingInOperation();
    //returns 0 if operation was created and -1 if error
    int startOperation(const char * details = nullptr);
    //returns recording_number if recording was created or -1 if error
    int createNewRecording();
    //returns 0 if data was added or -1 if error
    int addSensorDataToRecording(char * sensorType, char * sensorSide, char * data, int dataLength, float sampleRate);
private:
    int checkOperationExists(const char * operation);

    int getLastRecordingInOperation(const char * operation);

    int startOperation(const char * operation, const char * details = nullptr);

    int createNewRecording(const char * operation);

    int addSensorDataToRecording(const char * operation, int recordingNumber, char * sensorType, char * sensorSide, char * data, int dataLength, float sampleRate);
private:
    char * sendGetRequest(char * path);
    char * sendPostRequest(char * path, char * data);
};
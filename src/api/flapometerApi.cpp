#include "flapometerApi.h"
#include <iostream>
//json -ljsoncpp
#include <jsoncpp/json/json.h>
//include time stuff
//#include <ctime>
#include <chrono>

#include <string>  
#include <cstring>

//time utils
//https://stackoverflow.com/questions/17223096/outputting-date-and-time-in-c-using-stdchrono
//2019-01-01 00:00:00
void getCurrentTime(char * buffer)
{
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    std::strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", std::localtime(&now_c));
}

//kitchen

void flapoApi::setUrl(const char *url)
{
    this->url = url;
}

flapoApi::flapoApi(const char *operation, const char* url)
{
    this->curl = curl_easy_init();
    this->url = url;
    this->operation = operation;
}

flapoApi::~flapoApi()
{
    curl_easy_cleanup(curl);
}

//callback

size_t writeCallback(char* buf, size_t size, size_t nmemb, char** up)
{ //callback must have this declaration
    //buf is a pointer to the data that curl has for us
    //size*nmemb is the size of the buffer
    if(flapoApi::printResponse)
    {
        std::cout << "writeCallback: ";
        for (int c = 0; c<size*nmemb; c++)
        {
                printf("%c", buf[c]);
        }
    }
    *up = new char[size*nmemb+1]; //
    memcpy(*up, buf, size*nmemb); //copy the data fetched to the up pointer
    (*up)[size*nmemb] = 0; //end marker
    return size*nmemb; //tell curl how many bytes we handled
}

//private parts

char * flapoApi::sendGetRequest(char * path)
{
    curl_easy_reset(curl);
        if(!curl) {
        curl_easy_cleanup(curl);
        if(flapoApi::printDebug)
        {
            std::cout << "Send get request failed! curl_easy_reset or init failed!" << std::endl;
        }
        return nullptr;
    }
    
    char *response;
    //create url + path
    std::string path_str(path);
    std::string url_str(this->url);
    url_str += path_str;
    //setopt path
    curl_easy_setopt(curl, CURLOPT_URL, url_str.c_str());
    //set write function and data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //decide if print debug
    if(flapoApi::printVerbose)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    }
    //print request type
    if(flapoApi::printResponse)
    {
        std::cout << "GET " << url_str.c_str() << std::endl;
    }
    //perform
    curl_easy_perform(curl);
    //print response
    if(flapoApi::printResponse)
    {
        std::cout << std::endl;
        std::cout << "response: " << response << std::endl;
    }
    //return response
    return response;
}

char * flapoApi::sendPostRequest(char * path, char * data)
{
    curl_easy_reset(curl);
        if(!curl) {
        curl_easy_cleanup(curl);
        if(flapoApi::printDebug)
        {
            std::cout << "Send post request failed! curl_easy_reset or init failed!" << std::endl;
        }
        return nullptr;
    }

    char *response;
    //create url + path
    std::string path_str(path);
    std::string url_str(this->url);
    url_str += path_str;
    //setopt path
    curl_easy_setopt(curl, CURLOPT_URL, url_str.c_str());
    //set write function and data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    //set post data
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
    //set post data size
    //curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));
    //set post data type
    struct curl_slist *headers = NULL;
    headers = curl_slist_append(headers, "Expect:");
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
    //decide if print verbose
    if(flapoApi::printVerbose)
    {
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        std::cout << url_str.c_str() << std::endl;
    }
    //print request type
    if(flapoApi::printResponse)
    {
        std::cout << "POST " << url_str.c_str() << std::endl;
    }
    //perform
    curl_easy_perform(curl);
    //print response
    if(flapoApi::printResponse)
    {
        std::cout << std::endl;
        std::cout << "response: " << response << std::endl;
    }
    //return response
    return response;
}

//gets

int flapoApi::checkApiConnection()
{
    char *response = this->sendGetRequest((char*)"/api");
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Api connection failed! No response from /api" << std::endl;
        }
        return -1;
    } 
    //response should be {"env":x,"message":"Flapometer API Live!"}
    Json::Value root;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &root, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Api connection failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        delete[] response;
        return -1;
    }
    if(!root.isMember("message"))
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Api connection failed! No message!" << std::endl;
        }
        delete[] response;
        return -1;
    }
    if(root["message"] != "Flapometer API Live!")
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Api connection failed! Wrong message!" << std::endl;
        }
        delete[] response;
        return -1;
    }
    if(!root.isMember("env"))
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Api connection weird! No env!" << std::endl;
        }
        delete[] response;
        return 0;
    }
    if(flapoApi::printResponse)
    {
        std::cout << "Api connection successful!" << std::endl;
    }
    delete[] response;
    return 0;   
}

int flapoApi::checkOperationExists(const char * operation)
{
    char *path = new char[strlen("/api/operaions/")+strlen(operation)+1];
    strcpy(path, "/api/operations/");
    strcat(path, operation);
    char *response = this->sendGetRequest( path );
    delete[] path;
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Check operation exists failed! No response from /api/operations/" << operation << std::endl;
        }
        return -1;
    }
    //response should be {"error":"no such operation"} or 
    //{"data":{"details":x,"id":x,"name":operation,"start_time":x}}
    Json::Value root;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &root, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Get operation failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        return -1;
    }
    if(root.isMember("error") && root["error"] == "no such operation")
    {
        if(flapoApi::printResponse)
        {
            std::cout << "Operation " << operation << " does not exist!" << std::endl;
        }
        return 0;
    }
    else if(root.isMember("data") && root["data"].isMember("name") && root["data"]["name"] == operation)
    {
        if(flapoApi::printResponse)
        {
            std::cout << "Operation " << operation << " exists!" << std::endl;
        }
        return 1;
    }
    else
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Fallback response, panic if you see this" << std::endl;
            std::cout << "Get operation failed! Wrong response!" << std::endl;
        }
        return -1;
    }

    delete[] response;
    return 0;
}
int flapoApi::checkOperationExists()
{
    return this->checkOperationExists(this->operation);
}
int flapoApi::getLastRecordingInOperation(const char * operation)
{
    char *path = new char[strlen("/api/operations/")+strlen(operation)+strlen("/recordings/last")+1];
    strcpy(path, "/api/operations/");
    strcat(path, operation);
    strcat(path, "/recordings/last");
    char *response = this->sendGetRequest( path );
    delete[] path;
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Get last recording in operation failed! No response from /api/operations/" << operation << "/recordings/last" << std::endl;
        }
        return -1;
    }
    Json::Value root;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &root, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Get last recording in operation failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        return -1;
    }
    //{"error":"no such operation"}
    if(root.isMember("error") && root["error"] == "no such operation")
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Operation " << operation << " does not exist!" << std::endl;
        }
        return -2;
    }
    //{"data":null}
    else if(root.isMember("data") && root["data"].isNull())
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Operation " << operation << " exists but has no recordings!" << std::endl;
        }
        return -1;
    }
    //{"data":{"id":x,"recording_number":N,"start_time":x},"operation_name":operation}
    else if(root.isMember("data") && root["data"].isMember("recording_number"))
    {
        int value = root["data"]["recording_number"].asInt();
        if(flapoApi::printResponse)
        {
            std::cout << "Operation " << operation << " exists and has last recording_number" << value << std::endl;
        }
        return value;
    }
    else
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Fallback response, panic if you see this" << std::endl;
            std::cout << "Get last recording in operation failed! Wrong response!" << std::endl;
        }
        return -1;
    }

    delete[] response;
    return 0;
}
int flapoApi::getLastRecordingInOperation()
{
    return this->getLastRecordingInOperation(this->operation);
}
//posts

int flapoApi::startOperation(const char * operation_name, const char * details)
{
    char * path = new char[strlen("/api/operations/")+1];
    strcpy(path, "/api/operations/");

    //get current time
    char * current_time = new char[100];
    getCurrentTime(current_time);

    //{operation: {name: operaion, details: details, start_time: right now}}
    //do the json magic
    Json::Value rootSend;
    Json::Value operation_json;
    operation_json["name"] = operation_name;
    if(details != nullptr)
    {   
        operation_json["details"] = details;
    }
    else
    {
        operation_json["details"] = "null";
    }
    operation_json["start_time"] = current_time;
    rootSend["operation"] = operation_json;
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::string json_string = Json::writeString(builder, rootSend);
    //std::cout << json_string << std::endl;
    //send the request
    char *response = this->sendPostRequest( path, (char*)json_string.c_str() );
    delete[] path;
    delete[] current_time;
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Start operation failed! No response from /api/operations/" << std::endl;
        }
        delete[] response;
        return -1;
    }
    //response should be {"error":"operation already exists"} or
    //{"data":{"details":details,"id":x,"name":operaion,"start_time":current_time}}
    Json::Value rootReceive;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &rootReceive, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Start operation failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        delete[] response;
        return -1;
    }
    //{"errors":{"name":["has already been taken"]}}
    if(rootReceive.isMember("errors") && rootReceive["errors"].isMember("name") && rootReceive["errors"]["name"].isArray() && rootReceive["errors"]["name"][0] == "has already been taken")
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Operation " << operation_name << " already exists!" << std::endl;
        }
        delete[] response;
        return -1;
    }
    else if(rootReceive.isMember("data") && rootReceive["data"].isMember("name") && rootReceive["data"]["name"] == operation_name)
    {
        if(flapoApi::printResponse)
        {
            std::cout << "Operation " << operation_name << " started correctly!" << std::endl;
        }
        delete[] response;
        return 0;
    }
    else
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Fallback response, panic if you see this" << std::endl;
            std::cout << "Start operation failed! Wrong response!" << std::endl;
        }
        delete[] response;
        return -1;
    }

    delete[] response;
    return 0;
}
int flapoApi::startOperation(const char * details)
{
    return this->startOperation(this->operation, details);
}
int flapoApi::createNewRecording(const char * operation)
{
    char * path = new char[strlen("/api/operations/")+strlen(operation)+strlen("/recordings")+1];
    strcpy(path, "/api/operations/");
    strcat(path, operation);
    strcat(path, "/recordings");

    //get current time
    char * current_time = new char[100];
    getCurrentTime(current_time);

    //{recording: {start_time: right now}}
    //do the json magic
    Json::Value rootSend;
    Json::Value recording_json;
    recording_json["start_time"] = current_time;
    rootSend["recording"] = recording_json;
    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::string json_string = Json::writeString(builder, rootSend);
    //std::cout << json_string << std::endl;
    //send the request
    char *response = this->sendPostRequest( path, (char*)json_string.c_str() );
    delete[] path;
    delete[] current_time;
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Create new recording failed! No response from /api/operations/" << operation << "/recordings" << std::endl;
        }
        return -1;
    }
    Json::Value rootReceive;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &rootReceive, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Create new recording failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        delete[] response;
        return -1;
    }
    //{"data":{"id":x,"recording_number":N,"start_time":x},"operation_name":operation}
    if (rootReceive.isMember("data") && rootReceive["data"].isMember("recording_number"))
    {
        int value = rootReceive["data"]["recording_number"].asInt();
        if(flapoApi::printResponse)
        {
            std::cout << "Operation " << operation << " has new recorging_nr: " << value << std::endl;
        }
        delete[] response;
        return value;
    }
    //{"errors":{"start_time":["can't be blank"]}}
    else if(rootReceive.isMember("errors") && rootReceive["errors"].isMember("start_time") && rootReceive["errors"]["start_time"].isArray() && rootReceive["errors"]["start_time"][0] == "can't be blank")
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Create new recording failed! Start time can't be blank!" << std::endl;
        }
        delete[] response;
        return -1;
    }
    //fallback
    else
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Fallback response, panic if you see this" << std::endl;
            std::cout << "Create new recording failed! Wrong response!" << std::endl;
        }
        delete[] response;
        return -1;
    }
    if(flapoApi::printDebug)
    {
        std::cout << "Fallback response, PANIC HARD if you see this, this is below an else statement" << std::endl;
        std::cout << "Create new recording failed! Wrong response!" << std::endl;
    }
    delete[] response;
    return 0;
}
int flapoApi::createNewRecording()
{
    return this->createNewRecording(this->operation);
}
int flapoApi::addSensorDataToRecording(const char *operation, int recordingNumber, char *sensorType, char *sensorSide, char *data, int dataLength, float sampleRate)
{
    char * recordingNumberString = new char[16];
    sprintf(recordingNumberString, "%d", recordingNumber);

    char * path = new char[strlen("/api/operations/")+strlen(operation)+strlen("/recordings/")+strlen(recordingNumberString)+strlen("/sensors")+1];
    strcpy(path, "/api/operations/");
    strcat(path, operation);
    strcat(path, "/recordings/");
    strcat(path, recordingNumberString);
    strcat(path, "/sensors");

    //get current time
    char * current_time = new char[100];
    getCurrentTime(current_time);

    //{sensor: {sensor_type: sensorType, side: sensorSide, data: data, data_count: dataLength, sample_rate: sampleRate, start_time: right now}}
    //do the json magic
    Json::Value rootSend;
    Json::Value sensor_json;
    sensor_json["sensor_type"] = sensorType;
    sensor_json["side"] = sensorSide;
    sensor_json["data_count"] = dataLength;
    sensor_json["sample_rate"] = sampleRate;
    sensor_json["start_time"] = current_time; //TODO: this should be the time the recording started rather than its upload time
    for(int i = 0; i < dataLength; i++)
    {
        //append as string
        sensor_json["data"].append(data[i]);
    }
    rootSend["sensor"] = sensor_json;



    Json::StreamWriterBuilder builder;
    builder["indentation"] = "";
    std::string json_string = Json::writeString(builder, rootSend);
    //send the request
    char *response = this->sendPostRequest( path, (char*)json_string.c_str() );
    delete[] path;
    delete[] current_time;
    delete[] recordingNumberString;
    if(response == nullptr)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Add sensor data to recording failed! No response from /api/operations/" << operation << "/recordings/" << recordingNumber << "/sensors" << std::endl;
        }
        return -1;
    }
    Json::Value rootReceive;
    Json::CharReader * reader = Json::CharReaderBuilder().newCharReader();
    std::string errors;
    bool parsingSuccessful = reader->parse(response, response + strlen(response), &rootReceive, &errors);
    delete reader;
    if(!parsingSuccessful)
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Add sensor data to recording failed! Parsing failed!" << std::endl;
            std::cout << errors << std::endl;
        }
        delete[] response;
        return -1;
    }
    //{"data":{"data":"data_hidden","data_count":x,"id":x,"sample_rate":x,"sensor_type":x,"side":x,"start_time":x},"operation_name":operation,"recording_number":N}
    if(rootReceive.isMember("data") && rootReceive["data"].isMember("data_count") && rootReceive["data"].isMember("data") & rootReceive["data"]["data"] == "data_hidden")
    {
        int value = rootReceive["data"]["data_count"].asInt();
        if(flapoApi::printResponse)
        {
            std::cout << "Op " << operation << " rec " << recordingNumber << " sensor " << sensorType << " " << sensorSide << " has new data_count: " << value << std::endl;
        }
        delete[] response;
        return 0;
    }
    //{"errors":{ANY:["can't be blank"]}}
    else if(rootReceive.isMember("errors"))
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Add sensor data to recording failed! Errors:" << std::endl;
            std::cout << rootReceive["errors"] << std::endl;
        }
        delete[] response;
        return -1;
    }
    //fallback
    else
    {
        if(flapoApi::printDebug)
        {
            std::cout << "Fallback response, panic if you see this" << std::endl;
            std::cout << "Add sensor data to recording failed! Wrong response!" << std::endl;
        }
        delete[] response;
        return -1;
    }



    delete[] response;
    return 0;
}
int flapoApi::addSensorDataToRecording(char *sensorType, char *sensorSide, char *data, int dataLength, float sampleRate)
{
    return this->addSensorDataToRecording(this->operation, this->currentRecordingNumber, sensorType, sensorSide, data, dataLength, sampleRate);
}

//statics
bool flapoApi::printDebug = true; //should always be true, to know where errors happen
bool flapoApi::printResponse = true; //allows viewing responses when stuff goes correctly
bool flapoApi::printVerbose = false; //allows viewing curl verbose output. enable when debugging curl
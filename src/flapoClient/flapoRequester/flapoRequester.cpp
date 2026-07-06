#include "flapoRequester.h"

#include <curl/curl.h>
#include <iostream>

// #include <openssl/ssl.h>  // Core OpenSSL functions for SSL/TLS
// #include <openssl/sha.h>  // For SHA-256
// #include <openssl/x509.h> // For X509 parsing
// #include <openssl/pem.h>  // For reading certificates

// move mutex variable to class declaration maybe? for now keep separate from .h body
#include <mutex>
// #include <iomanip>
// #include <string.h> //only needed for callback

// global mutex variable
static std::mutex curlGlobalMutex;

// fingerprint
#define VERIFY_SSL 1 // DO NOT EVER SET THIS TO 0 IN PRODUCTION

// define callback function
// it just returns the data as is
size_t FlapoRequester::writeCallback(char *ptr, size_t size, size_t nmemb, std::string *responseData)
{
    responseData->append(ptr, size * nmemb);
    return size * nmemb;
}

/*
//upload data and read callback function were meant to fix a bug thats fixed in a different way
//leave code here in case needed in the future
struct UploadData {
    const char* data;
    std::size_t length;
    std::size_t readPos;
};
size_t FlapoRequester::readCallback(char* buffer, std::size_t size, std::size_t nitems, void* instream)
{
    UploadData* upload = static_cast<UploadData*>(instream);
    std::size_t buffer_size = size * nitems;

    if (upload->readPos >= upload->length)
    {
        return 0;
    }

    std::size_t to_copy = std::min(buffer_size, upload->length - upload->readPos);
    memcpy(buffer, upload->data + upload->readPos, to_copy);
    upload->readPos += to_copy;

    return to_copy;
}
*/

FlapoRequester::FlapoRequester(const std::string &url)
{
    std::lock_guard<std::mutex> lock(curlGlobalMutex);
    this->url = url;
    curl = curl_easy_init(); // Initialize curl
    if (!curl)               // Check if curl was initialized successfully
    {
        std::cerr << "curl_easy_init() failed" << std::endl;
    }
}

FlapoRequester::~FlapoRequester()
{
    std::lock_guard<std::mutex> lock(curlGlobalMutex);
    curl_easy_cleanup(curl); // Cleanup curl
}

// sudo cp ssl/server.crt /usr/local/share/ca-certificates/
// sudo update-ca-certificates

std::string FlapoRequester::get(const std::string &path)
{
    std::lock_guard<std::mutex> lock(curlGlobalMutex);
    std::string response;
    std::string fullUrl = url + path;
    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str()); // Set url to url + path
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "ssl/server.crt");
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, VERIFY_SSL);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, VERIFY_SSL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FlapoRequester::writeCallback); // Set the write callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);                         // Set the write callback data pointer to the response string

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);        // Timeout after 1 second
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L); // Connection timeout of 1 second

    CURLcode res = curl_easy_perform(curl); // Perform the request
    if (res != CURLE_OK)                    // Check if the request was successful
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    return response;
}
// openssl s_client -connect 192.168.1.10:8080 | tee logfile

std::string FlapoRequester::post(const std::string &path, int byteCount, const char *data)
{
    std::lock_guard<std::mutex> lock(curlGlobalMutex);
    std::string response;
    std::string fullUrl = url + path;

    // init mime handle
    curl_mime *mime = curl_mime_init(curl);
    curl_mimepart *part = curl_mime_addpart(mime);

    // UploadData uploadData = { data, static_cast<std::size_t>(byteCount), 0 };
    // curl_mime_data_cb(part, byteCount, FlapoRequester::readCallback, nullptr, nullptr, &uploadData);

    // callback is not needed, it was just made in an attemptto fix an error
    // said error is fixed by providing curl_mime_name and curl_mime_filename

    curl_mime_data(part, data, byteCount);    // Set the post data
    curl_mime_name(part, "file");             // this is required to avoid error 400 bad request
    curl_mime_filename(part, "fakefile.txt"); // this is required to avoid error 422 unprocessable entity (file name is required, as fastapi expects file)

    curl_easy_setopt(curl, CURLOPT_URL, fullUrl.c_str()); // Set url to url + path
    // curl_easy_setopt(curl, CURLOPT_CAINFO, "ssl/server.crt");
    // curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, VERIFY_SSL);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, VERIFY_SSL);
    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);                               // Set the post data
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, FlapoRequester::writeCallback); // Set the write callback function
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);                         // Set the write callback data pointer to the response string
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); // Set the post data
    // curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, byteCount); // Set the post data size

    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 1L);        // Timeout after 1 second
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 1L); // Connection timeout of 1 second

    CURLcode res = curl_easy_perform(curl); // Perform the request

    // free mime
    curl_mime_free(mime);

    if (res != CURLE_OK) // Check if the request was successful
    {
        std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        return "";
    }
    return response;
}

std::string FlapoRequester::post(const std::string &path, const std::string &data)
{
    return post(path, data.length(), data.c_str());
}
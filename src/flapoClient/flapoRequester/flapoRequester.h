#pragma once

#include <string>

typedef void CURL; // Forward declaration of CURL struct. This is the only bit of libcurl we need in the header. Avoid including libcurl headers in my headers.
class FlapoRequester
{
public:
    CURL *curl; // This is a pointer to a CURL object, sort of like a handle.
private:
    std::string url;
    static size_t writeCallback(char *ptr, size_t size, size_t nmemb, std::string *data); // This is a static function, so it can be used as a callback function for libcurl.
    //static size_t readCallback(char* buffer, std::size_t size, std::size_t nitems, void* instream);
    
public:
    FlapoRequester(const std::string &url);
    ~FlapoRequester();
    std::string get(const std::string &path);
    std::string post(const std::string &path, const std::string &data);
    std::string post(const std::string &path, int byteCount, const char* data);
    
    //std::string put(const std::string &path, const std::string &data);
};
/*#include "flapoClient/flapoRequester/flapoRequester.h"
#include <iostream>
#include <string.h>

int simpleGetRequest(std::string url)
{
    FlapoRequester requester(url);

    std::string response = requester.get("");

    //correct response is:
    //Ați spart NASA, felicitări!
    if (response == "Ați spart NASA, felicitări!")
    {
        return 0;
    }
    else
    {
        std::cout << "flapoRequesterGetPost.cpp simpleGetRequest() test failed. Make sure server is running!" << std::endl;
        return 1;
    }
}
int simplePostRequest(std::string url)
{
    FlapoRequester requester(url);

    std::string sendData = "";
    //put the number 123 for 20 times in the string
    for (int i = 0; i < 20; i++)
    {
        sendData += "123 ";
    }

    std::string response = requester.post("/upload", sendData);

    //correct response is:
    //NASA has been broken again. Congrats!
    if (response == "NASA has been broken again. Congrats!")
    {
        return 0;
    }
    else
    {
        std::cout << "flapoRequesterGetPost.cpp simplePostRequest() test failed. Make sure server is running!" << std::endl;
        return 1;
    }
}


int main(int argc, char** argv)
{
    std::string url = "invalid";
    for (int i = 0; i < argc; i++)
    {
        //if arg[i] matches, set value to arg[i + 1]
        if (strcmp(argv[i], "url") == 0)
        {
            url = argv[i + 1];
        }
    }
    if (url == "invalid")
    {
        std::cout << "Invalid url argument, please provide url in test/args file" << std::endl;
        return -1;
    }

    simpleGetRequest(url);
    simplePostRequest(url);
    //wrong ip request
    //wrong path request
    return 0;
}*/
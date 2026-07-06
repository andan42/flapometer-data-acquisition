#include "flapoClientConsumer.h"

#include "flapoRequester/flapoRequester.h"
#include "flapoProcessedData/flapoProcessedData.h"

#include "flapoProcessedData/flapoNistSphere/flapoNistSphere.h"

FlapoClientConsumer::FlapoClientConsumer(FlapoRequester *requester): 
m_requester(requester), deleteRequester(false)
{}

FlapoClientConsumer::FlapoClientConsumer(const std::string &url):
m_requester(new FlapoRequester(url)), deleteRequester(true)
{}

FlapoClientConsumer::~FlapoClientConsumer()
{
    if (deleteRequester)
    {
        delete m_requester;
    }
}

void FlapoClientConsumer::consume(std::shared_ptr<const FlapoProcessedData> consumedData)
{
    FlapoNistSphere nistSphere = FlapoNistSphere(consumedData.get());

    int byteCount = nistSphere.getBytesSize();
    char* bytes = new char[byteCount];
    nistSphere.getBytes(bytes);

    std::string response = m_requester->post("/upload", byteCount, bytes);
    //std::cout << response << std::endl;

    delete[] bytes;
}

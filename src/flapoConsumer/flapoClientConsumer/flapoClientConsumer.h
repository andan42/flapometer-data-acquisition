#pragma once
#include "flapoConsumer/flapoConsumer.h"

class FlapoRequester;

class FlapoClientConsumer : public FlapoConsumer
{
private:
    FlapoRequester *m_requester;
    bool deleteRequester;
public:
    FlapoClientConsumer(FlapoRequester *requester);
    FlapoClientConsumer(const std::string &url);
    ~FlapoClientConsumer();
    void consume(std::shared_ptr<const FlapoProcessedData> consumedData) override;

};
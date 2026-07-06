#pragma once
#include <memory>

//abstract class that will be used to create a consumer
//consumers will are called by the consume thread

class FlapoProcessedData; //forward declaration

class FlapoConsumer
{
public:
    virtual void consume(std::shared_ptr<const FlapoProcessedData> consumedData) = 0;
    virtual ~FlapoConsumer() = default;
};
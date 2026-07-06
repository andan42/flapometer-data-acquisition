#pragma once
#include "flapoConsumer/flapoConsumer.h"
#include <string>

class FlapoSaveConsumer : public FlapoConsumer
{
private:
    std ::string saveDirPath;

public:
    FlapoSaveConsumer(const std::string &saveDirPath);
    ~FlapoSaveConsumer();
    void consume(std::shared_ptr<const FlapoProcessedData> consumedData) override;
};

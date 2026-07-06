#include "flapoSaveConsumer.h"

#include "flapoProcessedData/flapoProcessedData.h"

#include "flapoProcessedData/flapoNistSphere/flapoNistSphere.h"

#include <sys/stat.h>
#include <stdexcept>
#include <sys/types.h>
#include <cstring>

FlapoSaveConsumer::FlapoSaveConsumer(const std::string &saveDirPath)
    : saveDirPath(saveDirPath)

{
    // Check if the directory exists
    struct stat info;
    if (stat(saveDirPath.c_str(), &info) != 0)
    {
        // Directory does not exist, create it
        if (mkdir(saveDirPath.c_str(), 0777) != 0)
        {
            throw std::runtime_error("Could not create directory " + saveDirPath + ": " + std::strerror(errno));
        }
    }
    else if (!(info.st_mode & S_IFDIR))
    {
        // Path exists but is not a directory
        throw std::runtime_error(saveDirPath + " is not a directory");
    }
}

FlapoSaveConsumer::~FlapoSaveConsumer()
{
}

void FlapoSaveConsumer::consume(std::shared_ptr<const FlapoProcessedData> consumedData)
{
    FlapoNistSphere nistSphere = FlapoNistSphere(consumedData.get());

    int byteCount = nistSphere.getBytesSize();
    char *bytes = new char[byteCount];
    nistSphere.getBytes(bytes);
    std::string timestamp = std::to_string(nistSphere.getStartTimestamp());
    std::string name = consumedData.get()->getMetadata().getSensorLabel() + "_" + timestamp;
    nistSphere.m_name = name;
    nistSphere.saveToFile(saveDirPath);
    delete[] bytes;
}

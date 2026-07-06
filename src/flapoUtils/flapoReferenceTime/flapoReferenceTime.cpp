#include "flapoReferenceTime.h"

static int64_t globalOffset;

std::chrono::steady_clock::time_point getUnoffsetReferenceTime()
{
    static std::chrono::steady_clock::time_point referenceTime = std::chrono::steady_clock::now();
    return referenceTime;
}

std::chrono::steady_clock::time_point getReferenceTime()
{
    return getUnoffsetReferenceTime() - std::chrono::milliseconds(globalOffset);
}

void setReferenceTimeOffset(int64_t offset)
{
    globalOffset = offset;
}

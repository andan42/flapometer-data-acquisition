#pragma once
#include <chrono>

std::chrono::steady_clock::time_point getReferenceTime();
std::chrono::steady_clock::time_point getUnoffsetReferenceTime();
void setReferenceTimeOffset(int64_t offset); // use this to offset the timestamp to account for pre existing recordings in the database
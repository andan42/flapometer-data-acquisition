#pragma once
#include <string>
class FlapoMaxSample
{
private:
    int bits;
    //value only uses 24 bits
    //value contains both the ADC value with a sign bit and the tag
    //the tag is the most significant 4 bits
    //the ADC value is the least significant 20 bits

public:

    //if tag is -1, then this is a dummy sample, meant to align the rest of the data
    int getTag() const;
    int getValue() const;
    int getBitsRaw() const;

    //implement copy operator
    FlapoMaxSample& operator=(const FlapoMaxSample& other)
    {
        bits = other.bits;
        return *this;
    }
        
    FlapoMaxSample();
    FlapoMaxSample(int bits);
    FlapoMaxSample(int tag, int value);
    ~FlapoMaxSample();

    bool operator==(const FlapoMaxSample &other) const;
    bool operator!=(const FlapoMaxSample &other) const { return !(*this == other); };

    std::string toStr();
};

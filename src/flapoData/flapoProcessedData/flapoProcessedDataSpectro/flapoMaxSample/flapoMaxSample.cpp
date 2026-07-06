#include "flapoMaxSample.h"
#include <stdexcept>

int FlapoMaxSample::getTag() const
{
    return bits >> 20;
}

int FlapoMaxSample::getValue() const
{
    //sign bit is bit 19
    //value is bits 0-18
    //sign bit is 1 for negative, 0 for positive
    //value is 2's complement

    int unsigned_value = bits &  0x0007FFFF;
    int sign_bit = bits &        0x00080000;
    
    //create value from sign bit and unsigned value
    int value = sign_bit ? -unsigned_value : unsigned_value;

    return value;
}

int FlapoMaxSample::getBitsRaw() const
{
    return bits;
}

FlapoMaxSample::FlapoMaxSample()
{
}

FlapoMaxSample::FlapoMaxSample(int bits) : bits(bits)
{
    if (bits < 0 || bits > 0x00FFFFFF)
    {
        throw std::runtime_error("FlapoMaxSample::FlapoMaxSample(int bits) ERROR: bits out of range!");
    }
}

FlapoMaxSample::FlapoMaxSample(int tag, int value)
{
    //sign bit is bit 19
    //value is bits 0-18
    //sign bit is 1 for negative, 0 for positive
    //value is 2's complement

    int unsigned_value = value < 0 ? -value : value;
    int sign_bit = value < 0 ? 1 : 0;

    //create bits from sign bit and unsigned value
    bits = ((tag & 0x0F) << 20) | (sign_bit << 19) | unsigned_value;
}

FlapoMaxSample::~FlapoMaxSample() {}

bool FlapoMaxSample::operator==(const FlapoMaxSample &other) const
{
    return bits == other.bits;
}

std::string FlapoMaxSample::toStr()
{
    std::string str = "Tag: " + std::to_string(getTag()) + ", Value: " + std::to_string(getValue());
    return str;
}

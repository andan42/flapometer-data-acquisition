#include "flapoOperationMetadata.h"

FlapoOperationMetadata::FlapoOperationMetadata(const std::string operationLabel, const std::string patientName) :
    m_operationLabel(operationLabel),
    m_patientName(patientName)
{
}

FlapoOperationMetadata::FlapoOperationMetadata(const FlapoOperationMetadata &other) :
    m_operationLabel(other.m_operationLabel),
    m_patientName(other.m_patientName)
{
}

FlapoOperationMetadata::~FlapoOperationMetadata()
{
}

const std::string &FlapoOperationMetadata::getOperationLabel() const
{
    return m_operationLabel;
}

const std::string &FlapoOperationMetadata::getPatientName() const
{
    return m_patientName;
}

bool FlapoOperationMetadata::operator==(const FlapoOperationMetadata &other) const
{
    return m_operationLabel == other.m_operationLabel && m_patientName == other.m_patientName;
}

bool FlapoOperationMetadata::operator!=(const FlapoOperationMetadata &other) const
{
    return !(*this == other);
}

bool FlapoOperationMetadata::isEqual(const FlapoOperationMetadata &other) const
{
    return m_operationLabel == other.m_operationLabel && m_patientName == other.m_patientName;
}


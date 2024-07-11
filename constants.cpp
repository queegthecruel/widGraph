#include "constants.h"


bool unit::operator==(const unit &rhsUnit) const
{
    bool textOK = m_textUnit == rhsUnit.m_textUnit;
    bool conversionOK = m_toSI == rhsUnit.m_toSI;
    return textOK && conversionOK;
}

double unit::m_toUnitFromSI(double valueInSI) const
{
    return valueInSI/m_toSI;
}

double unit::m_toSIFromUnit(double valueInUnit) const
{
    return valueInUnit*m_toSI;
}
/*
bool operator==(const unit &lhs, const unit &rhs)
{
    bool textOK = lhs.m_textUnit == rhs.m_textUnit;
    bool conversionOK = lhs.m_toSI == rhs.m_toSI;
    return textOK && conversionOK;
}
*/

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include "constants_global.h"
#include <QString>
#include <QVector>

class CONSTANTS_EXPORT consts
{
private:
    consts() = delete;
    consts(const consts&) = delete;
    consts operator=(const consts&) = delete;
public:
    inline static constexpr double SI = 1;
    inline static constexpr double PI = 3.14159;
    inline static constexpr double G = 9.81;
};

class unit
{
public:
    unit(const QString &textUnit, double toSI, const unit & unitSI):
        m_textUnit(textUnit), m_toSI(toSI), m_unitSI(unitSI)
    {}
    unit(const QString &textUnit):
        m_textUnit(textUnit), m_toSI(consts::SI), m_unitSI(*this)
    {}
    bool operator==(const unit &rhsUnit) const;
    const QString &m_getUnit() const
        {return m_textUnit;}
    double m_getConversion() const
        {return m_toSI;}
    const unit &m_getSIUnit() const
        {return m_unitSI;}
    double m_toUnitFromSI(double valueInSI) const;
    double m_toSIFromUnit(double valueInUnit) const;
private:
    const QString m_textUnit;
    const double m_toSI;
    const unit& m_unitSI;
};

class CONSTANTS_EXPORT units
{
private:
    units() = delete;
    units(const units&) = delete;
    units operator=(const units&) = delete;
public:
    inline static const unit m = unit("m");
    inline static const unit mm = unit("mm", 0.001, m);
    inline static const unit km = unit("km", 1000, m);

    inline static const unit kg = unit("kg");
    inline static const unit g = unit("g", 0.001, kg);
    inline static const unit t = unit("t", 1000, kg);

    inline static const unit none = unit("");

};

class CONSTANTS_EXPORT vUnits
{
private:
    vUnits() = delete;
    vUnits(const units&) = delete;
    vUnits operator=(const units&) = delete;
public:
    inline static const QVector<unit> length {units::m, units::km, units::mm};
    inline static const QVector<unit> weight {units::kg, units::g, units::t};
};



#endif // CONSTANTS_H

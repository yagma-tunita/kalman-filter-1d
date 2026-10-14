#pragma once

#include <vector>

enum class CurveType
{
    PRIOR,
    REAL,
    INPUT
};

class Curve
{
private:
    CurveType m_type;
    std::vector<double>m_data;

public:
    Curve(const std::vector<double>data,CurveType type);

    CurveType Type() const;
    const std::vector<double>& Data() const;
    size_t Size() const;
    
};
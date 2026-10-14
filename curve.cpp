#include "curve.hpp"

Curve::Curve(const std::vector<double> data, CurveType type):
    m_type(type),
    m_data(std::move(data))
{}

CurveType Curve::Type() const{ return this->m_type; }
const std::vector<double> &Curve::Data() const{ return this->m_data; }
size_t Curve::Size() const{ return this->m_data.size(); }

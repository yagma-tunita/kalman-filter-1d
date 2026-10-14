#pragma once

#include "curve.hpp"
#include <string>

class Graph
{
private:
    std::string m_title;
    std::vector<Curve> m_curves;
    
    double m_yMin;
    double m_yMax;
    double m_xStep;

public:
    Graph(const std::string title, const std::vector<Curve>curves,double yMin,double yMax, double step = 1);

    void Show() const;

    const std::vector<Curve>& Curves() const;
    double YMin() const;
    double YMax() const;
    double XStep() const;
    const std::string& Title() const;
};
#include <windows.h>
#include <vector>
#include "graph.hpp"
#include "curve.hpp"
#include "kalman.h"
#include "noise.hpp"

const int N = 300;

void PrepareData(std::vector<Curve>& curves)
{
    SeedRandom(42);

    KalmanUnit kf;
    InitKalmanUnit(&kf, 0.01, 4.0);

    double sigma = 2.0;

    std::vector<double> xTrue(N);
    std::vector<double> z(N);
    std::vector<double> xEst(N);

    for (int i = 0; i < N; i++)
    {
        xTrue[i] = 25.0;
        z[i]     = xTrue[i] + GaussianNoise(0.0, sigma);
        xEst[i]  = KalmanUpdate(&kf, z[i]);
    }

    curves.push_back(Curve(z,     CurveType::INPUT));
    curves.push_back(Curve(xTrue, CurveType::REAL));
    curves.push_back(Curve(xEst,  CurveType::PRIOR));
}

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    std::vector<Curve> curves;
    PrepareData(curves);

    Graph graph("Kalman Filter", curves, 18.0, 32.0, 1.0);
    graph.Show();

    return 0;
}
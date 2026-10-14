#ifndef KALMAN_H
#define KALMAN_H

typedef struct
{
	double m_X;
	double m_P;

	double m_Q;
	double m_R;

}KalmanUnit;

void InitKalmanUnit(KalmanUnit* unit,const double q,const double r);
double KalmanUpdate(KalmanUnit* unit, const double Z);

#endif

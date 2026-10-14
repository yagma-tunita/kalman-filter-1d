#include "kalman.h"

void InitKalmanUnit(KalmanUnit* unit,const double q,const double r)
{
	unit->m_X = 0.0;
	unit->m_P = 1.0;
	unit->m_Q = q;
	unit->m_R = r;
}

double KalmanUpdate(KalmanUnit* unit, const double Z)
{
	double X_pred = unit->m_X;
	double P_pred = unit->m_P + unit->m_Q;
	double K = P_pred / (P_pred + unit->m_R);

	double X_new = (1-K) * X_pred + K*Z;
	unit->m_X = X_new;
	unit->m_P = (1-K) * P_pred;

	return X_new;
}

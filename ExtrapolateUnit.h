//---------------------------------------------------------------------------

#ifndef ExtrapolateUnitH
#define ExtrapolateUnitH

#include <math.h>
#include "Chart.hpp"
#include "Series.hpp"
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out);
int extrapolate2Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out);

void curveFitting2(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
void curveFitting5(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
//---------------------------------------------------------------------------
#endif

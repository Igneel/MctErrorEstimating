//---------------------------------------------------------------------------

#ifndef ExtrapolateUnitH
#define ExtrapolateUnitH

#include "Chart.hpp"
#include "Series.hpp"
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out);
int extrapolate2Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out);
//---------------------------------------------------------------------------
#endif

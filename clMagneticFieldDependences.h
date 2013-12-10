//---------------------------------------------------------------------------

#ifndef clMagneticFieldDependencesH
#define clMagneticFieldDependencesH

#include "film.h"
#include <math.h>

#include <Series.hpp>

enum ChartType {SXX,SXY,US,UY,S_EFF,RH_EFF};

class clMagneticFieldDependences {

	public:
	clMagneticFieldDependences(int size,long double shag,long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,long double AFactor,long double KFactor,
long double thickness,long double cbRatio,long double currentIntensity,long double numberOfCarrierTypes);
	~clMagneticFieldDependences();

	void getTenzorFromCarrierParams();
	void getEffectiveParamsFromTenzor();
	void getSignalsFromEffectiveParams();
	void getEffectiveParamsFromSignals();
	void getTenzorFromEffectiveParams();
	void calculateMagneticFieldDependences();
	void constructPlotFromTwoMassive(ChartType type,TLineSeries* s,TColor color);
    void constructPlotFromOneMassive(ChartType type, TLineSeries* s,TColor color);

	film *carrierParams;

	private:

    const long double THEALMOSTZERO;

    long double electronCharge;

	long double h;
	const int NumberOfPoints;
	long double *sxx;
	long double *sxy;
	long double *B;
	long double *Us; // продольный сигнал
	long double *Uy; // поперечный сигнал
	long double *s_eff;
	long double *Rh_eff;
};
//---------------------------------------------------------------------------
#endif

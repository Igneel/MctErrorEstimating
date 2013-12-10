//---------------------------------------------------------------------------

#ifndef clMagneticFiledDepencesH
#define clMagneticFiledDepencesH

#include "film.h"

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

	private:
    long double electronCharge;
	film *carrierParams;
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

//---------------------------------------------------------------------------

#ifndef clMagneticFieldDependencesH
#define clMagneticFieldDependencesH

#include "film.h"
#include <math.h>

#include <Series.hpp>

enum SignalType {SXX,SXY,US,UY,S_EFF,RH_EFF};
enum FileSaveMode {ALL_POINTS, SOME_POINTS}; // сейчас SOME_POINTS - это конечно 11 точек.

class clMagneticFieldDependences {

	public:
	clMagneticFieldDependences(int size,long double shag,long double molarCompositionCadmium,
	long double Temperature,long double heavyHoleConcentrerion,long double AFactor,long double KFactor,
	long double thickness,long double cbRatio,long double currentIntensity,long double numberOfCarrierTypes);

	clMagneticFieldDependences(int size,long double shag,film * cp);

	//loadDataFromFile(ChartType * type, string )
	bool saveDataToFile(SignalType type, FileSaveMode saveMode, String filePath);

	~clMagneticFieldDependences();

	long double const * getSignalUs();
	long double const * getSignalUy();
	long double const * getSxx();
	long double const * getSxy();

	int modifySignals(void (*ShumAdding)(const long double *x,long double *out,long double *ret, long double koeff,const int l),
	const long double * idealUs,const long double * idealUy,long double *returnData,long double koeff);

	int modifySignals(double (*TrForMassiveFilter)(long double *inB,
	long double *inY,long double* outB,long double *outY,
	int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh),
	const long double * idealUs,const long double * idealUy,int lengthFilter);


	void calculateEffectiveParamsFromSignals();
	void calculateTenzorFromEffectiveParams();
	void calculateMagneticFieldDependences();
	void constructPlotFromTwoMassive(SignalType type,TLineSeries* s,TColor color);
	void constructPlotFromOneMassive(SignalType type, TLineSeries* s,TColor color);

	film *carrierParams;

	private:

	void calculateTenzorFromCarrierParams();
	void calculateEffectiveParamsFromTenzor();
	void calculateSignalsFromEffectiveParams();
	void calculateMagneticFieldPoints(); // расчет точек магнитного поля.
	void MemoryAllocation(); // выделение памяти.

	static const long double THEALMOSTZERO;
    static const long double electronCharge;

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

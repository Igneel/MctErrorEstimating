//---------------------------------------------------------------------------

#ifndef filmH
#define filmH

class film {

	public:

	film(long double molarCompositionCadmium,
	long double Temperature,long double heavyHoleConcentrerion,
	long double AFactor,long double KFactor,
	long double thickness,long double cbRatio,
	long double currentIntensity,long double numberOfCarrierTypes);

	~film();

	long double getNumberOfCarrierTypes();
	long double getConcentration(int index);
	long double getMobility(int index);

	long double getThickness();
	long double getCBRatio();
    long double getCurrentIntensity();

	private:


	static const long double THEALMOSTZERO=0.00000000001;
	void calcutatingCarrierParams(long double molarCompositionCadmium,
	long double Temperature,long double heavyHoleConcentrerion,long double AFactor,
	long double KFactor,long double thickness,long double cbRatio,long double currentIntensity);
    long double ownConcentrationOfElectrons(long double T, long double x);

	long double Thickness;
	long double CBRatio;
	long double MolarCompositionCadmium;
	long double CurrentTemperature;
	long double CurrentIntensity;
	long double AFactor;
	long double KFactor;

	const int NumberOfCarrierTypes;

	long double *Concentration;
	long double *Mobility;




};
//---------------------------------------------------------------------------
#endif

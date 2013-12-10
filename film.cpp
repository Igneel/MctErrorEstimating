//---------------------------------------------------------------------------


#pragma hdrstop

#include "film.h"
#include <math.h>


film::film(long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,long double AFactor,long double KFactor,
long double thickness,long double cbRatio,long double currentIntensity,
long double numberOfCarrierTypes):NumberOfCarrierTypes(numberOfCarrierTypes)
{
	Concentration=new long double[NumberOfCarrierTypes];
	Mobility=new long double[NumberOfCarrierTypes];

	calcutatingCarrierParams(molarCompositionCadmium,
	Temperature,heavyHoleConcentrerion,AFactor,
	KFactor,thickness,cbRatio,currentIntensity);
}

film::~film()
{
	delete[] Concentration;
	delete[] Mobility;
}

void film::calcutatingCarrierParams(long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,long double AFactor,
long double KFactor,long double thickness,long double cbRatio,long double currentIntensity)
{
	MolarCompositionCadmium=molarCompositionCadmium;
	CurrentTemperature=Temperature;
	Concentration[0]=heavyHoleConcentrerion;
	AFactor=AFactor;
	KFactor=KFactor;
	Thickness=thickness;
	CBRatio=cbRatio;
	CurrentIntensity=currentIntensity;

	long double m0=9.10938188E-31; // ����� ��������� ���� ��� � �� ����� � �����-��:)
	long double mh=0.443*m0; // ����� ������� �����
	long double ml=0.001*m0; // ����� ������ �����

	Concentration[1]=pow(ml/mh,1.5)*Concentration[0]*50; // ������������ ������ ����� ��������� �� 50
	Concentration[2]=-pow(ownConcentrationOfElectrons(CurrentTemperature,
	MolarCompositionCadmium),2)/Concentration[0];

	Mobility[2]=-AFactor*pow(CurrentTemperature/77.0,-KFactor);
	Mobility[1]=-0.1*Mobility[2];
	Mobility[0]=-0.005*Mobility[2];

}

long double film::getNumberOfCarrierTypes()
{
	return NumberOfCarrierTypes;
}

long double film::getConcentration(int index)
{
	if(index>=0 && index<NumberOfCarrierTypes)
		return Concentration[index];
	else
		return 0;
}

long double film::getMobility(int index)
{
	if(index>=0 && index<NumberOfCarrierTypes)
		return Mobility[index];
	else
		return 0;
}

long double film::getThickness()
{
	return Thickness;
}

long double film::getCBRatio()
{
    return CBRatio;
}

long double film::getCurrentIntensity()
{
    return CurrentIntensity;
}

long double film::ownConcentrationOfElectrons(long double T, long double x)
{
	long double k=1.380648813E-23/1.60217646E-19; // ���������� ��������� � ��������-�������
	long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2.0*x)*T;
	return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2./k/T); // ����������� ������������
}

//---------------------------------------------------------------------------

#pragma package(smart_init)

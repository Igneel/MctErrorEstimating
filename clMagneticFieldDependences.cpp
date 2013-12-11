//---------------------------------------------------------------------------


#pragma hdrstop

#include "clMagneticFieldDependences.h"

void clMagneticFieldDependences::MemoryAllocation()
{
	sxx=new long double[NumberOfPoints];
	sxy=new long double[NumberOfPoints];
	B=new long double[NumberOfPoints];
	Us=new long double[NumberOfPoints];
	Uy=new long double[NumberOfPoints];
	s_eff=new long double[NumberOfPoints];
	Rh_eff=new long double[NumberOfPoints];

	for(int i=0;i<NumberOfPoints;i++)
	{
		sxx[i]=0;
		sxy[i]=0;
		B[i]=0;
		Us[i]=0;
		Uy[i]=0;
		s_eff[i]=0;
		Rh_eff[i]=0;
	}

}

const long double clMagneticFieldDependences::THEALMOSTZERO=0.00000000001;
const long double clMagneticFieldDependences::electronCharge=1.60217646E-19;

clMagneticFieldDependences::clMagneticFieldDependences(int size,long double shag,
long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,
long double AFactor,long double KFactor,
long double thickness,long double cbRatio,
long double currentIntensity,long double numberOfCarrierTypes)
:NumberOfPoints(size)
{
	carrierParams = new film(molarCompositionCadmium,Temperature,heavyHoleConcentrerion,
	AFactor,KFactor,thickness,cbRatio,currentIntensity,numberOfCarrierTypes);

	h=shag;
    MemoryAllocation();
	calculateMagneticFieldPoints();
}

clMagneticFieldDependences::clMagneticFieldDependences(int size,long double shag,film * cp)
:NumberOfPoints(size)
{
	h=shag;
    carrierParams = new film(cp->getMolarCompositionCadmium(),
	cp->getCurrentTemperature(),cp->getConcentration(0),
	cp->getAFactor(),cp->getKFactor(),cp->getThickness(),
	cp->getCBRatio(),cp->getCurrentIntensity(),cp->getNumberOfCarrierTypes());
	MemoryAllocation();
	calculateMagneticFieldPoints();
}

clMagneticFieldDependences::~clMagneticFieldDependences()
{
	delete carrierParams;
	delete[] sxx;
	delete[] sxy;
	delete[] B;
	delete[] Us;
	delete[] Uy;
	delete[] s_eff;
	delete[] Rh_eff;
}

void clMagneticFieldDependences::calculateMagneticFieldPoints()
{
	B[0]=0;
	for(int i=1;i<NumberOfPoints;i++)
	{
		B[i]=B[i-1]+h;  // это наше магнитное поле
	}

}

void clMagneticFieldDependences::calculateTenzorFromCarrierParams()
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		sxx[i]=sxy[i]=0;
		int NumberOfCarrierTypes=carrierParams->getNumberOfCarrierTypes();

		for(int j=0;j<NumberOfCarrierTypes;j++)
		{
			sxx[i]+=electronCharge*carrierParams->getConcentration(j)*carrierParams->getMobility(j)/(1.0+
			carrierParams->getMobility(j)*carrierParams->getMobility(j)*B[i]*B[i]);

			sxy[i]+=electronCharge*carrierParams->getConcentration(j)*
			carrierParams->getMobility(j)*carrierParams->getMobility(j)*
			B[i]/(1.0+carrierParams->getMobility(j)*
			carrierParams->getMobility(j)*B[i]*B[i]);
		}
	}

}

void clMagneticFieldDependences::calculateEffectiveParamsFromTenzor()
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
		Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]);
	}

}

void clMagneticFieldDependences::calculateSignalsFromEffectiveParams()
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		Us[i]=carrierParams->getCBRatio()/carrierParams->getThickness()*carrierParams->getCurrentIntensity()/s_eff[i];
		Uy[i]=Rh_eff[i]*carrierParams->getCurrentIntensity()/carrierParams->getThickness();
	}

}
void clMagneticFieldDependences::calculateEffectiveParamsFromSignals()
{
	for (int i = 0; i < NumberOfPoints ; i++)
	{
		if(fabs(Us[i])<THEALMOSTZERO)
			s_eff[i]=0;
		else
		{
			s_eff[i]=carrierParams->getCBRatio()/carrierParams->getThickness()*carrierParams->getCurrentIntensity()/Us[i];
		}
		Rh_eff[i]=carrierParams->getThickness()*Uy[i]/carrierParams->getCurrentIntensity();
	}

}

void clMagneticFieldDependences::calculateTenzorFromEffectiveParams()
{
	for (int i = 0; i < NumberOfPoints ; i++)
	{
		sxx[i]=s_eff[i]/
			(Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1);
		sxy[i]=s_eff[i]*s_eff[i]*Rh_eff[i]/
		   (Rh_eff[i]*Rh_eff[i]*s_eff[i]*s_eff[i]+1);
	}
}

void clMagneticFieldDependences::calculateMagneticFieldDependences()
{
	calculateTenzorFromCarrierParams();
	calculateEffectiveParamsFromTenzor();
	calculateSignalsFromEffectiveParams();
}


void clMagneticFieldDependences::constructPlotFromTwoMassive(ChartType type,TLineSeries* s,TColor color)
{
	s->Clear();
	long double *y=0;
	switch(type)
	{
	case SXX: y=sxx;
		break;
	case SXY: y=sxy;
		break;
	case US: y=Us;
		break;
	case UY: y=Uy;
		break;
	case S_EFF: y=s_eff;
		break;
	case RH_EFF: y=Rh_eff;
		break;
	}

	for (int i = 0; i < NumberOfPoints; i++)
	{
		s->AddXY(B[i],y[i],"",color);
	}
}

void clMagneticFieldDependences::constructPlotFromOneMassive(ChartType type, TLineSeries* s,TColor color)
{
	s->Clear();
	long double *y=0;
	switch(type)
	{
	case SXX: y=sxx;
		break;
	case SXY: y=sxy;
		break;
	case US: y=Us;
		break;
	case UY: y=Uy;
		break;
	case S_EFF: y=s_eff;
		break;
	case RH_EFF: y=Rh_eff;
		break;
	}
	for (int i = 0; i < NumberOfPoints; i++)
	{
		s->AddY(y[i],"",color);
	}
}


long double const * clMagneticFieldDependences::getSignalUs()
{
    return Us;
}

long double const * clMagneticFieldDependences::getSignalUy()
{
    return Uy;
}

long double const * clMagneticFieldDependences::getSxx()
{
	return sxx;
}

long double const * clMagneticFieldDependences::getSxy()
{
    return sxy;
}

int clMagneticFieldDependences::modifySignals
(void (*ShumAdding)(const long double *,long double *,long double *,
long double,const int),const long double * idealUs,const long double * idealUy,long double *returnData,long double koeff)
{
	ShumAdding(idealUs,Us,returnData,koeff,NumberOfPoints);
	ShumAdding(idealUy,Uy,(returnData+3),koeff,NumberOfPoints);

	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();

    return 6;
}

int clMagneticFieldDependences::modifySignals(double (*TrForMassiveFilter)(long double *inB,
long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh),
const long double * idealUs,const long double * idealUy,int lengthFilter)
{
	long double * tempInB=new long double[2*NumberOfPoints];
	long double * tempInSignal=new long double[2*NumberOfPoints];
	long double * tempOutB=new long double[2*NumberOfPoints];
	long double * tempOutSignal=new long double[2*NumberOfPoints];

	for (int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=-idealUs[NumberOfPoints-i-1]+2*idealUs[0];
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=idealUs[i];
		tempInB[i+NumberOfPoints]=B[i];
	}

	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,2*NumberOfPoints,lengthFilter,5000,15,25);

	// надо добавить обработку полученных результатов после фильтрации.
	// и экстрапол€цию
	for(int i=0;i<NumberOfPoints;i++)
	{
        Us[i]=tempOutSignal[i+NumberOfPoints-1];
    }

	for (int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=-idealUy[NumberOfPoints-i-1]+2*idealUy[0];
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=idealUy[i];
		tempInB[i+NumberOfPoints]=B[i];
	}

	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,2*NumberOfPoints,lengthFilter,5000,15,25);

	for(int i=0;i<NumberOfPoints;i++)
	{
		B[i]=tempOutB[i+NumberOfPoints-1];
		Uy[i]=tempOutSignal[i+NumberOfPoints-1];
	}

	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();

	delete[] tempInB;
	delete[] tempInSignal;
	delete[] tempOutB;
	delete[] tempOutSignal;

	return 1;
}

/*
   */
//---------------------------------------------------------------------------

#pragma package(smart_init)

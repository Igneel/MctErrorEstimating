//---------------------------------------------------------------------------


#pragma hdrstop

#include "clMagneticFiledDepences.h"
#include <math.h>

clMagneticFieldDependences::clMagneticFieldDependences(int size,long double shag,
long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,
long double AFactor,long double KFactor,
long double thickness,long double cbRatio,
long double currentIntensity,long double numberOfCarrierTypes)
:NumberOfPoints(size),electronCharge(1.60217646E-19)
{
	carrierParams = new film(molarCompositionCadmium,Temperature,heavyHoleConcentrerion,
	AFactor,KFactor,thickness,cbRatio,currentIntensity,numberOfCarrierTypes);

	h=shag;
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

clMagneticFieldDependences::~clMagneticFieldDependences()
{
	delete[] sxx;
	delete[] sxy;
	delete[] B;
	delete[] Us;
	delete[] Uy;
	delete[] s_eff;
	delete[] Rh_eff;
}

void clMagneticFieldDependences::getTenzorFromCarrierParams()
{
	B[0]=0;

	for(int i=1;i<NumberOfPoints;i++)
	{
		B[i]=B[i-1]+h;  // это наше магнитное поле
	}

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

void clMagneticFieldDependences::getEffectiveParamsFromTenzor()
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
		Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]);
	}

}

void clMagneticFieldDependences::getSignalsFromEffectiveParams()
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		Us[i]=carrierParams->getCBRatio()/carrierParams->getThickness()*carrierParams->getCurrentIntensity()/s_eff[i];
		Uy[i]=Rh_eff[i]*carrierParams->getCurrentIntensity()/carrierParams->getThickness();
	}

}
void clMagneticFieldDependences::getEffectiveParamsFromSignals()
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

void clMagneticFieldDependences::getTenzorFromEffectiveParams()
{

}

void clMagneticFieldDependences::calculateMagneticFieldDependences()
{

}

/*

void getSignalsFromEffectiveParams(MagneticFieldDependences* mfd,CarrierParams* cp)
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		mfd->Us[i]=cp->CBRatio/cp->Thickness*cp->CurrentIntensity/mfd->s_eff[i];
		mfd->Uy[i]=mfd->Rh_eff[i]*cp->CurrentIntensity/cp->Thickness;
	}

}

void getEffectiveParamsFromSignals(MagneticFieldDependences* mfd,CarrierParams* cp)
{
	for (int i = 0; i < NumberOfPoints ; i++)
	{
		if(fabs(mfd->Us[i])<THEALMOSTZERO)
			mfd->s_eff[i]=0;
		else
		{
			mfd->s_eff[i]=cp->CBRatio/cp->Thickness*cp->CurrentIntensity/mfd->Us[i];
		}
		mfd->Rh_eff[i]=cp->Thickness*mfd->Uy[i]/cp->CurrentIntensity;
	}
}

void getTenzorFromEffectiveParams(MagneticFieldDependences* mfd)
{
	for (int i = 0; i < NumberOfPoints ; i++)
	{
		mfd->sxx[i]=mfd->s_eff[i]/
			(mfd->Rh_eff[i]*mfd->Rh_eff[i]*mfd->s_eff[i]*mfd->s_eff[i]+1);
		mfd->sxy[i]=mfd->s_eff[i]*mfd->s_eff[i]*mfd->Rh_eff[i]/
		   (mfd->Rh_eff[i]*mfd->Rh_eff[i]*mfd->s_eff[i]*mfd->s_eff[i]+1);
    }
}

void calculateMagneticFieldDependences(MagneticFieldDependences* mfd,CarrierParams* cp)
{
	getTenzorFromCarrierParams(mfd,cp);
	getEffectiveParamsFromTenzor(mfd);
	getSignalsFromEffectiveParams(mfd,cp);
}

void constructPlotFromTwoMassive(long double *x,long double *y,int length,TLineSeries* s,TColor color)
{
	s->Clear();
	for (int i = 0; i < length; i++)
	{
		s->AddXY(x[i],y[i],"",color);
	}
}

void constructPlotFromOneMassive(long double *y,int length,TLineSeries* s,TColor color)
{
	s->Clear();
	for (int i = 0; i < length; i++)
	{
		s->AddY(y[i],"",color);
	}
}
   */
//---------------------------------------------------------------------------

#pragma package(smart_init)

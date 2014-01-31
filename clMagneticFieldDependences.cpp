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
	// ������������� ������.
	carrierParams = new film(molarCompositionCadmium,Temperature,heavyHoleConcentrerion,
	AFactor,KFactor,thickness,cbRatio,currentIntensity,numberOfCarrierTypes);

	h=shag;
	MemoryAllocation();

	calculateMagneticFieldPoints();
}

clMagneticFieldDependences::clMagneticFieldDependences(int size,long double shag,film * cp)
:NumberOfPoints(size)
{
/*
��� ��������� �� ������ � �����, ��������� ������ ���������� ����.
*/
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
		B[i]=B[i-1]+h;  // ��� ���� ��������� ����
	}

}

void clMagneticFieldDependences::calculateTenzorFromCarrierParams()
{
	// ���������� ������������� �� calculateMagneticFieldDependences()
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
	// ���������� ������������� �� calculateMagneticFieldDependences()
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
	// ���������� ������������� �� calculateMagneticFieldDependences()
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


void clMagneticFieldDependences::constructPlotFromTwoMassive(SignalType type,TLineSeries* s,TColor color)
{
	// ������ ������ �� � - ��������� ����, �� � - ������ ������.
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

void clMagneticFieldDependences::constructPlotFromOneMassive(SignalType type, TLineSeries* s,TColor color)
{
	// �� � - ������ ����� �����.
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

    return 6; // ���������� �����, ���������� ���������� ��������� � ������������ �������, ���� ����� ���������.
}

int clMagneticFieldDependences::modifySignals(double (*TrForMassiveFilter)(long double *inB,
long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh),
const long double * idealUs,const long double * idealUy,int lengthFilter)
{
	long double * tempInB=new long double[2*NumberOfPoints];
	long double * tempInSignal=new long double[2*NumberOfPoints];
	long double * tempOutB=new long double[2*NumberOfPoints+ceil(lengthFilter/2.0)];
	long double * tempOutSignal=new long double[2*NumberOfPoints+ceil(lengthFilter/2.0)];

	// ��������� ������ ��� �������.
	// ���������� ��� � ������������� ��������� ����.
	for (int i = 0; i < NumberOfPoints; i++)
	{
		tempInSignal[i]=-idealUs[NumberOfPoints-i-1]+2*idealUs[0];
		tempInB[i]=-B[NumberOfPoints-i-1];
		tempInSignal[i+NumberOfPoints]=idealUs[i];
		tempInB[i+NumberOfPoints]=B[i];
	}


    // ���������
	TrForMassiveFilter(tempInB,tempInSignal,tempOutB,tempOutSignal,2*NumberOfPoints,lengthFilter,5000,15,25);

	// ���� �������� ��������� ���������� ����������� ����� ����������.
	// � �������������

	// ����� ���������� ������������� ����� �������������� ������� �������.
	for(int i=0;i<NumberOfPoints;i++)
	{
        Us[i]=tempOutSignal[i+NumberOfPoints-1];
    }
    // ������ �� �� ����� � ������ ��������, ��, ���� ������������� �������.
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

	// ������������� �����������.
	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();

	delete[] tempInB;
	delete[] tempInSignal;
	delete[] tempOutB;
	delete[] tempOutSignal;

	return 1;
}

void clMagneticFieldDependences::setB_Us_Uy(long double *newB, long double * newUs,long double *newUy)
{
	for (int i = 0; i < NumberOfPoints; i++) {
		B[i]=newB[i];
		Us[i]=newUs[i];
		Uy[i]=newUy[i];
	}
	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();
}

int clMagneticFieldDependences::modifySignals(ModifyType type,clMagneticFieldDependences * extrapolatedParams)
{
int returnValue=1;
const int a=6;
long double * koef1= new long double [a];
long double * koef2= new long double [a];

long double * newB= new long double [NumberOfPoints];
long double * newUs= new long double [NumberOfPoints];
long double * newUy= new long double [NumberOfPoints];
newB[0]=0;
	switch(type)
	{
		case EXTRAPOLATE:

			curveFitting5(B,Us,0,NumberOfPoints,koef1);
			curveFitting5(B,Uy,0,NumberOfPoints,koef2);

			for(int i=0;i<NumberOfPoints;i++)
			{
				if(i!=0) newB[i]=newB[i-1]+h;

				newUs[i]=pow(newB[i],a-1)*koef1[0]+pow(newB[i],a-2)*koef1[1]+
					pow(newB[i],a-3)*koef1[2]+pow(newB[i],a-4)*koef1[3]+
					pow(newB[i],1)*koef1[4]+koef1[5];

				newUy[i]=pow(newB[i],a-1)*koef2[0]+pow(newB[i],a-2)*koef2[1]+
					pow(newB[i],a-3)*koef2[2]+pow(newB[i],a-4)*koef2[3]+
					pow(newB[i],1)*koef2[4]+koef2[5];

			}
			extrapolatedParams->setB_Us_Uy(newB,newUs,newUy);
			break;
		default:
		returnValue=0;
	}

delete[] koef1;
delete[] koef2;
delete[] newB;
delete[] newUs;
delete[] newUy;
return returnValue;

}

bool clMagneticFieldDependences::saveDataToFile(SignalType type, FileSaveMode saveMode, String filePath)
{
	bool isRoundNeeded = false;

	if(isRoundNeeded)
	{
        ;
    }

	// ��������� ������.
	TStringList * tsl=new TStringList();

	for(int i=0;i<NumberOfPoints;i++)
	{
		tsl->Add(FloatToStr(B[i])+"\t"+FloatToStr(sxx[i])+"\t"+FloatToStr(sxy[i]));
	}
	tsl->Text=ReplaceTextW(tsl->Text,",","."); // �������� ��� ������� �� �����

	switch(type)
	{
	case SXX:
	case SXY:

		break;
	case US:
	case UY:

		break;
	case S_EFF:
	case RH_EFF:
	default:
	return false;
	}
}

/*
   */
//---------------------------------------------------------------------------

#pragma package(smart_init)

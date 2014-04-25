//---------------------------------------------------------------------------


#pragma hdrstop

#include "clMagneticFieldDependences.h"


void clMagneticFieldDependences::MemoryAllocation()
{
	sxx.resize(NumberOfPoints);
	sxy.resize(NumberOfPoints);
	B.resize(NumberOfPoints);
	Us.resize(NumberOfPoints);
	Uy.resize(NumberOfPoints);
	s_eff.resize(NumberOfPoints);
	Rh_eff.resize(NumberOfPoints);

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
	/*delete[] sxx;
	delete[] sxy;
	delete[] B;
	delete[] Us;
	delete[] Uy;
	delete[] s_eff;
	delete[] Rh_eff; */
}

void clMagneticFieldDependences::calculateMagneticFieldPoints()
{
	//B.push_back(0);
	B[0]=0;
	for(int i=1;i<NumberOfPoints;i++)
	{
		//B.push_back(B[i-1]+h);// ��� ���� ��������� ����
		B[i]=B[i-1]+h;
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
	std::vector<long double> y;
	//long double *y=0;
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
	//long double *y=0;
	std::vector<long double> y;
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


std::vector<long double> const & clMagneticFieldDependences::getSignalUs()
{
    return Us;
}

std::vector<long double> const & clMagneticFieldDependences::getSignalUy()
{
    return Uy;
}

std::vector<long double> const & clMagneticFieldDependences::getSxx()
{
	return sxx;
}

std::vector<long double> const & clMagneticFieldDependences::getSxy()
{
    return sxy;
}

int clMagneticFieldDependences::modifySignals
(void (*ShumAdding)(std::vector<long double> const &x,std::vector<long double> &out,
	std::vector<long double> &ret, long double koeff,const int l),
	const std::vector<long double> & idealUs,const std::vector<long double> & idealUy,
	std::vector<long double> &returnData,long double koeff)
{

	ShumAdding(idealUs,Us,returnData,koeff,NumberOfPoints);
	ShumAdding(idealUy,Uy,returnData,koeff,NumberOfPoints);

	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();

	return returnData.size();
}

int clMagneticFieldDependences::modifySignals(double (*TrForMassiveFilter)(long double *inB,
long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh),
std::vector<long double> const & idealUs,std::vector<long double> const & idealUy,int lengthFilter)
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
		//Us[i]=tempOutSignal[i];
    }
    // ������ �� �� ����� � ������ ��������, ��, ���� ������������� �������.
	for (int i = 0; i < NumberOfPoints; i++)
	{
		//tempInSignal[i]=idealUy[NumberOfPoints-i-1];   // ���
		tempInSignal[i]=-idealUy[NumberOfPoints-i-1]+2*idealUy[0];  // �����
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
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	for(int i=0;i<NumberOfPoints;i++)
	{
		if(B[i]<0)
		{
			B[i]=0;
			//Us[i]=0;
			Uy[i]=0;
		}
	}
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

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

void clMagneticFieldDependences::setB_Us_Uy(std::vector<long double> &newB,
	std::vector<long double> & newUs,std::vector<long double> &newUy)
{
	for (int i = 0; i < NumberOfPoints; i++) {
		B[i]=newB[i];
		Us[i]=newUs[i];
		Uy[i]=newUy[i];
	}
	calculateEffectiveParamsFromSignals();
	calculateTenzorFromEffectiveParams();
}

void calculatePolinomByKoef(std::vector<long double> & inX, std::vector<long double> & koef, std::vector<long double> & outF )
{
	int NumberOfPoints=inX.size();
	int NumberOfPolinomKoef=koef.size()-1;

	outF.clear();

	for(int i=0;i<NumberOfPoints;i++)
	{
		outF.push_back(0);
		for (int koef_index = 0; koef_index <= NumberOfPolinomKoef; koef_index++)
		{
			long double powedB=0;
			if(NumberOfPolinomKoef-koef_index==0)
			powedB=1;
			else
			powedB=powl(inX[i],NumberOfPolinomKoef-koef_index);

			outF[i]+=koef[koef_index]*powedB;
		}
	}

}


int clMagneticFieldDependences::modifySignals(ModifyType type,clMagneticFieldDependences * extrapolatedParams)
{
int returnValue=1;

const int polinomPowForUs=6;
const int polinomPowForUy=6;

std::vector<long double> koefUs;
std::vector<long double> koefUy;

std::vector<long double> newB;
std::vector<long double> newUs;
std::vector<long double> newUy;

koefUs.resize(polinomPowForUs+1);
koefUy.resize(polinomPowForUy+1);

int i=0;
	switch(type)
	{
		case EXTRAPOLATE:

			for(int i=0;i<500;i++)
			{
				B.push_back(0);
				Uy.push_back(0);
			}

			curveFittingUniversal(&B, &Us, &koefUs,polinomPowForUs);
			curveFittingUniversal(&B, &Uy, &koefUy,polinomPowForUy);

            for(int i=0;i<500;i++)
			{
				B.pop_back();
				Uy.pop_back();
			}

			newB.clear();
			newB.push_back(0);
			for (int i = 1; i < NumberOfPoints; i++) {
				newB.push_back(newB[i-1]+h);
			}

			calculatePolinomByKoef(newB,koefUs,newUs);
			calculatePolinomByKoef(newB,koefUy,newUy);

			extrapolatedParams->setB_Us_Uy(newB,newUs,newUy);

			//----------� ��� ��� ������������ ����������� ������� � �������----

			while(B[i++]<=0 && i<NumberOfPoints);
			i-=2; // ���� ��� ���� ���������� �������������.

            for(int j=i;j<NumberOfPoints;j++)
			{      // ���������� ��� �������� � ������.
				B[j-i]=B[j];
				Us[j-i]=Us[j];
				Uy[j-i]=Uy[j];
			}

			for(int j=NumberOfPoints-i;j<NumberOfPoints;j++)
			{     // � ����� ���������� ������������������ ��������.
				B[j]=newB[j];
				Us[j]=newUs[j];
				Uy[j]=newUy[j];
			}

			calculateEffectiveParamsFromSignals();
			calculateTenzorFromEffectiveParams();
			//------------------------------------------------------------------

			break;
		default:
		returnValue=0;
	}

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
	tsl->Text=ReplaceChar(tsl->Text,L',',L'.'); // �������� ��� ������� �� �����

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

	return true;
}

/*
   */
//---------------------------------------------------------------------------



void extrapolateNoiseFiltered(clMagneticFieldDependences * NoisyParams,
		clMagneticFieldDependences * FilteredParams,
		clMagneticFieldDependences * ExtrapolatedParams)
	{

	int sizeTemp=3*NoisyParams->NumberOfPoints;

	const int polinomPowForUs=4;
	const int polinomPowForUy=4;

	std::vector<long double> koefUs;
	std::vector<long double> koefUy;

	koefUs.resize(polinomPowForUs+1);
	koefUy.resize(polinomPowForUy+1);

	std::vector<long double> newB;
	std::vector<long double> newUs;
	std::vector<long double> newUy;

	newUs.resize(NoisyParams->NumberOfPoints);
	newUy.resize(NoisyParams->NumberOfPoints);

	std::vector<long double> inBforUs;
	std::vector<long double> inBforUy;
	std::vector<long double> inUs;
	std::vector<long double> inUy;


	for(int i=0;i<NoisyParams->NumberOfPoints;i++)
	{
		inBforUs.push_back(NoisyParams->B[i]);
		inBforUy.push_back(NoisyParams->B[i]);
		inUs.push_back(NoisyParams->Us[i]);
		inUy.push_back(NoisyParams->Uy[i]);

	}
	for(int i=0;i<NoisyParams->NumberOfPoints;i++)
	{
		inBforUs.push_back(FilteredParams->B[i]);
		inBforUy.push_back(FilteredParams->B[i]);
		inUs.push_back(FilteredParams->Us[i]);
		inUy.push_back(FilteredParams->Uy[i]);

	}
	for(int i=0;i<NoisyParams->NumberOfPoints;i++)
	{

		inBforUs.push_back(NoisyParams->B[i]);
		inBforUy.push_back(NoisyParams->B[i]);
		inUs.push_back(NoisyParams->Us[i]);
		inUy.push_back(NoisyParams->Uy[i]);
	}




		curveFittingUniversal(&inBforUs, &inUs, &koefUs,polinomPowForUs);
		curveFittingUniversal(&inBforUy, &inUy, &koefUy,polinomPowForUy);

		newB.clear();
		newB.push_back(0);
		for (int i = 1; i < NoisyParams->NumberOfPoints; i++) {
			newB.push_back(newB[i-1]+NoisyParams->h);
		}

		calculatePolinomByKoef(newB,koefUs,newUs);
		calculatePolinomByKoef(newB,koefUy,newUy);

		ExtrapolatedParams->setB_Us_Uy(newB,newUs,newUy);

		//----------------------------------------------------------------------
		inBforUs.clear();
		inBforUy.clear();
		inUs.clear();
		inUy.clear();

		for(int i=0;i<FilteredParams->NumberOfPoints;i++)
		{
			inBforUs.push_back(FilteredParams->B[i]);
			inBforUy.push_back(FilteredParams->B[i]);
			inUs.push_back(FilteredParams->Us[i]);
			inUy.push_back(FilteredParams->Uy[i]);
		}
		for(int i=0;i<ExtrapolatedParams->NumberOfPoints;i++)
		{
			inBforUs.push_back(ExtrapolatedParams->B[i]);
			inBforUy.push_back(ExtrapolatedParams->B[i]);
			inUs.push_back(ExtrapolatedParams->Us[i]);
			inUy.push_back(ExtrapolatedParams->Uy[i]);
		}

		for(int i=0;i<500;i++)
		{
			inBforUy.push_back(0);
			inUy.push_back(0);
		}

		curveFittingUniversal(&inBforUs, &inUs, &koefUs,polinomPowForUs);
		curveFittingUniversal(&inBforUy, &inUy, &koefUy,polinomPowForUy);

		newB.clear();
		newB.push_back(0);
		for (int i = 1; i < NoisyParams->NumberOfPoints; i++) {
			newB.push_back(newB[i-1]+NoisyParams->h);
		}

		calculatePolinomByKoef(newB,koefUs,newUs);
		calculatePolinomByKoef(newB,koefUy,newUy);

		ExtrapolatedParams->setB_Us_Uy(newB,newUs,newUy);

		//----------------------------------------------------------------------
		//----------� ��� ��� ������������ ����������� ������� � ������������� �������----
            int i=0;
			while(FilteredParams->B[i++]<=0 && i<FilteredParams->NumberOfPoints);
			i-=2; // ���� ��� ���� ���������� �������������.

			for(int j=i;j<FilteredParams->NumberOfPoints;j++)
			{      // ���������� ��� �������� � ������.
				FilteredParams->B[j-i]=FilteredParams->B[j];
				FilteredParams->Us[j-i]=FilteredParams->Us[j];
				FilteredParams->Uy[j-i]=FilteredParams->Uy[j];
			}

			for(int j=FilteredParams->NumberOfPoints-i;j<FilteredParams->NumberOfPoints;j++)
			{     // � ����� ���������� ������������������ ��������.
				FilteredParams->B[j]=newB[j];
				FilteredParams->Us[j]=newUs[j];
				FilteredParams->Uy[j]=newUy[j];
			}

			FilteredParams->calculateEffectiveParamsFromSignals();
			FilteredParams->calculateTenzorFromEffectiveParams();
			//------------------------------------------------------------------

	}



#pragma package(smart_init)

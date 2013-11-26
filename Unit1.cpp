//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"
#include "NoiseUnit.h"
#include <math.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "Chart"
#pragma link "Series"
#pragma link "TeEngine"
#pragma link "TeeProcs"
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------



/*

TO DO:

1. ���������� ���������� ������� ����������� �������
2. �������������� ���������� ����������� �������, � �� ��������� �������
3. �������������� ���������� ����������� �������, � ����� � ���
4. ��������� �������� �������� � 1-3.
5. ������ ��������� ����� ���� ������ � ���������.
6. �������� ������� ���������� ������ ��� ������ ���������� �� ������ ������.
7. ���� �� ������� ������� ���������� �������� �������� ����� � ���� �����, ���� ��� ������� ���������.

*/



/*

����, ��������� ��������� ������

class Film

class Carrier

class

*/
  /*
class carrier
{


public:

long double getMobility() {return Mobility;}
long double getConcentration() {return Concentration;}
void setMobility(long double m) {Mobility=m;}
void setConcentration(long double c) {Concentration=c;}


carrier(long double concentration, long double mobility)
{
Concentration=concentration;
Mobility=mobility;
}

private:

long double Concentration;
long double Mobility;

};

class film
{
public:

film(int numberOfCarriers, long double *carrierParams )
{
NumberOfCarriers=numberOfCarriers;
for (int i=0; i < NumberOfCarriers; i++) {
 Carriers[i] = new carrier(carrierParams[2*i],carrierParams[2*i+1]);
}

}

void setFilmParams(long double thickness,long double cBRatio,
long double molarCompositionCadmium,long double currentTemperature,
long double currentIntensity,long double aFactor,long double kFactor)
{
Thickness=thickness;
CBRatio=cBRatio;
MolarCompositionCadmium=molarCompositionCadmium;
CurrentTemperature=currentTemperature;
CurrentIntensity=currentIntensity;
AFactor=aFactor;
KFactor=kFactor;
}



private:
int NumberOfCarriers;
long double Thickness;
long double CBRatio;
long double MolarCompositionCadmium;
long double CurrentTemperature;
long double CurrentIntensity;
long double AFactor;
long double KFactor;
carrier **Carriers;

};

class magneticFieldDependences
{
public:

magneticFieldDependences(long double B, long double Us, long double Uy)
{
;
}

private:

int Length;
long double sxx;
long double sxy;
long double B;
long double Us; // ���������� ������
long double Uy; // ���������� ������
long double s_eff;
long double Rh_eff;


};    */


const long double THEALMOSTZERO=0.00000000001;



__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
const int NumberOfPoints=201;//11;  // ���������� �����
const int NumberOfNumbersAfterPoint=4; // �������� ����������.

const int NumberOfCarrierTypes=3;

long double electronCharge=1.60217646E-19; // ����� ���������
long double h; // �������� ����

//------------------------------------------------------------------------------

struct MagneticFieldDependences {
	//const int Length;
	long double sxx[NumberOfPoints];
	long double sxy[NumberOfPoints];
	long double B[NumberOfPoints];
	long double Us[NumberOfPoints]; // ���������� ������
	long double Uy[NumberOfPoints]; // ���������� ������
	long double s_eff[NumberOfPoints];
	long double Rh_eff[NumberOfPoints];
};

struct CarrierParams
{
	long double Thickness;
	long double CBRatio;
	long double MolarCompositionCadmium;
	long double CurrentTemperature;
	long double CurrentIntensity;
	long double AFactor;
	long double KFactor;
	long double Concentration[NumberOfCarrierTypes];
	long double Mobility[NumberOfCarrierTypes];
} carrierParams;

MagneticFieldDependences IdealParams;
MagneticFieldDependences ParamsWithNoise;
MagneticFieldDependences FilteredParams;


//------------------------------------------------------------------------------



long double B_ideal[NumberOfPoints]={0};

long double sxx_ideal[NumberOfPoints]={0};  // ������ ������������
long double sxy_ideal[NumberOfPoints]={0};
long double s_eff_ideal[NumberOfPoints]={0};  // �����������
long double Rh_eff_ideal[NumberOfPoints]={0};  // ������ �����
long double Us_ideal[NumberOfPoints]={0};     // ���������� ������
long double Uy_ideal[NumberOfPoints]={0};     // ���������� ������

long double s_eff_withNoise[NumberOfPoints]={0};  // �����������
long double Rh_eff_withNoise[NumberOfPoints]={0};  // ������ �����
long double Us_withNoise[NumberOfPoints]={0};  // ���������� ������
long double Uy_withNoise[NumberOfPoints]={0};  // ���������� ������
long double sxx_withNoise[NumberOfPoints]={0};  // ������ ������������
long double sxy_withNoise[NumberOfPoints]={0};

long double s_eff_filtered[NumberOfPoints]={0};  // �����������
long double Rh_eff_filtered[NumberOfPoints]={0};  // ������ �����
long double Us_filtered[NumberOfPoints]={0};  // ���������� ������
long double Uy_filtered[NumberOfPoints]={0};  // ���������� ������
long double sxx_filtered[NumberOfPoints]={0};  // ������ ������������
long double sxy_filtered[NumberOfPoints]={0};

bool silentMode=false; // ���� ���� ����� ��� - �� ������� ���������� �� ���������� ��� �����
// ���� ������������ ��� ������� ��������))
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
g_Nz_par->Cells[0][1]="���������";
g_Nz_par->Cells[0][2]="������ �����";
g_Nz_par->Cells[0][3]="������� �����";
g_Nz_par->Cells[1][0]="������������";
g_Nz_par->Cells[2][0]="�����������";
}
//---------------------------------------------------------------------------

void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);
void ParamsKRT(void);


void getTenzorFromCarrierParams(MagneticFieldDependences* mfd,CarrierParams* cp)
{
	mfd->B[0]=0;

	for(int i=1;i<NumberOfPoints;i++)
	{
		mfd->B[i]=mfd->B[i-1]+h;  // ��� ���� ��������� ����
	}

	for(int i=0;i<NumberOfPoints;i++)
	{
	mfd->sxx[i]=mfd->sxy[i]=0;

		for(int j=0;j<NumberOfCarrierTypes;j++)
		{
		mfd->sxx[i]+=electronCharge*cp->Concentration[j]*cp->Mobility[j]/(1.0+
		cp->Mobility[j]*cp->Mobility[j]*mfd->B[i]*mfd->B[i]);

		mfd->sxy[i]+=electronCharge*cp->Concentration[j]*cp->Mobility[j]*cp->Mobility[j]*
		mfd->B[i]/(1.0+cp->Mobility[j]*cp->Mobility[j]*mfd->B[i]*mfd->B[i]);
		}
	}

}

void getEffectiveParamsFromTenzor(MagneticFieldDependences* mfd)
{
	for(int i=0;i<NumberOfPoints;i++)
	{
		mfd->s_eff[i]=(mfd->sxx[i]*mfd->sxx[i]+mfd->sxy[i]*mfd->sxy[i])/mfd->sxx[i];
		mfd->Rh_eff[i]=mfd->sxy[i]/(mfd->sxx[i]*mfd->sxx[i]+mfd->sxy[i]*mfd->sxy[i]);
	}
}


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
	for (int i = 0; i < length; i++)
	{
		s->AddXY(x[i],y[i],"",color);
	}
}

void constructPlotFromOneMassive(long double *y,int length,TLineSeries* s,TColor color)
{
	for (int i = 0; i < length; i++)
	{
		s->AddY(y[i],"",color);
	}
}



// ������ �������� ������������
void __fastcall TForm1::bCalculateCarrierParamsClick(TObject *Sender)
{

/*

������ ������������ � ����������� � �����.
������ ���.
��������� ��������� ����.
������������ ���������� ������� �� ��������� ���������� �����.
������� �� �� �����
������������ ����������� ��������.
�������� ���������� �������.


*/
	ParamsKRT();

	Series1->Clear();     // ������ ��� �������
	Series2->Clear();
	Series5->Clear();
	LineSeries5->Clear();

	LineSeries1->Clear();
	LineSeries7->Clear();

	LineSeries3->Clear();
	LineSeries9->Clear();

	for(int i=0;i<NumberOfCarrierTypes;i++) // ������ �������� � �����
	{
		carrierParams.Mobility[i]=g_Nz_par->Cells[2][i+1].ToDouble();
		carrierParams.Concentration[i]=g_Nz_par->Cells[1][i+1].ToDouble();
	}

	h=g_hsize->Text.ToDouble(); // ��������� �������� ����

	calculateMagneticFieldDependences(&IdealParams,&carrierParams);
	
	constructPlotFromTwoMassive(IdealParams.B,IdealParams.sxx,NumberOfPoints,Series1,clRed);
	constructPlotFromTwoMassive(IdealParams.B,IdealParams.sxy,NumberOfPoints,Series2,clRed);
	constructPlotFromTwoMassive(IdealParams.B,IdealParams.sxx,NumberOfPoints,Series5,clRed);
	constructPlotFromTwoMassive(IdealParams.B,IdealParams.sxy,NumberOfPoints,LineSeries5,clRed);

	bGaussianNoiseGenerator->Enabled=1;

	constructPlotFromTwoMassive(IdealParams.B,IdealParams.s_eff,NumberOfPoints,LineSeries1,clRed);
	constructPlotFromTwoMassive(IdealParams.B,IdealParams.Rh_eff,NumberOfPoints,LineSeries7,clRed);

	constructPlotFromOneMassive(IdealParams.Us,NumberOfPoints,LineSeries3,clRed);
	constructPlotFromOneMassive(IdealParams.Uy,NumberOfPoints,LineSeries9,clRed);
}
//---------------------------------------------------------------------------
// ���������� ����������� ��� ����������, ���������� ������ ��� ���� ����� �� �������
// ����� ����� ������� ��� �� � ���� ��������� �������� - � ���� ��� ������
void RoundM(long double * x,int length)
{
int S=pow(10,NumberOfNumbersAfterPoint);
for(int i=0;i<length;i++)
{
int n= (int)(x[i]*S)%10;
if(n<5)
x[i]=floor(x[i]*S)/S;
else
x[i]=ceil(x[i]*S)/S;
}
}


// ��������� ����
void __fastcall TForm1::bGaussianNoiseGeneratorClick(TObject *Sender)
{
/*
� ��������� ��������� ������� ����������� ���.
��������� �� �����.
*/

srand(time(NULL));
long double vz[3]={0};  // � ��� � ��� � %

for (int i = 0; i < NumberOfPoints; i++) {
	ParamsWithNoise.Us[i]=IdealParams.Us[i];
	ParamsWithNoise.Uy[i]=IdealParams.Uy[i];
	ParamsWithNoise.B[i]=IdealParams.B[i];
}

ShumAdding(IdealParams.Us,ParamsWithNoise.Us,vz,Edit5->Text.ToDouble(),NumberOfPoints);
Form1->mDebug->Lines->Add(FloatToStr(vz[0]));
Form1->Edit1->Text=FloatToStr(vz[1]); // ���
Form1->Edit3->Text=FloatToStr(vz[2]);

ShumAdding(IdealParams.Uy,ParamsWithNoise.Uy,vz,Edit5->Text.ToDouble(),NumberOfPoints);
Form1->mDebug->Lines->Add(FloatToStr(vz[0]));
Form1->Edit2->Text=FloatToStr(vz[1]); // ���
Form1->Edit4->Text=FloatToStr(vz[2]);

Series3->Clear();
Series4->Clear();

constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Us,NumberOfPoints,Series3,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Uy,NumberOfPoints,Series4,clRed);

tenzorCalculating->Enabled=1;
}
//---------------------------------------------------------------------------
// ���������� ����������� �����������
void __fastcall TForm1::bSaveAllPointsClick(TObject *Sender)
{
if (rbLeftPlot->Checked)
{
	 SavingAllPoints(Series1,Series2);
}
if (rbRightPlot->Checked)
{
	 SavingAllPoints(Series3,Series4);
}
if (rbFilteredPlot->Checked)
{
     SavingAllPoints(Series7,Series8);
}
if (rbIdealPlot->Checked)
{
     SavingAllPoints(LineSeries3,LineSeries9);
}

}
//---------------------------------------------------------------------------


void __fastcall TForm1::g_Nz_parKeyPress(TObject *Sender, wchar_t &Key)
{
if(Key=='.') Key=',';
if(Key=='e') Key='E';
if(Key=='�') Key='E';  // ������� � ������� �� ����������
if (Key=='�') Key='E';
if ((Key>='0' && Key<='9') || Key==',' || Key=='\b' || Key=='E' ) ;
else
Key=0;
}
//---------------------------------------------------------------------------
// ��������� ������ ����, �� ���� ����� �� �� ��� ������)
void __fastcall TForm1::bWhiteNoiseGeneratorClick(TObject *Sender)
{
/*long double x[l];
long double y[l];
long double y1[l];
srand(time(NULL));
for(int i=0;i<l;i++)
{
y[i]=rand()%1000/250./Edit5->Text.ToDouble();
y1[i]=rand()%1000/250./Edit6->Text.ToDouble();
}
Memo1->Lines->Add(FloatToStr(Mo(y,l)));
Memo1->Lines->Add(FloatToStr(Mo(y1,l)));
for(int i=0;i<l;i++)
{
y[i]=Series1->YValues->Value[i]+y[i];
}
Series3->Clear() ;

for(int i=0;i<l;i++)
Series3->AddXY(B[i],y[i],"",clRed);




for(int i=0;i<l;i++)
{
y1[i]=Series2->YValues->Value[i]+y1[i];
}
Series4->Clear() ;

for(int i=0;i<l;i++)
Series4->AddXY(B[i],y1[i],"",clRed);

 Edit1->Text=FloatToStr(Sko(sxx,y,l));
 Edit2->Text=FloatToStr(Sko(sxy,y1,l));

 // ��� %  �� ��������������� ��������
 Edit3->Text=FloatToStr(Edit1->Text.ToDouble()/Mo(sxx,l)*100);
 Edit4->Text=FloatToStr(Edit2->Text.ToDouble()/Mo(sxy,l)*100);
 */
}
//---------------------------------------------------------------------------
// ���������� ����������� ������������ ����������
long double niSob(long double T, long double x)

{
long double k=1.380648813E-23/1.60217646E-19; // ���������� ��������� � ��������-�������
long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2*x)*T;
return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2/k/T); // ����������� ������������
}

void calcutatingCarrierParams(CarrierParams *carrierParams,long double molarCompositionCadmium,
long double Temperature,long double heavyHoleConcentrerion,long double AFactor,long double KFactor,
long double thikness,long double cbRatio,long double currentIntensity)
{
	carrierParams->MolarCompositionCadmium=molarCompositionCadmium;
	carrierParams->CurrentTemperature=Temperature;
	carrierParams->Concentration[0]=heavyHoleConcentrerion;
	carrierParams->AFactor=AFactor;
	carrierParams->KFactor=KFactor;
	carrierParams->Thickness=thikness;
	carrierParams->CBRatio=cbRatio;
	carrierParams->CurrentIntensity =currentIntensity;

	long double m0=9.10938188E-31; // ����� ��������� ���� ��� � �� ����� � �����-��:)
	long double mh=0.443*m0; // ����� ������� �����
	long double ml=0.001*m0; // ����� ������ �����

	carrierParams->Concentration[1]=pow(ml/mh,1.5)*carrierParams->Concentration[0]*50; // ������������ ������ ����� ��������� �� 50
	carrierParams->Concentration[2]=-pow(niSob(carrierParams->CurrentTemperature,
	 carrierParams->MolarCompositionCadmium),2)/carrierParams->Concentration[0];

	carrierParams->Mobility[2]=-carrierParams->AFactor*pow(carrierParams->CurrentTemperature/77.0,-carrierParams->KFactor);
	carrierParams->Mobility[1]=-0.1*carrierParams->Mobility[2];
	carrierParams->Mobility[0]=-0.005*carrierParams->Mobility[2];
}

void ParamsKRT(void)
{
	calcutatingCarrierParams(&carrierParams,Form1->eMolarCompositionCadmium->Text.ToDouble(),
	Form1->eTemperature->Text.ToDouble(),Form1->eHeavyHoleConcentration->Text.ToDouble(),
	Form1->eAFactor->Text.ToDouble(),Form1->eKFactor->Text.ToDouble(),
	Form1->eSampleThickness->Text.ToDouble(),Form1->eCBRatio->Text.ToDouble(),
	Form1->eCurrentIntensity->Text.ToDouble());

	Form1->g_Nz_par->Cells[1][1]=FloatToStr(carrierParams.Concentration[2]);
	Form1->g_Nz_par->Cells[2][1]=FloatToStr(carrierParams.Mobility[2]);
	Form1->g_Nz_par->Cells[1][2]=FloatToStr(carrierParams.Concentration[1]);
	Form1->g_Nz_par->Cells[2][2]=FloatToStr(carrierParams.Mobility[1]);
	Form1->g_Nz_par->Cells[1][3]=FloatToStr(carrierParams.Concentration[0]);
	Form1->g_Nz_par->Cells[2][3]=FloatToStr(carrierParams.Mobility[0]);

	Form1->bCalculateCarrierParams->Enabled=true;
	Form1->BuildingPlots->Enabled=1;

	//--------------------------------------------------------------------------
}



void __fastcall TForm1::BuildingPlotsClick(TObject *Sender)
{
Form2->Show();
}
//---------------------------------------------------------------------------


// ������ �������� �� ������ ���������� ��������
void __fastcall TForm1::tenzorCalculatingClick(TObject *Sender)
{

LineSeries3->Clear();
LineSeries9->Clear();
LineSeries1->Clear();
LineSeries7->Clear();
Series3->Clear();
Series4->Clear();
Series5->Clear();
LineSeries5->Clear();

getEffectiveParamsFromSignals(&ParamsWithNoise,&carrierParams);
getTenzorFromEffectiveParams(&ParamsWithNoise);

constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Us,NumberOfPoints,LineSeries3,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Uy,NumberOfPoints,LineSeries9,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.s_eff,NumberOfPoints,LineSeries1,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Rh_eff,NumberOfPoints,LineSeries7,clRed);

constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.sxx,NumberOfPoints,Series5,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.sxy,NumberOfPoints,LineSeries5,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Us,NumberOfPoints,Series3,clRed);
constructPlotFromTwoMassive(ParamsWithNoise.B,ParamsWithNoise.Uy,NumberOfPoints,Series4,clRed);

// ���������� ��� ��� ���������� ��������

long double sko1=Sko(IdealParams.sxx,ParamsWithNoise.sxx,NumberOfPoints);
long double sko2=Sko(IdealParams.sxy,ParamsWithNoise.sxy,NumberOfPoints);
long double shx[NumberOfPoints]={0};
long double shy[NumberOfPoints]={0};
for(int i=0;i<NumberOfPoints;i++)
{
	shx[i]=ParamsWithNoise.sxx[i]-IdealParams.sxx[i];
	shy[i]=ParamsWithNoise.sxy[i]-IdealParams.sxy[i];
}
long double mx=Mo(shx,NumberOfPoints);
long double my=Mo(shy,NumberOfPoints);
Edit1->Text=FloatToStr(sko1);
Edit2->Text=FloatToStr(sko2);
Edit3->Text=FloatToStr(mx/Mo(IdealParams.sxx,NumberOfPoints)*100);
Edit4->Text=FloatToStr(my/Mo(IdealParams.sxy,NumberOfPoints)*100);
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bAutomaticCalculationClick(TObject *Sender)
{
/*
+1. ������ ������ ���.
+2. ���� �� ����������� � �����.
+3. ������������ ��������� ������ � ������� ������������
+4. ��������� ��������� �����.
+4. ��������� ��������� ������ ������� ��� ����� � 11 �����.
+5. ��������� ��������� ������ ���������� �������� Us � Uy ��� ����� � 11 �����
+6. ��������� ������������ ��������� ����� (������������ � ����������� ��).
+7. ������ ����� �������.

// ��� ������� �������� ����.
���������� ����������� ������.
+8. ��������� ��� ����� � 11 ����� ������������ ����������� �������.
+9. ��������� ����������.
+10. ��������� ��� ����� �������������� �������.
+11. ��������� �������� ������ ��� ������������ ������� (����� ���� �������� ����� �������� ��� � ��� "��� ����"?).
������� ����� �� ���������, �.�. �� ����� ���������� �� ���������� ��� ��������, � ������������ �� ��������.

+12. ��������� ��� ����� � 11 ����� ����������� ��������� �������.
13. ������������ ������������� ���������� ������, ��� ����� �� ���� �������� ��� ��������.
+14. ��������� �������� ������ ��� �������������� ������������ �������.
+15. ��������� ��� ����� � 11 ����� ������������� ��������� �������.

*/
silentMode=true;
int h=eStepByTemperature->Text.ToInt(); // ��� �� �����������
int T1=80; // ��������� �����������
int Tmax=300; // �������� �����������

int koef=1;// ��������� ����������� ����
int endkoef=5;  // �������� �����������

int h_koef=1; // ��� �� ������ ����
eFilterLength->Text=IntToStr(80); // ����� ����� �������, �������� - ��� ��� ������������� � ����������� �������!
g_hsize->Text=FloatToStr(0.0125); // ��� �� ���������� ����.

UnicodeString standartName; // ��������� ��� �����
UnicodeString fName;   // ����� ��� �����

// ���� � ������� ��� ���� �������
if (sg1->Execute()) {
	mDebug->Lines->Add( sg1->FileName); // ������� ��� � ����
}
standartName = sg1->FileName;     // � ����������

for (int T=T1; T <= Tmax; T+=h)  // ���� �� ����������� � �������� �����
{
     eTemperature->Text=IntToStr(T);  // ��� ������� ��������� �������� �����������
	 bCalculateCarrierParams->Click(); // �������� ������ ����������
     // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
	 sg1->FileName=standartName+fName;
	 bSaveFilmParams->Click(); // ��������� ��������� �����


	 rbLeftPlot->Checked=true; // �������� ��������� ������ ��������
	 // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=standartName+fName;
	 bSaveAllPoints->Click();     	 // ��������� ��� ����� ���������� ������� �������
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=standartName+fName;
	 // ��������� 11 ����� ���������� ������� �������
	 bSaveElevenPoints->Click();


	 rbIdealPlot->Checked=true; // �������� ��������� ������ ���������� ��������
     // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=standartName+fName;
	 bSaveAllPoints->Click();     	 // ��������� ��� ����� ���������� ������� ��������
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=standartName+fName;
	 // ��������� 11 ����� ���������� ������� ��������
	 bSaveElevenPoints->Click();

     for (int i=koef; i <= endkoef; i+=h_koef) // ����� ���� ���������� ���� � ��������������
	 {
		  Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams.Uy[NumberOfPoints-1]))); // ������ ������������
		  Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams.Uy[NumberOfPoints-1])));
		  bGaussianNoiseGenerator->Click(); // ��������� ����

		  rbRightPlot->Checked=true; // �������� ����������� ������ ����������� �������
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // ��������� ��� ����� ���������� ������� �������
		 fName="T_"+IntToStr(T)+"_Us_Uy_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 // ��������� 11 ����� ���������� ������� �������
		 bSaveElevenPoints->Click();

		 // ��������� ����������
		 bFilteringPlots->Click();

		 // ��������� ���������� ���������� (��� �����)-------------------------
		 rbFilteredPlot->Checked=true; // �������� ������������� ������
		 fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();
         //---------------------------------------------------------------------

		  tenzorCalculating->Click(); // �������� ������
		  //--------------------------------------------------------------------
		  // ���� ��������� ���������� ����������
		  rbRightPlot->Checked=1;
		  fName="T_"+IntToStr(T)+"_tenzor_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=standartName+fName;
		  bSaveAllPoints->Click();// ������ ���������
		  fName="T_"+IntToStr(T)+"_tenzor_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=standartName+fName;
		  // ��������� 11 ����� �������
		  bSaveElevenPoints->Click();
		  //--------------------------------------------------------------------
		  //-------��������� ����������� ����������-----------------------------

		  //1T_80_Us_Uy_vseZnachenia_filtr_k_50.txt
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		  sg1->FileName=standartName+fName;
		  rbIdealPlot->Checked=1;
		  bLoadingPlots->Click();

		  //-------�������� ������ ��� ������������� ��������
		  tenzorCalculating->Click();
		  //--------------------------------------------------------------------
          //--------------------------------------------------------------------
		  rbRightPlot->Checked=1;// ��������� ���������� ������������� ��������
		  fName="T_"+IntToStr(T)+"_tenzor_filt_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=standartName+fName;
		  bSaveAllPoints->Click();
		  // � ���� ��������: ��� ����� � 11 �����.
		  fName="T_"+IntToStr(T)+"_tenzor_filt_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=standartName+fName;
		  bSaveElevenPoints->Click();
		  //--------------------------------------------------------------------
	 }
}
silentMode=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bFilteringPlotsClick(TObject *Sender)
{
// ���� �������� ������������� �������� ���������� ������������!

//int length=Edit8->Text.ToInt();

/*if (l!=Series3->YValues->Count) {
	ShowMessage("����� ������� �� ��!");
	return;
}     */
 /*double *x=new double[2*l];
 double *y=new double[2*l];



 for (int i = 0; i < l; i++) {
	y[i]=Series3->YValues->Value[l-i-1];
	x[i]=-Series3->XValues->Value[l-i-1];
	y[i+l]=Series3->YValues->Value[i];
	x[i+l]=Series3->XValues->Value[i];
 }
 Series3->Clear();

 for (int i = 0; i < 2*l; i++) {
	Series3->AddXY(x[i],y[i],"",clRed);
 }

 for (int i = 0; i < l; i++) {
	y[i]=-Series4->YValues->Value[l-i-1];
	x[i]=-Series4->XValues->Value[l-i-1];
	y[i+l]=Series4->YValues->Value[i];
	x[i+l]=Series4->XValues->Value[i];
 }
 Series4->Clear();

 for (int i = 0; i < 2*l; i++) {
	Series4->AddXY(x[i],y[i],"",clRed);
 }

	*/

 long double *x=new long double[2*NumberOfPoints];
 long double *y=new long double[2*NumberOfPoints];

 long double *x2=new long double[2*NumberOfPoints];
 long double *y2=new long double[2*NumberOfPoints];



 for (int i = 0; i < NumberOfPoints; i++) {
	y[i]=ParamsWithNoise.Us[NumberOfPoints-i-1];
	x[i]=-ParamsWithNoise.B[NumberOfPoints-i-1];
	y[i+NumberOfPoints]=ParamsWithNoise.Us[i];
	x[i+NumberOfPoints]=ParamsWithNoise.B[i];
 }

 Series7->Clear();
 Series8->Clear();

 TrForMassiveFilter(x,y,x2,y2,2*NumberOfPoints,eFilterLength->Text.ToInt(),5000,15,25);

 for (int i = 0; i < NumberOfPoints; i++)
 {
	FilteredParams.Us[i]=y2[i+NumberOfPoints];
	FilteredParams.B[i]=x2[i+NumberOfPoints];
 }

 //TrForMassiveFilter(ParamsWithNoise.B,ParamsWithNoise.Us,FilteredParams.B,
 //	FilteredParams.Us,NumberOfPoints,eFilterLength->Text.ToInt(),5000,15,25);
 TrForMassiveFilter(ParamsWithNoise.B,ParamsWithNoise.Uy,FilteredParams.B,
	FilteredParams.Uy,NumberOfPoints,eFilterLength->Text.ToInt(),5000,15,25);

 getEffectiveParamsFromSignals(&FilteredParams,&carrierParams);
 getTenzorFromEffectiveParams(&FilteredParams);

 constructPlotFromTwoMassive(FilteredParams.B,FilteredParams.Us,NumberOfPoints,Series7,clBlue);
 constructPlotFromTwoMassive(FilteredParams.B,FilteredParams.Uy,NumberOfPoints,Series8,clBlue);

 delete [] x;
 delete [] y;
 delete [] x2;
 delete [] y2;


  //extrapolate2Degree(Series3, 0, 2.75, 0.01,Series3);
  //extrapolate2Degree(Series4, 0, 2.75, 0.01,Series4);


//Tr_Filter(Series3,Series7,eFilterLength->Text.ToInt(),5000,15,25);
//Tr_Filter(Series4,Series8,eFilterLength->Text.ToInt(),5000,15,25);

}



void __fastcall TForm1::bSaveElevenPointsClick(TObject *Sender)
{
TLineSeries* Saving1=0;
TLineSeries* Saving2=0;
if(rbLeftPlot->Checked)
{
 Saving1=Series1;
  Saving2=Series2;
}
if(rbRightPlot->Checked)
{
 Saving1=Series3;
  Saving2=Series4;
}
if(rbFilteredPlot->Checked)
{
 Saving1=Series7;
  Saving2=Series8;
}

if(rbIdealPlot->Checked)
{
 Saving1=LineSeries3;
  Saving2=LineSeries9;
}

if(!Saving1)
return;

double points[11]={0};
double shag=0.2;
for (int i=1; i < 11; i++) {
    points[i]=points[i-1]+shag;
}

TStringList * tsl=new TStringList();
wchar_t *s;

int length=Saving1->XValues->Count;

 //int counting=0;
for (int i = 0; i < 11; i++) {
	int index=0;
	double r=4;
	for(int k=0;k<length;k++)
	{
		if(fabs(fabs(Saving1->XValues->Value[k])-fabs(points[i]))<=r)
		{
			r=fabs(Saving1->XValues->Value[k]-points[i]);
			index=k;
		}
	}
		
	tsl->Add(FloatToStr(Saving1->XValues->Value[index])+"\t"+Saving1->YValues->Value[index]+"\t"+Saving2->YValues->Value[index]);
	s=tsl->Strings[i].w_str() ;
	for(int j=0;j<tsl->Strings[i].Length();j++)
	if(s[j]==',')
		s[j]='.';
	tsl->Delete(i);
	tsl->Add(s);

	
}

if (silentMode || sg1->Execute()) {
tsl->SaveToFile(sg1->FileName);
}
delete tsl;
}
//---------------------------------------------------------------------------

void LoadingDataFromFile(TLineSeries * Series1,TLineSeries * Series2)
{
TStringList * tls=new TStringList();
if (silentMode || Form1->sg1->Execute()) {
tls->LoadFromFile(Form1->sg1->FileName);
Series1->Clear();
	  Series2->Clear();

for(int i=0;i<tls->Count;i++) // �� ���������� �����
	  {

	  if(tls->Strings[i].IsEmpty()) // ������ ������ ����������
	  continue;
	  String s = tls->Strings[i];

	  for (int k = 1; k <= s.Length(); k++) {
		  if(s[k]==L'.')
		  s[k]=L',';
	  }

	  String s1=wcstok(s.c_str(),L" \t");
	  String s2=wcstok(NULL,L" \t");
	  String s3=wcstok(NULL,L" \t");



	  Series1->AddXY(s1.ToDouble(), // ������ ����� �� ������� ��� �, ������ ����� �
	  s2.ToDouble(),"",clRed);
	  Series2->AddXY(s1.ToDouble(), // ������ ����� �� ������� ��� �, ������ ����� �
	  s3.ToDouble(),"",clRed);

	  }

}
delete tls;
}

void __fastcall TForm1::bLoadingPlotsClick(TObject *Sender)
{

if (rbLeftPlot->Checked) // �����
{
	LoadingDataFromFile(Series1,Series2);
}
if (rbRightPlot->Checked) // ������
{
	LoadingDataFromFile(Series3,Series4);
	for (int i=0; i < NumberOfPoints; i++)
	{
		Us_withNoise[i]=Series3->YValues->Value[i];
		Uy_withNoise[i]=Series4->YValues->Value[i];
	}
}
if (rbFilteredPlot->Checked) // �������������
{
	LoadingDataFromFile(Series7,Series8);
}

if (rbIdealPlot->Checked) // ���������
{
	LoadingDataFromFile(LineSeries3,LineSeries9);    //------------------------------!
	int length=LineSeries3->YValues->Count;
	long double *x=new long double[length];
	long double *y=new long double[length];

	 for (int i=0; i < length; i++) {
		 x[i]=LineSeries3->XValues->Value[i];
		 y[i]=LineSeries3->YValues->Value[i];
	 }
	 LineSeries3->Clear();
	 for (int i=0; i < length; i++) {
	 if (x[i]>=0) {
		 LineSeries3->AddXY(x[i],y[i],"",clRed);

     }
	 }
	  for (int i=0; i < length; i++) {
		 x[i]=LineSeries9->XValues->Value[i];
		 y[i]=LineSeries9->YValues->Value[i];
	 }
	 LineSeries9->Clear();
	 for (int i=0; i < length; i++) {
	 if (x[i]>=0) {
		 LineSeries9->AddXY(x[i],y[i],"",clRed);

     }
	 }
     //ShowMessage(LineSeries3->YValues->Count);

 for (int i=0; i <NumberOfPoints /*LineSeries3->YValues->Count*/; i++) { //----------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Us_withNoise[i]=LineSeries3->YValues->Value[i];
	Uy_withNoise[i]=LineSeries9->YValues->Value[i];
 }


 delete[] x;
 delete[] y;
}

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
SavingAllPoints(Series7,Series8);
}
//---------------------------------------------------------------------------
// ������� ��������� ��� ����� ���� ��������
// �.�. ������������� � ������ ������� ��� �������� ������������.
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8)
{


bool isRoundNeeded = false;


if(!(Series7->YValues->Count || Series8->YValues->Count))
{
 ShowMessage("������ ����!");
 return;
}
if(Series7->YValues->Count!=Series8->YValues->Count)
{
 ShowMessage("������ ���������� ����� �� ��������!");
 return;
}
long double * x = new long double[Series7->YValues->Count];
// ���������� ���������� �����������
for(int i=0;i<Series7->YValues->Count;i++)
{
	for(int j=0;j<Series7->YValues->Count;j++)
		x[j]=Series7->YValues->Value[j];
	if (isRoundNeeded) {
	RoundM(x,Series7->YValues->Count);
	}

    for(int j=0;j<Series7->YValues->Count;j++)
		Series7->YValues->Value[j]=x[j];
	for(int j=0;j<Series8->YValues->Count;j++)
		x[j]=Series8->YValues->Value[j];
	if (isRoundNeeded) {
	RoundM(x,Series8->YValues->Count);
    }
	for(int j=0;j<Series8->YValues->Count;j++)
		Series8->YValues->Value[j]=x[j];
}

TStringList * tsl=new TStringList();
wchar_t *s;
for(int i=0;i<Series7->YValues->Count;i++)
{
tsl->Add(FloatToStr(Series7->XValues->Value[i])+"\t"+Series7->YValues->Value[i]+"\t"+Series8->YValues->Value[i]);
s=tsl->Strings[i].w_str() ;
for(int j=0;j<tsl->Strings[i].Length();j++)
if(s[j]==',')     // �������� ��� ������� �� �����
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);

}
// ���� ������� ����� ����� - ��� ��� ������ ���� ��������
if (silentMode || Form1->sg1->Execute()) {
tsl->SaveToFile(Form1->sg1->FileName);
}
delete[] x;
delete tsl;
}

// ���������� ���������� ���
// ������������ ����� �����
void __fastcall TForm1::bSaveFilmParamsClick(TObject *Sender)
{
TStringList * tsl=new TStringList();
wchar_t *s;
for (int i = 0; i < NumberOfCarrierTypes; i++) {
tsl->Add(g_Nz_par->Cells[1][i+1] +"\t"+g_Nz_par->Cells[2][i+1]);
s=tsl->Strings[i].w_str() ;
for(int j=0;j<tsl->Strings[i].Length();j++)
if(s[j]==',')
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);
}
if (silentMode || Form1->sg1->Execute()) {
tsl->SaveToFile(Form1->sg1->FileName);
}
delete tsl;
}



void __fastcall TForm1::bTestingSomethingClick(TObject *Sender)
{



extrapolate5Degree(Series2, 0, 2.5, 0.2,Series4);



}
//---------------------------------------------------------------------------





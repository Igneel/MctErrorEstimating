//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
#include "ExtrapolateUnit.h"
#include "FilteringUnit.h"
#include "NoiseUnit.h"
#include "clMagneticFieldDependences.h"
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




bool silentModeEnabled=false; // ���� ���� ����� ��� - �� ������� ���������� �� ���������� ��� �����
// ���� ������������ ��� ������� ��������))

/*

TO DO:

1. ����� ����������� ���������� ���������� ���������� ������� � ������������� ������� ���������� ����.+
2. ����������� ������������ ������������� ����� ����������.+
3. ������ ����� ������� ������������ �������� ���������� ����� (0,0) �� ��������� ��������.
4. ��������� �������� �����, ���������� ����� ������ ���������.
5. ������� ������� ���������� � �������� ������.


�� ������ ������ ���, ���������� �� ������ ���������� ��������� � �������������� �������.

*/

 // */

const int NumberOfPoints=201;//11;  // ���������� �����
const int NumberOfNumbersAfterPoint=4; // �������� ����������.

const int NumberOfCarrierTypes=3;
long double h; // �������� ����


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//------------------------------------------------------------------------------

// �� ����� �������.
clMagneticFieldDependences *IdealParams=0;
clMagneticFieldDependences *ParamsWithNoise=0;
clMagneticFieldDependences *FilteredParams=0;
clMagneticFieldDependences *ExtrapolatedParams=0;


//------------------------------------------------------------------------------
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


void ParamsKRT(void)
{

	h=Form1->g_hsize->Text.ToDouble(); // ��������� �������� ����

	//--------------------------������------------------------------------------
	IdealParams=new clMagneticFieldDependences(NumberOfPoints,h,
	Form1->eMolarCompositionCadmium->Text.ToDouble(),
	Form1->eTemperature->Text.ToDouble(),Form1->eHeavyHoleConcentration->Text.ToDouble(),
	Form1->eAFactor->Text.ToDouble(),Form1->eKFactor->Text.ToDouble(),
	Form1->eSampleThickness->Text.ToDouble(),Form1->eCBRatio->Text.ToDouble(),
	Form1->eCurrentIntensity->Text.ToDouble(),NumberOfCarrierTypes);

	Form1->g_Nz_par->Cells[1][1]=FloatToStr(IdealParams->carrierParams->getConcentration(2));
	Form1->g_Nz_par->Cells[2][1]=FloatToStr(IdealParams->carrierParams->getMobility(2));
	Form1->g_Nz_par->Cells[1][2]=FloatToStr(IdealParams->carrierParams->getConcentration(1));
	Form1->g_Nz_par->Cells[2][2]=FloatToStr(IdealParams->carrierParams->getMobility(1));
	Form1->g_Nz_par->Cells[1][3]=FloatToStr(IdealParams->carrierParams->getConcentration(0));
	Form1->g_Nz_par->Cells[2][3]=FloatToStr(IdealParams->carrierParams->getMobility(0));
	//--------------------------������------------------------------------------

	//Form1->bCalculateCarrierParams->Enabled=true;
	Form1->BuildingPlots->Enabled=1;

	//--------------------------------------------------------------------------
}

// ������ �������� ������������
void __fastcall TForm1::bCalculateCarrierParamsClick(TObject *Sender)
{
	if(IdealParams!=0)
	delete IdealParams;
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

	//------------------������--------------------------------------------------
	IdealParams->calculateMagneticFieldDependences();

	IdealParams->constructPlotFromTwoMassive(SXX,Series1,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,Series2,clRed);

	IdealParams->constructPlotFromTwoMassive(SXX,gSeriesIdealParamsSxx,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,gSeriesIdealParamsSxy,clRed);

	IdealParams->constructPlotFromTwoMassive(S_EFF,gSeriesIdealParamsS_eff,clRed);
	IdealParams->constructPlotFromTwoMassive(RH_EFF,gSeriesIdealParamsRh_eff,clRed);

	IdealParams->constructPlotFromTwoMassive(US,gSeriesIdealParamsUs,clRed);
	IdealParams->constructPlotFromTwoMassive(UY,gSeriesIdealParamsUy,clRed);

	//------------------������--------------------------------------------------
	bGaussianNoiseGenerator->Enabled=1;
}
//---------------------------------------------------------------------------
// ���������� ����������� ��� ����������, ���������� ������ ��� ���� ����� �� �������
// ����� ����� ������� ��� �� � ���� ��������� �������� - � ���� ��� ������
void TForm1::RoundM(long double * x,int length)
{
	int S=pow(10,NumberOfNumbersAfterPoint);
	for(int i=0;i<length;i++)
	{
		int n=(int)(x[i]*S)%10;
		if(n<5)
			x[i]=floorl(x[i]*S)/S;
		else
			x[i]=ceill(x[i]*S)/S;
	}
}


// ��������� ����
void __fastcall TForm1::bGaussianNoiseGeneratorClick(TObject *Sender)
{
	/*
	� ��������� ��������� ������� ����������� ���.
	��������� �� �����.
	*/
	//--------------------������----------------------------------------------------

	srand(time(NULL));

	long double vz[6]={0};  // � ��� � ��� � %
	if(ParamsWithNoise!=0)
		delete ParamsWithNoise;
	ParamsWithNoise= new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	ParamsWithNoise->modifySignals(ShumAdding,IdealParams->getSignalUs(),IdealParams->getSignalUy(),vz,Edit5->Text.ToDouble());

	mDebug->Lines->Add(FloatToStr(vz[0]));
	Edit1->Text=FloatToStr(vz[1]); // ���
	Edit3->Text=FloatToStr(vz[2]);

	mDebug->Lines->Add(FloatToStr(vz[3]));
	Edit2->Text=FloatToStr(vz[4]); // ���
	Edit4->Text=FloatToStr(vz[5]);

	ParamsWithNoise->constructPlotFromTwoMassive(US,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,Series4,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(US,gSeriesParamsWithNoiseUs,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,gSeriesParamsWithNoiseUy,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(S_EFF,gSeriesParamsWithNoiseS_eff,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(RH_EFF,gSeriesParamsWithNoiseRh_eff,clBlack);

	ParamsWithNoise->constructPlotFromTwoMassive(SXX,gSeriesParamsWithNoiseSxx,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,gSeriesParamsWithNoiseSxy,clBlack);
	ParamsWithNoise->constructPlotFromTwoMassive(SXX,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,Series4,clRed);

    // ������ ��� � ���. ��������.

	long double sko1=Sko(IdealParams->getSxx(),ParamsWithNoise->getSxx(),NumberOfPoints);
	long double sko2=Sko(IdealParams->getSxy(),ParamsWithNoise->getSxy(),NumberOfPoints);
	long double shx[NumberOfPoints]={0};
	long double shy[NumberOfPoints]={0};
	for(int i=0;i<NumberOfPoints;i++)
	{
		shx[i]=ParamsWithNoise->getSxx()[i]-IdealParams->getSxx()[i];
		shy[i]=ParamsWithNoise->getSxy()[i]-IdealParams->getSxy()[i];
	}
	long double mx=Mo(shx,NumberOfPoints);
	long double my=Mo(shy,NumberOfPoints);
	Edit1->Text=FloatToStr(sko1);
	Edit2->Text=FloatToStr(sko2);
	Edit3->Text=FloatToStr(mx/Mo(IdealParams->getSxx(),NumberOfPoints)*100);
	Edit4->Text=FloatToStr(my/Mo(IdealParams->getSxy(),NumberOfPoints)*100);


//-----------------------------������_------------------------------------------
}
//---------------------------------------------------------------------------
// ���������� �����������
void __fastcall TForm1::bSaveAllPointsClick(TObject *Sender)
{

	// ��������� ������
	if (rbIdealUPlot->Checked)
	{
		 SavingAllPoints(gSeriesIdealParamsUs,gSeriesIdealParamsUy);
	}

	if (rbIdealTenzorPlot->Checked)
	{
		SavingAllPoints(gSeriesIdealParamsSxx,gSeriesIdealParamsSxy);
	}

	// ����������� ������
	if (rbNoisyU->Checked)
	{
		SavingAllPoints(gSeriesParamsWithNoiseUs,gSeriesParamsWithNoiseUy);
	}

	if(rbNoisyTenzor->Checked)
	{
		SavingAllPoints(gSeriesParamsWithNoiseSxx,gSeriesParamsWithNoiseSxy);
	}

	// ������������� ������
	if (rbFilteredUPlot->Checked)
	{
		 SavingAllPoints(gSeriesFilteredParamsUs,gSeriesFilteredParamsUy);
	}

	if (rbFilteredTenzor->Checked)
	{
		SavingAllPoints(gSeriesFilteredParamsSxx,gSeriesFilteredParamsSxy);
	}
    // ������������������ ������
	if (rbExtrapolatedU->Checked) {
		SavingAllPoints(gSeriesExtrapolatedParamsUs,gSeriesExtrapolatedParamsUy);
	}

	if (rbExtrapolatedTenzor->Checked) {
        SavingAllPoints(gSeriesExtrapolatedParamsSxx,gSeriesExtrapolatedParamsSxy);
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
long double ownConcentrationOfElectrons(long double T, long double x)
{
	long double k=1.380648813E-23/1.60217646E-19; // ���������� ��������� � ��������-�������
	long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2.0*x)*T;
	return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2./k/T); // ����������� ������������
}






void __fastcall TForm1::BuildingPlotsClick(TObject *Sender)
{
	Form2->Show();
}
//---------------------------------------------------------------------------


void TForm1::automaticCalculationHelper(UnicodeString SaveFileName)
{
	/*

	� SaveFileName ����� ���������� _Us_Uy_vseZnachenia_k_"+IntToStr(i) � �.�.

	*/

	long double sko_xx=StrToFloat(Edit3->Text);
	RoundM(&sko_xx,1);
	long double sko_xy=StrToFloat(Edit4->Text);
	RoundM(&sko_xy,1);
	UnicodeString standartName; // ��������� ��� �����
	standartName = sg1->FileName;     // ���������� ���
	sg1->FileName=standartName+"T_"+eTemperature->Text+"_"+
		SaveFileName+"_sko_p_xx"+FloatToStr(sko_xx)+ "_sko_p_xy"+FloatToStr(sko_xy)+".txt";

	bSaveAllPoints->Click();
	sg1->FileName=ReplaceStr(Form1->sg1->FileName,"vseZnachenia","11Znacheniy");
	bSaveElevenPoints->Click();

	sg1->FileName=standartName;
}


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

	+12. ��������� ��� ����� � 11 ����� ����������� ��������� �������.
	+15. ��������� ��� ����� � 11 ����� ������������� ��������� �������.
	+16. � ����� ������������������ �������� � ��������.

	*/
	silentModeEnabled=true;
	int h=eStepByTemperature->Text.ToInt(); // ��� �� �����������
	int T1=80; // ��������� �����������
	int Tmax=300; // �������� �����������

	int koef=1;// ��������� ����������� ����
	int endkoef=5;  // �������� �����������

	int h_koef=1; // ��� �� ������ ����
	eFilterLength->Text=IntToStr(50); // ����� ����� �������, �������� - ��� ��� ������������� �������!
	g_hsize->Text=FloatToStr(0.01); // ��� �� ���������� ����., ������ ������ ���� ������ �� 2 ��.

	UnicodeString standartName; // ��������� ��� �����
	UnicodeString fName;   // ����� ��� �����

	// ���� � ������� ��� ���� �������
	if (sg1->Execute()) {
		mDebug->Lines->Add( sg1->FileName); // ������� ��� � ����
	}
	else
	{
		return;
	}

	standartName = sg1->FileName;     // ���������� ���

	for (int T=T1; T <= Tmax; T+=h)  // ���� �� ����������� � �������� �����
	{
		 eTemperature->Text=IntToStr(T);  // ��� ������� ��������� �������� �����������
		 bCalculateCarrierParams->Click(); // �������� ������ ����������

		 // ����� ��� �����
		 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveFilmParams->Click(); // ��������� ��������� �����

         rbIdealTenzorPlot->Checked=true; // �������� ��������� ������ ��������
		 // ����� ��� �����
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia.txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // ��������� ��� ����� ���������� ������� �������
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy.txt";
		 sg1->FileName=standartName+fName;
		 // ��������� 11 ����� ���������� ������� �������
		 bSaveElevenPoints->Click();


		 rbIdealUPlot->Checked=true; // �������� ��������� ������ ���������� ��������
		 // ����� ��� �����
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia.txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // ��������� ��� ����� ���������� ������� ��������
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy.txt";
		 sg1->FileName=standartName+fName;
		 // ��������� 11 ����� ���������� ������� ��������
		 bSaveElevenPoints->Click();
		 sg1->FileName=standartName;
		 for (int i=koef; i <= endkoef; i+=h_koef) // ����� ���� ���������� ���� � ��������������
		 {
			Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1]))); // ������ ������������
			Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1])));
			bGaussianNoiseGenerator->Click(); // ��������� ����
			bFilteringPlots->Click(); // ��������� ����������

			sg1->FileName=standartName;
			rbNoisyU->Checked=true; // �������� ����������� ������ ����������� �������
			fName="Us_Uy_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			// ���� ��������� ����������� ����������
			rbNoisyTenzor->Checked=true;
			sg1->FileName=standartName;
			fName="tenzor_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			// ��������� ���������� ���������� (��� �����)-------------------------
			rbFilteredUPlot->Checked=true; // �������� ������������� ������
			sg1->FileName=standartName;
			fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
			sg1->FileName=standartName+fName;
			bSaveAllPoints->Click();
			// ��������� 11 ����� �� ������ ������ ������ ���.
			//---------------------------------------------------------------------
			rbFilteredTenzor->Checked=true;// ��������� ���������� ������������� ��������
			sg1->FileName=standartName;
			fName="tenzor_filt_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
			rbExtrapolatedU->Checked=true;
			sg1->FileName=standartName;
			fName="Us_Uy_Extrapolated_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
			rbExtrapolatedTenzor->Checked=true;
			sg1->FileName=standartName;
            fName="tenzor_Extrapolated_vseZnachenia_k_"+IntToStr(i);
			automaticCalculationHelper(fName);
			//--------------------------------------------------------------------
		 }
	}
	silentModeEnabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bFilteringPlotsClick(TObject *Sender)
{

	//------------------������-----------------------------------------------------
	if(FilteredParams!=0)
	delete FilteredParams;

	FilteredParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	FilteredParams->modifySignals(TrForMassiveFilter,ParamsWithNoise->getSignalUs(),ParamsWithNoise->getSignalUy(),eFilterLength->Text.ToInt());

	FilteredParams->constructPlotFromTwoMassive(US,gSeriesFilteredUs,clBlue);
	FilteredParams->constructPlotFromTwoMassive(UY,gSeriesFilteredUy,clBlue);

	FilteredParams->constructPlotFromTwoMassive(US,gSeriesFilteredParamsUs,clBlue);
	FilteredParams->constructPlotFromTwoMassive(UY,gSeriesFilteredParamsUy,clBlue);

	FilteredParams->constructPlotFromTwoMassive(S_EFF,gSeriesFilteredParamsS_eff,clBlue);
	FilteredParams->constructPlotFromTwoMassive(RH_EFF,gSeriesFilteredParamsRh_eff,clBlue);

	FilteredParams->constructPlotFromTwoMassive(SXX,gSeriesFilteredParamsSxx,clBlue);
	FilteredParams->constructPlotFromTwoMassive(SXY,gSeriesFilteredParamsSxy,clBlue);

	//------------------������-----------------------------------------------------

	// � ��������������.
	if(ExtrapolatedParams!=0)
	delete ExtrapolatedParams;

	ExtrapolatedParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	FilteredParams->modifySignals(EXTRAPOLATE,ExtrapolatedParams);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(US,gSeriesExtrapolatedParamsUs,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(UY,gSeriesExtrapolatedParamsUy,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(S_EFF,gSeriesExtrapolatedParamsS_eff,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(RH_EFF,gSeriesExtrapolatedParamsRh_eff,clGreen);

	ExtrapolatedParams->constructPlotFromTwoMassive(SXX,gSeriesExtrapolatedParamsSxx,clGreen);
	ExtrapolatedParams->constructPlotFromTwoMassive(SXY,gSeriesExtrapolatedParamsSxy,clGreen);
}



void __fastcall TForm1::bSaveElevenPointsClick(TObject *Sender)
{

	// �������� � ���������
	TLineSeries* Saving1=0;
	TLineSeries* Saving2=0;

	// ��������� ������
	if (rbIdealUPlot->Checked)
	{
		Saving1=gSeriesIdealParamsUs;
		Saving2=gSeriesIdealParamsUy;
	}

	if (rbIdealTenzorPlot->Checked)
	{
		Saving1=gSeriesIdealParamsSxx;
		Saving2=gSeriesIdealParamsSxy;
	}

	// ����������� ������
	if (rbNoisyU->Checked)
	{
		Saving1=gSeriesParamsWithNoiseUs;
		Saving2=gSeriesParamsWithNoiseUy;
	}

	if(rbNoisyTenzor->Checked)
	{
		Saving1=gSeriesParamsWithNoiseSxx;
		Saving2=gSeriesParamsWithNoiseSxy;
	}

	// ������������� ������
	if (rbFilteredUPlot->Checked)
	{
		 Saving1=gSeriesFilteredParamsUs;
		 Saving2=gSeriesFilteredParamsUy;
	}

	if (rbFilteredTenzor->Checked)
	{
		Saving1=gSeriesFilteredParamsSxx;
		Saving2=gSeriesFilteredParamsSxy;
	}
    // ������������������ ������
	if (rbExtrapolatedU->Checked)
	{
		Saving1=gSeriesExtrapolatedParamsUs;
		Saving2=gSeriesExtrapolatedParamsUy;
	}

	if (rbExtrapolatedTenzor->Checked)
	{
		Saving1=gSeriesExtrapolatedParamsSxx;
		Saving2=gSeriesExtrapolatedParamsSxy;
	}

	if(!Saving1)
		return;

	double points[11]={0};
	double shag=0.2;
	for (int i=1; i < 11; i++) {
		points[i]=points[i-1]+shag;
	}

	TStringList * tsl=new TStringList();

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
	}
	tsl->Text=ReplaceTextW(tsl->Text,",","."); // �������� ��� ������� �� �����

	if (silentModeEnabled || sg1->Execute()) {
	tsl->SaveToFile(sg1->FileName);
	}
	delete tsl;
}
//---------------------------------------------------------------------------

void LoadingDataFromFile(TLineSeries * Series1,TLineSeries * Series2)
{
	// �������� � ���������, �� �������� ��������.
	TStringList * tsl=new TStringList();
	if (silentModeEnabled || Form1->sg1->Execute()) {
	tsl->LoadFromFile(Form1->sg1->FileName);
	Series1->Clear();
		  Series2->Clear();

	tsl->Text=ReplaceTextW(tsl->Text,".",","); // �������� ��� ����� �� �������

	for(int i=0;i<tsl->Count;i++) // �� ���������� �����
		  {

		  if(tsl->Strings[i].IsEmpty()) // ������ ������ ����������
		  continue;
		  String s = tsl->Strings[i];

		  String s1=wcstok(s.c_str(),L" \t");
		  String s2=wcstok(NULL,L" \t");
		  String s3=wcstok(NULL,L" \t");

		  Series1->AddXY(s1.ToDouble(), // ������ ����� �� ������� ��� �, ������ ����� �
		  s2.ToDouble(),"",clRed);
		  Series2->AddXY(s1.ToDouble(), // ������ ����� �� ������� ��� �, ������ ����� �
		  s3.ToDouble(),"",clRed);

		  }
	}
	delete tsl;
}

void __fastcall TForm1::bLoadingPlotsClick(TObject *Sender)
{

// NEED'S UP TO DATE!!!!

/*if (rbLeftPlot->Checked) // �����
{
	LoadingDataFromFile(Series1,Series2);
}
if (rbRightPlot->Checked) // ������
{
	LoadingDataFromFile(Series3,Series4);
	/*for (int i=0; i < NumberOfPoints; i++)
	{
		ParamsWithNoise.Us[i]=Series3->YValues->Value[i];
		ParamsWithNoise.Uy[i]=Series4->YValues->Value[i];
	}
}      */
if (rbFilteredUPlot->Checked) // �������������
{
	LoadingDataFromFile(gSeriesFilteredUs,gSeriesFilteredUy);
}

if (rbIdealUPlot->Checked) // ���������
{
	LoadingDataFromFile(gSeriesIdealParamsUs,gSeriesIdealParamsUy);    //------------------------------!
	int length=gSeriesIdealParamsUs->YValues->Count;
	long double *x=new long double[length];
	long double *y=new long double[length];

	 for (int i=0; i < length; i++) {
		 x[i]=gSeriesIdealParamsUs->XValues->Value[i];
		 y[i]=gSeriesIdealParamsUs->YValues->Value[i];
	 }
	 gSeriesIdealParamsUs->Clear();
	 for (int i=0; i < length; i++) {
	 if (x[i]>=0) {
		 gSeriesIdealParamsUs->AddXY(x[i],y[i],"",clRed);

     }
	 }
	  for (int i=0; i < length; i++) {
		 x[i]=gSeriesIdealParamsUy->XValues->Value[i];
		 y[i]=gSeriesIdealParamsUy->YValues->Value[i];
	 }
	 gSeriesIdealParamsUy->Clear();
	 for (int i=0; i < length; i++) {
	 if (x[i]>=0) {
		 gSeriesIdealParamsUy->AddXY(x[i],y[i],"",clRed);

     }
	 }
     //ShowMessage(LineSeries3->YValues->Count);
 /*
 for (int i=0; i <NumberOfPoints; i++) { //----------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	ParamsWithNoise.Us[i]=LineSeries3->YValues->Value[i];
	ParamsWithNoise.Uy[i]=LineSeries9->YValues->Value[i];
 }             */


 delete[] x;
 delete[] y;
}

}

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
	SavingAllPoints(gSeriesFilteredUs,gSeriesFilteredUy);
}
//---------------------------------------------------------------------------
// ������� ��������� ��� ����� ���� ��������
// �.�. ������������� � ������ ������� ��� �������� ������������.
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8)
{
	bool isRoundNeeded = false;

	if(!(Series7->YValues->Count || Series8->YValues->Count))
	{
	 ShowMessage("������ ����! ������ ��������: "+ IntToStr(Series7->YValues->Count) +"������ ��������: "+ IntToStr(Series8->YValues->Count));
	 return;
	}
	if(Series7->YValues->Count!=Series8->YValues->Count)
	{
	 ShowMessage("������ ���������� ����� �� ��������!");
	 return;
	}
	long double * x = new long double[Series7->YValues->Count];
	// ���������� ���������� ����������� � ������ � �����-�� ������ �_�
	// ���, �� ����� ����� ������� � �������, ��������� � ����� �������, � ���:)

	if(isRoundNeeded)
	{
		for(int i=0;i<Series7->YValues->Count;i++)
		{
			for(int j=0;j<Series7->YValues->Count;j++)
				x[j]=Series7->YValues->Value[j];

			Form1->RoundM(x,Series7->YValues->Count);

			for(int j=0;j<Series7->YValues->Count;j++)
				Series7->YValues->Value[j]=x[j];
			for(int j=0;j<Series8->YValues->Count;j++)
				x[j]=Series8->YValues->Value[j];

			Form1->RoundM(x,Series8->YValues->Count);

			for(int j=0;j<Series8->YValues->Count;j++)
				Series8->YValues->Value[j]=x[j];
		}
	}
	// ��������� ������.
	TStringList * tsl=new TStringList();
	for(int i=0;i<Series7->YValues->Count;i++)
	{
		tsl->Add(FloatToStr(Series7->XValues->Value[i])+"\t"+Series7->YValues->Value[i]+"\t"+Series8->YValues->Value[i]);
	}
	tsl->Text=ReplaceTextW(tsl->Text,",","."); // �������� ��� ������� �� �����
	// ���� ������� ����� ����� - ��� ��� ������ ���� ��������
	if (silentModeEnabled || Form1->sg1->Execute())
	{
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
	for (int i = 0; i < NumberOfCarrierTypes; i++)
	{
		tsl->Add(g_Nz_par->Cells[1][i+1] +"\t"+g_Nz_par->Cells[2][i+1]);
	}
	tsl->Text=ReplaceTextW(tsl->Text,",","."); // �������� ��� ������� �� �����
	if (silentModeEnabled || Form1->sg1->Execute())
	{
		tsl->SaveToFile(Form1->sg1->FileName);
	}
	delete tsl;
}



void __fastcall TForm1::bTestingSomethingClick(TObject *Sender)
{
	extrapolate5Degree(Series2, 0, 2.5, 0.2,Series4,5);
}
//---------------------------------------------------------------------------





void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
	delete IdealParams;
	delete ParamsWithNoise;
	delete FilteredParams;
	delete ExtrapolatedParams;

	IdealParams=0;
	ParamsWithNoise=0;
	FilteredParams=0;
	ExtrapolatedParams=0;
}
//---------------------------------------------------------------------------



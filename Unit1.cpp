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

void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);
void ParamsKRT(void);


bool silentModeEnabled=false; // если флаг равен тру - то функции сохранения не спрашивают имя файла
// флаг используется для крупных расчетов))

/*

TO DO:

1. Перед фильтрацией прикрутить нормальное достроение графика в отрицательную область магнитного поля.+
2. Попробовать использовать экстраполяцию ПОСЛЕ фильтрации.
3. Подбор длины фильтра относительно величины отклонения точки (0,0) от истинного значения.
4. Коррекция смещения точки, проходящей через начало координат.
5. Апгрейд функций сохранения и загрузки данных.


На данный момент код, отвечающий за расчет параметров переписан с использованием классов.

*/

 // */

const int NumberOfPoints=201;//11;  // количество точек
const int NumberOfNumbersAfterPoint=4; // параметр округления.

const int NumberOfCarrierTypes=3;
long double h; // величина шага


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}

//------------------------------------------------------------------------------


clMagneticFieldDependences *IdealParams=0;
clMagneticFieldDependences *ParamsWithNoise=0;
clMagneticFieldDependences *FilteredParams=0;


//------------------------------------------------------------------------------
//---------------------------------------------------------------------------
void __fastcall TForm1::FormCreate(TObject *Sender)
{
g_Nz_par->Cells[0][1]="электроны";
g_Nz_par->Cells[0][2]="легкие дырки";
g_Nz_par->Cells[0][3]="тяжелые дырки";
g_Nz_par->Cells[1][0]="концентрация";
g_Nz_par->Cells[2][0]="подвижность";
}
//---------------------------------------------------------------------------

void RoundM(long double * x,int length);

void ParamsKRT(void)
{

	h=Form1->g_hsize->Text.ToDouble(); // сохраняем значение шага

	//--------------------------Классы------------------------------------------
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
	//--------------------------Классы------------------------------------------

	Form1->bCalculateCarrierParams->Enabled=true;
	Form1->BuildingPlots->Enabled=1;

	//--------------------------------------------------------------------------
}

// расчет тензоров проводимости
void __fastcall TForm1::bCalculateCarrierParamsClick(TObject *Sender)
{
	if(IdealParams!=0)
	delete IdealParams;
	/*

	Читаем концентрации и подвижности с формы.
	Читаем шаг.
	Заполняем магнитное поле.
	Рассчитываем компоненты тензора по значениям параметров плёнки.
	Выводим их на экран
	Рассчитываем эффективные значения.
	Получаем измеряемые сигналы.


	*/
	ParamsKRT();

	//------------------Классы--------------------------------------------------
	IdealParams->calculateMagneticFieldDependences();

	IdealParams->constructPlotFromTwoMassive(SXX,Series1,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,Series2,clRed);
	IdealParams->constructPlotFromTwoMassive(SXX,Series5,clRed);
	IdealParams->constructPlotFromTwoMassive(SXY,LineSeries5,clRed);

	IdealParams->constructPlotFromTwoMassive(S_EFF,LineSeries1,clRed);
	IdealParams->constructPlotFromTwoMassive(RH_EFF,LineSeries7,clRed);

	IdealParams->constructPlotFromOneMassive(US,LineSeries3,clRed);
	IdealParams->constructPlotFromOneMassive(UY,LineSeries9,clRed);

	//------------------Классы--------------------------------------------------
	bGaussianNoiseGenerator->Enabled=1;
}
//---------------------------------------------------------------------------
// округление результатов при сохранении, необходимо потому что если этого не сделать
// метод будет считать что мы с этой точностью измеряли - и мало что найдет
void RoundM(long double * x,int length)
{
	int S=pow(10,NumberOfNumbersAfterPoint);
	for(int i=0;i<length;i++)
	{
		int n=(int)(x[i]*S)%10;
		if(n<5)
			x[i]=floor(x[i]*S)/S;
		else
			x[i]=ceil(x[i]*S)/S;
	}
}


// генератор шума
void __fastcall TForm1::bGaussianNoiseGeneratorClick(TObject *Sender)
{
	/*
	К идеальным значениям сигнала добавляется шум.
	Выводится на экран.
	*/
	//--------------------Классы----------------------------------------------------

	srand(time(NULL));

	long double vz[6]={0};  // М СКО и СКО в %
	if(ParamsWithNoise!=0)
	delete ParamsWithNoise;
	ParamsWithNoise= new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
	ParamsWithNoise->modifySignals(ShumAdding,IdealParams->getSignalUs(),IdealParams->getSignalUy(),vz,Edit5->Text.ToDouble());

	mDebug->Lines->Add(FloatToStr(vz[0]));
	Edit1->Text=FloatToStr(vz[1]); // СКО
	Edit3->Text=FloatToStr(vz[2]);

	mDebug->Lines->Add(FloatToStr(vz[3]));
	Edit2->Text=FloatToStr(vz[4]); // СКО
	Edit4->Text=FloatToStr(vz[5]);

	ParamsWithNoise->constructPlotFromTwoMassive(US,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,Series4,clRed);

	//--------------------Классы----------------------------------------------------

	tenzorCalculating->Enabled=1;
}
//---------------------------------------------------------------------------
// сохранение результатов
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
	if(Key=='Е') Key='E';  // русскую Е заменяю на английскую
	if (Key=='е') Key='E';
	if ((Key>='0' && Key<='9') || Key==',' || Key=='\b' || Key=='E' ) ;
	else
		Key=0;
}
//---------------------------------------------------------------------------
// генератор белого шума, не знаю нужен ли он нам вообще)
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

 // СКО %  от математического ожидания
 Edit3->Text=FloatToStr(Edit1->Text.ToDouble()/Mo(sxx,l)*100);
 Edit4->Text=FloatToStr(Edit2->Text.ToDouble()/Mo(sxy,l)*100);
 */
}
//---------------------------------------------------------------------------
// вычисление собственной концентрации электронов
long double ownConcentrationOfElectrons(long double T, long double x)
{
	long double k=1.380648813E-23/1.60217646E-19; // постоянная больцмана в электрон-Вольтах
	long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2.0*x)*T;
	return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2./k/T); // собственная концентрация
}






void __fastcall TForm1::BuildingPlotsClick(TObject *Sender)
{
	Form2->Show();
}
//---------------------------------------------------------------------------


// расчет тензоров на основе измеренных сигналов
void __fastcall TForm1::tenzorCalculatingClick(TObject *Sender)
{
//-----------------------------Классы-------------------------------------------
	ParamsWithNoise->constructPlotFromTwoMassive(US,LineSeries3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(UY,LineSeries9,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(S_EFF,LineSeries1,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(RH_EFF,LineSeries7,clRed);

	ParamsWithNoise->constructPlotFromTwoMassive(SXX,Series5,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,LineSeries5,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(SXX,Series3,clRed);
	ParamsWithNoise->constructPlotFromTwoMassive(SXY,Series4,clRed);

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


//-----------------------------Классы_------------------------------------------
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bAutomaticCalculationClick(TObject *Sender)
{
	/*
	+1. Просим задать имя.
	+2. Идем по температуре с шагом.
	+3. Рассчитываем параметры пленки и тензоры проводимости
	+4. Сохраняем параметры плёнки.
	+4. Сохраняем идеальный график тензора все точки и 11 точек.
	+5. Сохраняем идеальный график измеряемых сигналов Us и Uy все точки и 11 точек
	+6. Сохраняем рассчитанные параметры плёнки (концентрации и подвижности НЗ).
	+7. Задаем длину фильтра.

	// для каждого значения шума.
	Генерируем зашумленный сигнал.
	+8. Сохраняем все точки и 11 точек зашумленного измеряемого сигнала.
	+9. Запускаем фильтрацию.
	+10. Сохраняем все точки фильтрованного сигнала.
	+11. Выполняем обратный расчет для зашумленного сигнала (перед этим наверное стоит привести его в вид "как было"?).
	Обратно можно не приводить, т.к. во время фильтрации мы издеваемся над графиком, а рассчитываем по массивам.

	+12. Сохраняем все точки и 11 точек зашумленных компонент тензора.
	+13. Обрабатываем фильтрованный измеренный сигнал, так чтобы он стал пригоден для расчетов.
	+14. Выполняем обратный расчет для фильтрованного зашумленного сигнала.
	+15. Сохраняем все точки и 11 точек фильтрованных компонент тензора.

	*/
	silentModeEnabled=true;
	int h=eStepByTemperature->Text.ToInt(); // шаг по температуре
	int T1=80; // начальная температура
	int Tmax=300; // конечная температура

	int koef=1;// начальный коэффициент шума
	int endkoef=5;  // конечный коэффициент

	int h_koef=1; // шаг по уровню шума
	eFilterLength->Text=IntToStr(80); // задаём длину фильтра, внимание - это для симметричного и удлиненного графика!
	g_hsize->Text=FloatToStr(0.0125); // шаг по магнитному полю.

	UnicodeString standartName; // эталонное имя файла
	UnicodeString fName;   // новое имя файла

	// если в диалоге имя было выбрано
	if (sg1->Execute()) {
		mDebug->Lines->Add( sg1->FileName); // выводим его в мемо
	}
	standartName = sg1->FileName;     // и запоминаем

	for (int T=T1; T <= Tmax; T+=h)  // идем по температуре с заданным шагом
	{
		 eTemperature->Text=IntToStr(T);  // для красоты обновляем значение температуры
		 bCalculateCarrierParams->Click(); // нажимаем кнопку рассчитать
		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveFilmParams->Click(); // сохраняем параметры плёнки


		 rbLeftPlot->Checked=true; // выбираем идеальный график тензоров
		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
		 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 // сохраняем 11 точек идеального графика тензора
		 bSaveElevenPoints->Click();


		 rbIdealPlot->Checked=true; // выбираем идеальный график измеряемых сигналов
		 // задаём имя файла
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика сигналов
		 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy_"+Edit3->Text+".txt";
		 sg1->FileName=standartName+fName;
		 // сохраняем 11 точек идеального графика сигналов
		 bSaveElevenPoints->Click();

		 for (int i=koef; i <= endkoef; i+=h_koef) // после чего начинается игра с коэффициентами
		 {
			Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1]))); // задаем коэффициенты
			Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(IdealParams->getSignalUy()[NumberOfPoints-1])));
			bGaussianNoiseGenerator->Click(); // генератор шума

			rbRightPlot->Checked=true; // выбираем зашумленный график измеряемого сигнала
			fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			 sg1->FileName=standartName+fName;
			 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
			 fName="T_"+IntToStr(T)+"_Us_Uy_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			 sg1->FileName=standartName+fName;
			 // сохраняем 11 точек идеального графика тензора
			 bSaveElevenPoints->Click();

			 // запускаем фильтрацию
			 bFilteringPlots->Click();

			 // сохраняем результаты фильтрации (все точки)-------------------------
			 rbFilteredPlot->Checked=true; // выбираем фильтрованный график
			 fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
			 sg1->FileName=standartName+fName;
			 bSaveAllPoints->Click();
			 //---------------------------------------------------------------------

			  tenzorCalculating->Click(); // обратный расчет
			  //--------------------------------------------------------------------
			  // надо сохранять результаты генератора
			  rbRightPlot->Checked=1;
			  fName="T_"+IntToStr(T)+"_tenzor_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			  sg1->FileName=standartName+fName;
			  bSaveAllPoints->Click();// теперь сохранить
			  fName="T_"+IntToStr(T)+"_tenzor_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			  sg1->FileName=standartName+fName;
			  // сохраняем 11 точек графика
			  bSaveElevenPoints->Click();
			  //--------------------------------------------------------------------
			  //-------обработка результатов фильтрации-----------------------------

			  //1T_80_Us_Uy_vseZnachenia_filtr_k_50.txt
			  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
			  sg1->FileName=standartName+fName;
			  rbIdealPlot->Checked=1;
			  bLoadingPlots->Click();

			  //-------обратный расчет для фильтрованных сигналов
			  tenzorCalculating->Click();
			  //--------------------------------------------------------------------
			  //--------------------------------------------------------------------
			  rbRightPlot->Checked=1;// сохранять результаты фильтрованных тензоров
			  fName="T_"+IntToStr(T)+"_tenzor_filt_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			  sg1->FileName=standartName+fName;
			  bSaveAllPoints->Click();
			  // в двух форматах: все точки и 11 точек.
			  fName="T_"+IntToStr(T)+"_tenzor_filt_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
			  sg1->FileName=standartName+fName;
			  bSaveElevenPoints->Click();
			  //--------------------------------------------------------------------
		 }
	}
	silentModeEnabled=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bFilteringPlotsClick(TObject *Sender)
{

 //------------------Классы-----------------------------------------------------
 if(FilteredParams!=0)
 delete FilteredParams;

 FilteredParams=new clMagneticFieldDependences(NumberOfPoints,h,IdealParams->carrierParams);
 FilteredParams->modifySignals(TrForMassiveFilter,ParamsWithNoise->getSignalUs(),ParamsWithNoise->getSignalUy(),eFilterLength->Text.ToInt());

 FilteredParams->constructPlotFromTwoMassive(US,Series7,clBlue);
 FilteredParams->constructPlotFromTwoMassive(UY,Series8,clBlue);

 //------------------Классы-----------------------------------------------------
}



void __fastcall TForm1::bSaveElevenPointsClick(TObject *Sender)
{

// работает с графиками
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

if (silentModeEnabled || sg1->Execute()) {
tsl->SaveToFile(sg1->FileName);
}
delete tsl;
}
//---------------------------------------------------------------------------

void LoadingDataFromFile(TLineSeries * Series1,TLineSeries * Series2)
{
// работает с графиками, от структур свободна.
TStringList * tls=new TStringList();
if (silentModeEnabled || Form1->sg1->Execute()) {
tls->LoadFromFile(Form1->sg1->FileName);
Series1->Clear();
	  Series2->Clear();

for(int i=0;i<tls->Count;i++) // по количеству строк
	  {

	  if(tls->Strings[i].IsEmpty()) // пустые строки пропускаем
	  continue;
	  String s = tls->Strings[i];

	  for (int k = 1; k <= s.Length(); k++) {
		  if(s[k]==L'.')
		  s[k]=L',';
	  }

	  String s1=wcstok(s.c_str(),L" \t");
	  String s2=wcstok(NULL,L" \t");
	  String s3=wcstok(NULL,L" \t");



	  Series1->AddXY(s1.ToDouble(), // первая часть до пробела это х, вторая после у
	  s2.ToDouble(),"",clRed);
	  Series2->AddXY(s1.ToDouble(), // первая часть до пробела это х, вторая после у
	  s3.ToDouble(),"",clRed);

	  }

}
delete tls;
}

void __fastcall TForm1::bLoadingPlotsClick(TObject *Sender)
{

// NEED'S UP TO DATE!!!!

if (rbLeftPlot->Checked) // левый
{
	LoadingDataFromFile(Series1,Series2);
}
if (rbRightPlot->Checked) // правый
{
	LoadingDataFromFile(Series3,Series4);
	/*for (int i=0; i < NumberOfPoints; i++)
	{
		ParamsWithNoise.Us[i]=Series3->YValues->Value[i];
		ParamsWithNoise.Uy[i]=Series4->YValues->Value[i];
	}   */
}
if (rbFilteredPlot->Checked) // фильтрованный
{
	LoadingDataFromFile(Series7,Series8);
}

if (rbIdealPlot->Checked) // идеальный
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
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

void __fastcall TForm1::Button13Click(TObject *Sender)
{
SavingAllPoints(Series7,Series8);
}
//---------------------------------------------------------------------------
// функция сохраняет все точки двух графиков
// т.е. предназначена в первую очередь для тензоров проводимости.
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8)
{


bool isRoundNeeded = false;


if(!(Series7->YValues->Count || Series8->YValues->Count))
{
 ShowMessage("График пуст!");
 return;
}
if(Series7->YValues->Count!=Series8->YValues->Count)
{
 ShowMessage("Разное количество точек на графиках!");
 return;
}
long double * x = new long double[Series7->YValues->Count];
// производим округление результатов
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
if(s[j]==',')     // заменить все запятые на точки
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);

}
// если включен тихий режим - имя уже должно быть известно
if (silentModeEnabled || Form1->sg1->Execute()) {
tsl->SaveToFile(Form1->sg1->FileName);
}
delete[] x;
delete tsl;
}

// сохранение параметров КРТ
// поддерживает тихий режим
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
if (silentModeEnabled || Form1->sg1->Execute()) {
tsl->SaveToFile(Form1->sg1->FileName);
}
delete tsl;
}



void __fastcall TForm1::bTestingSomethingClick(TObject *Sender)
{



extrapolate5Degree(Series2, 0, 2.5, 0.2,Series4);



}
//---------------------------------------------------------------------------





void __fastcall TForm1::FormClose(TObject *Sender, TCloseAction &Action)
{
delete IdealParams;
delete ParamsWithNoise;
delete FilteredParams;

IdealParams=0;
ParamsWithNoise=0;
FilteredParams=0;
}
//---------------------------------------------------------------------------


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

1. Адекватное сохранение чистого измеряемого сигнала
2. Автоматическую фильтрацию измеряемого сигнала, а не компонент тензора
3. Автоматическое сохранение измеряемого сигнала, с шумом и без
4. Доработка конвейра согласно п 1-3.
5. Видимо сгенерить опять кучу данных и посчитать.
6. Возможно сделать расфасовку данных для разных температур по разным папкам.
7. Было бы неплохо сделать построение спектров графиков прямо в этой проге, хотя это пожалуй избыточно.

*/



/*

Итак, попробуем изобрести классы

class Film

class Carrier

class

*/

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
long double Us; // продольный сигнал
long double Uy; // поперечный сигнал
long double s_eff;
long double Rh_eff;


};



__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
const int l=201;//11;  // количество точек
const int NumberOfNumbersAfterPoint=4;
long double q=1.60217646E-19; // заряд электрона
long double h; // величина шага
long double B[l]={0};
long double sxx[l]={0};  // тензор проводимости
long double sxy[l]={0};
long double s_eff[l]={0};  // подвижность
long double Rh_eff[l]={0};  // эффект Холла
long double Us[l]={0};     // измеряемый сигнал
long double Uy[l]={0};     // измеряемый сигнал

long double s_eff_n[l]={0};  // подвижность
long double Rh_eff_n[l]={0};  // эффект Холла
long double Us_n[l]={0};  // измеряемый сигнал
long double Uy_n[l]={0};  // измеряемый сигнал

long double sxx_n[l]={0};  // тензор проводимости
long double sxy_n[l]={0};

bool silentMode=false; // если флаг равен тру - то функции сохранения не спрашивают имя файла
// флаг используется для крупных расчетов))
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

void ParamsKRT(void);

void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);



// расчет тензоров проводимости
void __fastcall TForm1::bCalculateCarrierParamsClick(TObject *Sender)
{
ParamsKRT();
Series1->Clear();     // чистим все графики
Series2->Clear();
Series5->Clear();
LineSeries5->Clear();

LineSeries1->Clear();
LineSeries7->Clear();

LineSeries3->Clear();
LineSeries9->Clear();
long double u[3]={0}; // подвижности НЗ
long double n[3]={0}; // концентрации НЗ
for(int i=0;i<3;i++) // читаем эти значения с формы
{
	u[i]=g_Nz_par->Cells[2][i+1].ToDouble();
	n[i]=g_Nz_par->Cells[1][i+1].ToDouble();
}
h=g_hsize->Text.ToDouble(); // сохраняем значение шага

B[0]=0;
for(int i=1;i<l;i++)
{
	B[i]=B[i-1]+h;  // это наше магнитное поле от 0 до 2 Тл
}
// рассчитываем компоненты тензона проводимости


for(int i=0;i<l;i++)
{
sxx[i]=sxy[i]=0;
	for(int j=0;j<3;j++)
	{
		sxx[i]+=q*n[j]*u[j]/(1.0+u[j]*u[j]*B[i]*B[i]);
		sxy[i]+=q*n[j]*u[j]*u[j]*B[i]/(1.0+u[j]*u[j]*B[i]*B[i]);
	}
	Series1->AddXY(B[i],sxx[i],"",clRed);  // рисуем их на графиках
	Series2->AddXY(B[i],sxy[i],"",clRed);
	Series5->AddXY(B[i],sxx[i],"",clRed);
	LineSeries5->AddXY(B[i],sxy[i],"",clRed);


}
bGaussianNoiseGenerator->Enabled=1;
//-------------------------------------------------
//- рассчитываем эффективные значения
for(int i=0;i<l;i++)
{
	s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
	Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]); // 1/B
	LineSeries1->AddXY(B[i],s_eff[i],"",clRed);
	LineSeries7->AddXY(B[i],Rh_eff[i],"",clRed);
}
//-------------------------------------------------
long double d=eSampleThickness->Text.ToDouble(); // 1e-5; // толщина образца
long double cb=eCBRatio->Text.ToDouble(); //3; // отношение с к b
long double I=eCurrentIntensity->Text.ToDouble(); //1e-3; // ток от 1e-3 до 1e-4


//-------------------------------------------------------------------------------------
for(int i=0;i<l;i++)
{
   //Us(B)=cb/d*I/s(B)
   //Uy(B)=Rh(B)*I*B/d

   // вот к этим значениям будет добавляться шум.
Us[i]=cb/d*I/s_eff[i];
Uy[i]=Rh_eff[i]*I/d;   // Rh_eff[i]*I*B[i]/d;

	LineSeries3->AddY(Us[i],"",clRed);
	LineSeries9->AddY(Uy[i],"",clRed);
}

}
//---------------------------------------------------------------------------
// округление результатов при сохранении, необходимо потому что если этого не сделать
// метод будет считать что мы с этой точностью измеряли - и мало что найдет
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


// генератор шума
void __fastcall TForm1::bGaussianNoiseGeneratorClick(TObject *Sender)
{
srand(time(NULL));
long double y[l]={0}; // шум для sxx
long double y1[l]={0};  // шум для sxy
long double vz[3]={0};  // М СКО и СКО в %
ShumAdding(Us,y,vz,Edit5->Text.ToDouble(),l);
Form1->mDebug->Lines->Add(FloatToStr(vz[0]));
Form1->Edit1->Text=FloatToStr(vz[1]); // СКО
Form1->Edit3->Text=FloatToStr(vz[2]);

ShumAdding(Uy,y1,vz,Edit5->Text.ToDouble(),l);
Form1->mDebug->Lines->Add(FloatToStr(vz[0]));
Form1->Edit2->Text=FloatToStr(vz[1]); // СКО
Form1->Edit4->Text=FloatToStr(vz[2]);

Series3->Clear();
Series4->Clear();
for(int i=0;i<l;i++)
{
 Series3->AddXY(B[i],y[i],"",clRed);  // выводим графики с шумом
 Us_n[i]=y[i];                        // и сохраняем их для обратного расчета
 Series4->AddXY(B[i],y1[i],"",clRed);
 Uy_n[i]=y1[i];
}
Button8->Enabled=1;
}
//---------------------------------------------------------------------------
// сохранение зашумленных результатов
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
long double niSob(long double T, long double x)

{
long double k=1.380648813E-23/1.60217646E-19; // постоянная больцмана в электрон-Вольтах
long double Eg=-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2*x)*T;
return  (5.585-3.82*x+1.753E-3*T-1.364E-3*x*T)*1E20*pow(Eg,3/4.)*pow(T,1.5)*exp(-Eg/2/k/T); // собственная концентрация
}

void ParamsKRT(void)
{
long double x=Form1->eMolarCompositionCadmium->Text.ToDouble();//0.22; // мольный состав кадмия
long double T=Form1->eTemperature->Text.ToDouble();//77; // температура
long double m0=9.10938188E-31; // масса электрона хотя она и не нужна в общем-то:)
long double mh=0.443*m0; // масса тяжелых дырок
long double ml=0.001*m0; // масса легких дырок

long double ph=Form1->eHeavyHoleConcentration->Text.ToDouble();//1.0E22; // концентрация тяжелых дырок
long double pl=pow(ml/mh,1.5)*ph*50; // концентрация легких дырок домножена на 50
long double n=-pow(niSob(T,x),2)/ph;
long double A=Form1->eAFactor->Text.ToDouble();//5; // 5-8
long double k=Form1->eKFactor->Text.ToDouble();//1.3; //1.3-1.5
long double ue=A*pow(T/77.0,-k); // подвижность электронов в области 77-300К

long double uh=0.005*ue;   // подвижности тяжелых и
long double ul=0.1*ue;     // легких дырок в первом приближении
ue*=-1;

Form1->g_Nz_par->Cells[1][1]=FloatToStr(n);
Form1->g_Nz_par->Cells[2][1]=FloatToStr(ue);
Form1->g_Nz_par->Cells[1][2]=FloatToStr(pl);
Form1->g_Nz_par->Cells[2][2]=FloatToStr(ul);
Form1->g_Nz_par->Cells[1][3]=FloatToStr(ph);
Form1->g_Nz_par->Cells[2][3]=FloatToStr(uh);
Form1->bCalculateCarrierParams->Enabled=true;
Form1->Button7->Enabled=1;
}



void __fastcall TForm1::Button7Click(TObject *Sender)
{
Form2->Show();
}
//---------------------------------------------------------------------------


// обратный расчет тензоров
void __fastcall TForm1::Button8Click(TObject *Sender)
{
long double d=eSampleThickness->Text.ToDouble(); // 1e-5; // толщина образца
long double cb=eCBRatio->Text.ToDouble(); //3; // отношение с к b
long double I=eCurrentIntensity->Text.ToDouble(); //1e-3; // ток от 1e-3 до 1e-4
LineSeries3->Clear();
LineSeries9->Clear();
LineSeries1->Clear();
LineSeries7->Clear();
for(int i=0;i<l;i++)
{
  // теперь считаем в обратном направлении
s_eff_n[i]=cb/d*I/Us_n[i];
Rh_eff_n[i]=d*Uy_n[i]/I;
//if(B[i])

//else
//Rh_eff_n[i]=0;//d*Uy_n[i]/I;

	LineSeries3->AddXY(B[i],Us_n[i],"",clRed);
	LineSeries9->AddXY(B[i],Uy_n[i],"",clRed);
	LineSeries1->AddXY(B[i],s_eff_n[i],"",clRed);
	LineSeries7->AddXY(B[i],Rh_eff_n[i],"",clRed);

}
Series3->Clear();
Series4->Clear();
Series5->Clear();
LineSeries5->Clear();
for(int i=0;i<l;i++)
{
	sxx_n[i]=s_eff_n[i]/(Rh_eff_n[i]*Rh_eff_n[i]*s_eff_n[i]*s_eff_n[i]+1);
	sxy_n[i]=s_eff_n[i]*s_eff_n[i]*Rh_eff_n[i]/(Rh_eff_n[i]*Rh_eff_n[i]*s_eff_n[i]*s_eff_n[i]+1);//Rh_eff_n[i]*s_eff_n[i]*sxx_n[i];
	Series5->AddXY(B[i],sxx_n[i],"",clRed);
	LineSeries5->AddXY(B[i],sxy_n[i],"",clRed);
	Series3->AddXY(B[i],sxx_n[i],"",clRed);
	Series4->AddXY(B[i],sxy_n[i],"",clRed);
//    Memo1->Lines->Add(FloatToStr(sxy_n[i]));
}

// рассчитаем СКО для полученных графиков

long double sko1=Sko(sxx,sxx_n,l);
long double sko2=Sko(sxy,sxy_n,l);
long double shx[l]={0};
long double shy[l]={0};
for(int i=0;i<l;i++)
{
	shx[i]=sxx_n[i]-sxx[i];
	shy[i]=sxy_n[i]-sxy[i];
}
long double mx=Mo(shx,l);
long double my=Mo(shy,l);
Edit1->Text=FloatToStr(sko1);
Edit2->Text=FloatToStr(sko2);
Edit3->Text=FloatToStr(mx/Mo(sxx,l)*100);
Edit4->Text=FloatToStr(my/Mo(sxy,l)*100);
;
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
13. Обрабатываем фильтрованный измеренный сигнал, так чтобы он стал пригоден для расчетов.
+14. Выполняем обратный расчет для фильтрованного зашумленного сигнала.
+15. Сохраняем все точки и 11 точек фильтрованных компонент тензора.

*/
silentMode=true;
int h=eStepByTemperature->Text.ToInt(); // шаг по температуре
int T1=300; // начальная температура
int Tmax=300; // конечная температура

int koef=1;// начальный коэффициент шума
int endkoef=5;  // конечный коэффициент

int h_koef=1; // шаг по уровню шума
eFilterLength->Text=IntToStr(80); // задаём длину фильтра, внимание - это для симметричного и удлиненного графика!
g_hsize->Text=FloatToStr(0.0125);
UnicodeString oldName; // старое имя файла
UnicodeString fName;   // новое имя файла

// если в диалоге имя было выбрано
if (sg1->Execute()) {
	mDebug->Lines->Add( sg1->FileName); // выводим его в мемо
}
oldName = sg1->FileName;     // и запоминаем

for (int T=T1; T <= Tmax; T+=h)  // идем по температуре с заданным шагом
{
     eTemperature->Text=IntToStr(T);  // для красоты обновляем значение температуры
	 bCalculateCarrierParams->Click(); // нажимаем кнопку рассчитать
     // задаём имя файла
	 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 bSaveFilmParams->Click(); // сохраняем параметры плёнки


	 rbLeftPlot->Checked=true; // выбираем идеальный график тензоров
	 // задаём имя файла
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // сохраняем 11 точек идеального графика тензора
	 bSaveElevenPoints->Click();


	 rbIdealPlot->Checked=true; // выбираем идеальный график измеряемых сигналов
     // задаём имя файла
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика сигналов
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // сохраняем 11 точек идеального графика сигналов
	 bSaveElevenPoints->Click();

     for (int i=koef; i <= endkoef; i+=h_koef) // после чего начинается игра с коэффициентами
	 {
		  Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1]))); // задаем коэффициенты
		  Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1])));
		  bGaussianNoiseGenerator->Click(); // генератор шума

		  rbRightPlot->Checked=true; // выбираем зашумленный график измеряемого сигнала
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 bSaveAllPoints->Click();     	 // сохраняем все точки идеального графика тензора
		 fName="T_"+IntToStr(T)+"_Us_Uy_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 // сохраняем 11 точек идеального графика тензора
		 bSaveElevenPoints->Click();

		 // запускаем фильтрацию
		 bFilteringPlots->Click();

		 // сохраняем результаты фильтрации (все точки)-------------------------
		 rbFilteredPlot->Checked=true; // выбираем фильтрованный график
		 fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		 sg1->FileName=oldName+fName;
		 bSaveAllPoints->Click();
         //---------------------------------------------------------------------

		  Button8->Click(); // обратный расчет
		  //--------------------------------------------------------------------
		  // надо сохранять результаты генератора
		  rbRightPlot->Checked=1;
		  fName="T_"+IntToStr(T)+"_tenzor_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  bSaveAllPoints->Click();// теперь сохранить
		  fName="T_"+IntToStr(T)+"_tenzor_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  // сохраняем 11 точек графика
		  bSaveElevenPoints->Click();
		  //--------------------------------------------------------------------
		  //-------обработка результатов фильтрации-----------------------------

		  //1T_80_Us_Uy_vseZnachenia_filtr_k_50.txt
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		  sg1->FileName=oldName+fName;
		  rbIdealPlot->Checked=1;
		  bLoadingPlots->Click();

		  //-------обратный расчет для фильтрованных сигналов
		  Button8->Click();
		  //--------------------------------------------------------------------
          //--------------------------------------------------------------------
		  rbRightPlot->Checked=1;// сохранять результаты фильтрованных тензоров
		  fName="T_"+IntToStr(T)+"_tenzor_filt_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  bSaveAllPoints->Click();
		  // в двух форматах: все точки и 11 точек.
		  fName="T_"+IntToStr(T)+"_tenzor_filt_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  bSaveElevenPoints->Click();
		  //--------------------------------------------------------------------
	 }
}
silentMode=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::bFilteringPlotsClick(TObject *Sender)
{
// Надо прикруть экстраполяцию графиков измеряемых зависимостей!

//int length=Edit8->Text.ToInt();

/*if (l!=Series3->YValues->Count) {
	ShowMessage("Длина массива не та!");
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

 delete [] x;
 delete [] y;    */

  extrapolate2Degree(Series3, 0, 2.75, 0.01,Series3);
  extrapolate2Degree(Series4, 0, 2.75, 0.01,Series4);



Tr_Filter(Series3,Series7,eFilterLength->Text.ToInt(),5000,15,25);
Tr_Filter(Series4,Series8,eFilterLength->Text.ToInt(),5000,15,25);

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
	
	if(fabs(fabs(Saving1->XValues->Value[k])-points[i])<=r)
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

if (rbLeftPlot->Checked) // левый
{
	LoadingDataFromFile(Series1,Series2);
}
if (rbRightPlot->Checked) // правый
{
	LoadingDataFromFile(Series3,Series4);
	for (int i=0; i < l; i++)
	{
		Us_n[i]=Series3->YValues->Value[i];
		Uy_n[i]=Series4->YValues->Value[i];
	}
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

 for (int i=0; i <l /*LineSeries3->YValues->Count*/; i++) { //----------------!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	Us_n[i]=LineSeries3->YValues->Value[i];
	Uy_n[i]=LineSeries9->YValues->Value[i];
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
if (silentMode || Form1->sg1->Execute()) {
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
for (int i = 0; i < 3; i++) {
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




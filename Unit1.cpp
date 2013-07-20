//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Unit1.h"
#include "Unit2.h"
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
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh);
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);

long double determinant (long double ** Arr,int size);

// расчет тензоров проводимости
void __fastcall TForm1::Button1Click(TObject *Sender)
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
Button3->Enabled=1;
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
long double d=LabeledEdit1->Text.ToDouble(); // 1e-5; // толщина образца
long double cb=LabeledEdit2->Text.ToDouble(); //3; // отношение с к b
long double I=LabeledEdit3->Text.ToDouble(); //1e-3; // ток от 1e-3 до 1e-4


//-------------------------------------------------------------------------------------
for(int i=0;i<l;i++)
{
   //Us(B)=cb/d*I/s(B)
   //Uy(B)=Rh(B)*I*B/d

   // вот к этим значениям будет добавляться шум.
Us[i]=cb/d*I/s_eff[i];
Uy[i]=Rh_eff[i]*I/d;   // Rh_eff[i]*I*B[i]/d;

	LineSeries3->AddXY(B[i],Us[i],"",clRed);
	LineSeries9->AddXY(B[i],Uy[i],"",clRed);
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

//---------------------------------------------------------------------------
// сохранение результатов без шума
// преобразование Бокса-Мюллера
void BoxMull(long double *z,int l)
{
long double x,y;

int i;
for(i=0;i<l-1;i+=2)
{
double s=3;
while (s>1 || s<=0)
{
//int a=rand();
x=(-1000+rand()%2000)/1000.0;
y=(-1000+rand()%2000)/1000.0;
// шум в пределах от -1 до 1
s=x*x+y*y;
}
z[i]=x*sqrt(-2*log(s)/s);
z[i+1]=y*sqrt(-2*log(s)/s);
i=i;
}
}


//--------------------------------------------------------------------------
// расчет среднего квадратичного отклонения
//--------------------------------------------------------------------------

long double Sko (long double *x0,long double *x,int l)
{
long double z=0;
for(int i=0;i<l;i++)
z+= pow((x[i]-x0[i]),2);
z/=(long double)l;
return sqrt(z);
}

//---------------------------------------------------
// расчет математического ожидания
//-------------------------------------------------

 long double Mo (long double *x,int l)
 {
 long double M=0;
 for(int i=0;i<l;i++)
 M+=x[i];
 return M/l;
 }

//---------------------------------------------------------------------------
// возвращает величину математического ожидания шума.
void ShumAdding(long double *x,long double *out,long double *ret, long double koeff)
{
long double y[l]={0}; // шум для нашего графика
BoxMull(y,l);
for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;
	// амплитуда шума определяется как 1/koef
  ret[0]=Mo(y,l); // математическое ожидание
  ret[1]=Sko(x,out,l);  // СКО
  ret[2]=ret[0]/Mo(x,l)*100; // СКО в %  от математического ожидания исходного графика
}

// генератор шума
void __fastcall TForm1::Button3Click(TObject *Sender)
{
srand(time(NULL));
long double y[l]={0}; // шум для sxx
long double y1[l]={0};  // шум для sxy
long double vz[3]={0};  // М СКО и СКО в %
ShumAdding(Us,y,vz,Edit5->Text.ToDouble());
Form1->Memo1->Lines->Add(FloatToStr(vz[0]));
Form1->Edit1->Text=FloatToStr(vz[1]); // СКО
Form1->Edit3->Text=FloatToStr(vz[2]);

ShumAdding(Uy,y1,vz,Edit5->Text.ToDouble());
Form1->Memo1->Lines->Add(FloatToStr(vz[0]));
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
void __fastcall TForm1::Button4Click(TObject *Sender)
{
if (RadioButton1->Checked)
{
	 SavingAllPoints(Series1,Series2);
}
if (RadioButton2->Checked)
{
	 SavingAllPoints(Series3,Series4);
}
if (RadioButton3->Checked)
{
     SavingAllPoints(Series7,Series8);
}
if (RadioButton4->Checked)
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
void __fastcall TForm1::Button5Click(TObject *Sender)
{
long double x[l];
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
long double x=Form1->LabeledEdit4->Text.ToDouble();//0.22; // мольный состав кадмия
long double T=Form1->LabeledEdit5->Text.ToDouble();//77; // температура
long double m0=9.10938188E-31; // масса электрона хотя она и не нужна в общем-то:)
long double mh=0.443*m0; // масса тяжелых дырок
long double ml=0.001*m0; // масса легких дырок

long double ph=Form1->LabeledEdit8->Text.ToDouble();//1.0E22; // концентрация тяжелых дырок
long double pl=pow(ml/mh,1.5)*ph*50; // концентрация легких дырок домножена на 50
long double n=-pow(niSob(T,x),2)/ph;
long double A=Form1->LabeledEdit6->Text.ToDouble();//5; // 5-8
long double k=Form1->LabeledEdit7->Text.ToDouble();//1.3; //1.3-1.5
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
Form1->Button1->Enabled=true;
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
long double d=LabeledEdit1->Text.ToDouble(); // 1e-5; // толщина образца
long double cb=LabeledEdit2->Text.ToDouble(); //3; // отношение с к b
long double I=LabeledEdit3->Text.ToDouble(); //1e-3; // ток от 1e-3 до 1e-4
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

void __fastcall TForm1::Button9Click(TObject *Sender)
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
int h=Edit7->Text.ToInt(); // шаг по температуре
int T1=80; // начальная температура
int Tmax=300; // конечная температура

int koef=1;// начальный коэффициент шума
int endkoef=5;  // конечный коэффициент

int h_koef=1; // шаг по уровню шума
Edit8->Text=IntToStr(80); // задаём длину фильтра, внимание - это для симметричного и удлиненного графика!
g_hsize->Text=FloatToStr(0.0125);
UnicodeString oldName; // старое имя файла
UnicodeString fName;   // новое имя файла

// если в диалоге имя было выбрано
if (sg1->Execute()) {
	Memo1->Lines->Add( sg1->FileName); // выводим его в мемо
}
oldName = sg1->FileName;     // и запоминаем

for (int T=T1; T <= Tmax; T+=h)  // идем по температуре с заданным шагом
{
     LabeledEdit5->Text=IntToStr(T);  // для красоты обновляем значение температуры
	 Button1->Click(); // нажимаем кнопку рассчитать
     // задаём имя файла
	 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button2->Click(); // сохраняем параметры плёнки


	 RadioButton1->Checked=true; // выбираем идеальный график тензоров
	 // задаём имя файла
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button4->Click();     	 // сохраняем все точки идеального графика тензора
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // сохраняем 11 точек идеального графика тензора
	 Button11->Click();


	 RadioButton4->Checked=true; // выбираем идеальный график измеряемых сигналов
     // задаём имя файла
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button4->Click();     	 // сохраняем все точки идеального графика сигналов
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // сохраняем 11 точек идеального графика сигналов
	 Button11->Click();




	 for (int i=koef; i <= endkoef; i+=h_koef) // после чего начинается игра с коэффициентами
	 {
		  Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1]))); // задаем коэффициенты
		  Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1])));
		  Button3->Click(); // генератор шума

		  RadioButton2->Checked=true; // выбираем зашумленный график измеряемого сигнала
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 Button4->Click();     	 // сохраняем все точки идеального графика тензора
		 fName="T_"+IntToStr(T)+"_Us_Uy_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 // сохраняем 11 точек идеального графика тензора
		 Button11->Click();

		 // запускаем фильтрацию
		 Button10->Click();

		 // сохраняем результаты фильтрации (все точки)-------------------------
		 RadioButton3->Checked=true; // выбираем фильтрованный график
		 fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		 sg1->FileName=oldName+fName;
		 Button4->Click();
         //---------------------------------------------------------------------

		  Button8->Click(); // обратный расчет
		  //--------------------------------------------------------------------
		  // надо сохранять результаты генератора
		  RadioButton2->Checked=1;
		  fName="T_"+IntToStr(T)+"_tenzor_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  Button4->Click();// теперь сохранить
		  fName="T_"+IntToStr(T)+"_tenzor_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  // сохраняем 11 точек графика
	      Button11->Click();
		  //--------------------------------------------------------------------
		  //-------обработка результатов фильтрации-----------------------------

		  //1T_80_Us_Uy_vseZnachenia_filtr_k_50.txt
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		  sg1->FileName=oldName+fName;
		  RadioButton4->Checked=1;
		  Button12->Click();

		  //-------обратный расчет для фильтрованных сигналов
		  Button8->Click();
		  //--------------------------------------------------------------------
          //--------------------------------------------------------------------
		  RadioButton2->Checked=1;// сохранять результаты фильтрованных тензоров
		  fName="T_"+IntToStr(T)+"_tenzor_filt_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  Button4->Click();
		  // в двух форматах: все точки и 11 точек.
		  fName="T_"+IntToStr(T)+"_tenzor_filt_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  Button11->Click();
		  //--------------------------------------------------------------------
	 }
}
silentMode=false;
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
// Надо прикруть экстраполяцию графиков измеряемых зависимостей!

int length=Edit8->Text.ToInt();

/*if (l!=Series3->YValues->Count) {
	ShowMessage("Длина массива не та!");
	return;
}     */
 double *x=new double[2*l];
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
 delete [] y;

/*double *x=new double[l+length];   // выделяем память под l+length значений
double *y=new double[l+length];

for (int i = 0; i < length; i++) {
	y[i]=Series3->YValues->Value[0];
	x[i]=-(length-i)*h;
}
for (int i = length; i < l+length; i++) {
	y[i]=Series3->YValues->Value[i-length];
	x[i]=Series3->XValues->Value[i-length];
}
Series3->Clear();
for (int i = 0; i < l+length; i++) {
	Series3->AddXY(x[i],y[i],"",clRed);
}

for (int i = 0; i < length; i++) {
	y[i]=Series4->YValues->Value[0];
	x[i]=-(length-i)*h;
}
for (int i = length; i < l+length; i++) {
	y[i]=Series4->YValues->Value[i-length];
	x[i]=Series4->XValues->Value[i-length];
}
Series4->Clear();
for (int i = 0; i < l+length; i++) {
	Series4->AddXY(x[i],y[i],"",clRed);
}  */


			// ну как-то не очень такой метод, надо экстраполяцию всё-таки.
Tr_Filter(Series3,Series7,Edit8->Text.ToInt(),5000,15,25);
Tr_Filter(Series4,Series8,Edit8->Text.ToInt(),5000,15,25);


/*
for(int i=0;i< l+length; i++) {
	   y[i]=Series3->YValues->Value[i];
		x[i]=Series3->XValues->Value[i];
	}
	Series3->Clear();
	for(int i=length;i< l+length; i++) {
	  Series3->AddXY(x[i],y[i],"",clRed);
	}

	for(int i=0;i< l+length; i++) {
	   y[i]=Series4->YValues->Value[i];
		x[i]=Series4->XValues->Value[i];
	}
	Series4->Clear();
	for(int i=length;i< l+length; i++) {
	  Series4->AddXY(x[i],y[i],"",clRed);
	}

delete [] x;
delete [] y;  */
}
//---------------------------------------------------------------------------
// ВНИМАНИЕ!!! Напрямую не вызывать!!! Пользоваться трамплином!!!---------------
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //Длина фильтра 20
long double Fd = Fdisk; //Частота дискретизации входных данных 2000
long double Fs = Fpropysk; //Частота конца полосы пропускания  10
long double Fx = Fzatyh; //Частота начала полосы затухания    20

long double *H= new long double [N] ; //Импульсная характеристика фильтра
long double *H_id= new long double [N]; //Идеальная импульсная характеристика
long double *W= new long double [N]; //Весовая функция

/*long double H [N] = {0}; //Импульсная характеристика фильтра
long double H_id [N] = {0}; //Идеальная импульсная характеристика
long double W [N] = {0}; //Весовая функция  */

//Расчет импульсной характеристики фильтра
long double Fc = (Fs + Fx) / (2 * Fd);

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2*M_PI*Fc;
else H_id[i] = sinl(2*M_PI*Fc*i )/(M_PI*i);
// весовая функция Блекмена
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
H [i] = H_id[i] * W[i];
}

//Нормировка импульсной характеристики
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //сумма коэффициентов равна 1

//----------------------------------------------------------------
// печать коэффициентов фильтра
//for(int i=0;i<N;i++)
//Form1->Memo2->Lines->Add(FloatToStr(H[i]));
//----------------------------------------------------------------

//Фильтрация входных данных
for (int i=0; i<sizeIn; i++)
{
out[i]=0.;
for (int j=0; j<(i>N-1?N-1:i); j++)// та самая формула фильтра
out[i]+= H[j]*in[i-j];
}
delete [] H;
delete [] H_id;
delete [] W;
return (N-1)/2.0;
}
// функция трамплин для фильтра.  ----------------------------------------------
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count)   // если пустой - ошибка, убегаем.
{
ShowMessage("Пустой массив!");
return 0;
}
int size=inS->YValues->Count;  // получаем размер
//size=inS->XValues->Count;  // получаем размер
double *in=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in[i]=inS->YValues->Value[i];
double *out=new double[size]; // выделяем память для выходных значений
double k=Filter(in,out,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// вычисляем сдвиг фаз
//----------------------------------------------
//---------добавление для фильтрации магнитного поля
// внимание - она пока работает не корректно ибо фильтр наполняется только за
// его длину.
/*double *in2=new double[size];  // выделяем память
for(int i=0;i<size;i++)       // копируем
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // выделяем память для выходных значений
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // вызываем фильтр
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// вычисляем сдвиг фаз
*/

//----------------------------------------------
 outS->Clear(); // чистим график, перед выводом
for(int i=0;i<size;i++) // выводим
{
outS->AddXY(inS->XValues->Value[i]-k,out[i],"",clBlue);
//Form1->Series2->AddY(out2[i]-k+k2,"",clBlue);
//Form1->Series3->AddY(inS->XValues->Value[i]-k,"",clRed);
//Form1->Series4->AddY(out[i],"",clBlack);
//Form1->Series5->AddY(out2[i],"",clYellow);

 }
delete[] in;  // прибираемся
//delete[] in2;  // прибираемся
delete[] out;
//delete[] out2;
return k;
}
//----------------------------------------------
//----------------------------------------------
//----------------------------------------------
//----------------------------------------------


void __fastcall TForm1::Button11Click(TObject *Sender)
{
TLineSeries* Saving1=0;
TLineSeries* Saving2=0;
if(RadioButton1->Checked)
{
 Saving1=Series1;
  Saving2=Series2;
}
if(RadioButton2->Checked)
{
 Saving1=Series3;
  Saving2=Series4;
}
if(RadioButton3->Checked)
{
 Saving1=Series7;
  Saving2=Series8;
}

if(RadioButton4->Checked)
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

void __fastcall TForm1::Button12Click(TObject *Sender)
{

if (RadioButton1->Checked)
{
	LoadingDataFromFile(Series1,Series2);
}
if (RadioButton2->Checked)
{
	LoadingDataFromFile(Series3,Series4);
	for (int i=0; i < l; i++)
	{
		Us_n[i]=Series3->YValues->Value[i];
		Uy_n[i]=Series4->YValues->Value[i];
	}
}
if (RadioButton3->Checked)
{
	LoadingDataFromFile(Series7,Series8);
}

if (RadioButton4->Checked)
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

 for (int i=0; i < LineSeries3->YValues->Count; i++) {
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
	RoundM(x,Series7->YValues->Count);
    for(int j=0;j<Series7->YValues->Count;j++)
		Series7->YValues->Value[j]=x[j];
	for(int j=0;j<Series8->YValues->Count;j++)
		x[j]=Series8->YValues->Value[j];
	RoundM(x,Series8->YValues->Count);
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
void __fastcall TForm1::Button2Click(TObject *Sender)
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
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//-----------curveFitting----------------------------------------------------
void curveFitting(long double * inX, long double *inY, long double * out, int length)
{
// выражение f(x)=x^5*p1+x^4*p2+x^3*p3+x^2*p4+x*p5+p6
const int a=6;
//long double * fullMatrix[a+1];
long double ** fullMatrix=new long double*[length];

for (int i = 0; i < length; i++) {
	fullMatrix[i]=new long double[a+1];

}
//for (int i = 0; i < length; i++) {
 //	fullMatrix[i]=new long double[length];
//}
/*
формируем матрицу вида

-y x^0 x^1 x^2 x^3 x^4 x^5  7 столбцов и length строк

x^5 x^4 x^3 x^2 x^1 x^0 -y
*/
for (int i = 0; i < length; i++) {
	for (int j = 0; j < a; j++) {
		fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

	}
	fullMatrix[i][a]=-inY[i];
}
long double K[a+1][a+1]={0}; // сюда сохраним результат перемножения x'*x
	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
	}}}
	// перемножение работает, проверено матлабом

    // после перемножения мило делим всё на постоянный коэффициент
   /* for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
		  // K[i][j]*=1000;

		}
        }*/

	// K5 - содержит 6 строк и 6 столбцов, т.е. без свободных членов.
	long double *K5[a];
	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	// Ks - столблец из 6 строк, свободные члены.
	long double Ks[a]={0};

	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
		K5[i][j]=K[i][j]; // копируем первые 5 строк
		//blabla=K5[i][j];
		}
	}

	// а 6ая строка - среднее арифметическое 6 и 7 строк
	for (int i = 0; i < a; i++) {
		K5[5][i]=(K[5][i]+K[6][i])/2;
	}

	for (int i = 0; i < a; i++) {
		Ks[i]=K[i][a];
	}

	long double d0=determinant(K5,a); // детерминант правилен

	long double **delta[a];
	for (int i = 0; i < a; i++) {
		 delta[i]=new long double*[a];
		for (int j = 0; j < a; j++) {
			delta[i][j]=new long double[a];

	}}

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
		for (int k = 0; k < a; k++) {
		if (k!=i) {
		 delta[i][j][k]=K5[j][k];
		}
		else
		{
		delta[i][j][k]=Ks[j];
		}
		//blabla=delta[i][j][k];
	}}}

	long double p[a]={0};

	for (int i = 0; i < a; i++) {
		p[i]=determinant(delta[i],a)/d0;
	}
	for (int i = 0; i <length; i++) {
        out[i]=pow(inX[i],5)*p[0]+pow(inX[i],4)*p[1]+pow(inX[i],3)*p[2]+pow(inX[i],2)*p[3]+pow(inX[i],1)*p[4]+p[5];
	}
for (int i = 0; i < a; i++) {
	delete[] K5[i];
}

for (int i = 0; i < length; i++) {
	delete[] fullMatrix[i];
}
delete[] fullMatrix;
for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
			delete[] delta[i][j];
	}
			 delete[] delta[i];
	}

}
//---------------------------------------------------------------------------


long double determinant (long double ** Arr,int size)
{
int i,j;
long double det=0;
long double** matr;
switch (size) {
case 1: return Arr[0][0];
case 2: return Arr[0][0]*Arr[1][1]-Arr[0][1]*Arr[1][0];
default:
  matr=new long double*[size-1];
				for(i=0;i<size;++i)
				{
						for(j=0;j<size-1;++j)
						{
								if(j<i)
										matr[j]=Arr[j];
								else
										matr[j]=Arr[j+1];
						}
						det+=pow((long double)-1, (i+j))*determinant(matr, size-1)*Arr[i][size-1];
				}
				delete[] matr;

  return det;
}
}
//---------------------------------------------------------------------------


void __fastcall TForm1::Button6Click(TObject *Sender)
{


int length=Series1->XValues->Count;
long double *inX= new long double [length];
long double *inY= new long double [length];
long double *out= new long double [length];

for (int i = 0; i < length; i++) {
	inX[i]=Series2->XValues->Value[i];
	inY[i]=Series2->YValues->Value[i];
}
Series4->Clear();
curveFitting(inX,inY,out,length);
for (int i = 0; i < length; i++) {
	Series4->AddXY(inX[i],out[i],"",clRed);
}

delete[] inX;
delete[] inY;
delete[] out;


}
//---------------------------------------------------------------------------


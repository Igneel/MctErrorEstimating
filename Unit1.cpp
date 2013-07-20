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


__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
const int l=201;//11;  // ���������� �����
const int NumberOfNumbersAfterPoint=4;
long double q=1.60217646E-19; // ����� ���������
long double h; // �������� ����
long double B[l]={0};
long double sxx[l]={0};  // ������ ������������
long double sxy[l]={0};
long double s_eff[l]={0};  // �����������
long double Rh_eff[l]={0};  // ������ �����
long double Us[l]={0};     // ���������� ������
long double Uy[l]={0};     // ���������� ������

long double s_eff_n[l]={0};  // �����������
long double Rh_eff_n[l]={0};  // ������ �����
long double Us_n[l]={0};  // ���������� ������
long double Uy_n[l]={0};  // ���������� ������

long double sxx_n[l]={0};  // ������ ������������
long double sxy_n[l]={0};

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

void ParamsKRT(void);

void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8);



// ������ �������� ������������
void __fastcall TForm1::Button1Click(TObject *Sender)
{
ParamsKRT();
Series1->Clear();     // ������ ��� �������
Series2->Clear();
Series5->Clear();
LineSeries5->Clear();

LineSeries1->Clear();
LineSeries7->Clear();

LineSeries3->Clear();
LineSeries9->Clear();
long double u[3]={0}; // ����������� ��
long double n[3]={0}; // ������������ ��
for(int i=0;i<3;i++) // ������ ��� �������� � �����
{
	u[i]=g_Nz_par->Cells[2][i+1].ToDouble();
	n[i]=g_Nz_par->Cells[1][i+1].ToDouble();
}
h=g_hsize->Text.ToDouble(); // ��������� �������� ����

B[0]=0;
for(int i=1;i<l;i++)
{
	B[i]=B[i-1]+h;  // ��� ���� ��������� ���� �� 0 �� 2 ��
}
// ������������ ���������� ������� ������������


for(int i=0;i<l;i++)
{
sxx[i]=sxy[i]=0;
	for(int j=0;j<3;j++)
	{
		sxx[i]+=q*n[j]*u[j]/(1.0+u[j]*u[j]*B[i]*B[i]);
		sxy[i]+=q*n[j]*u[j]*u[j]*B[i]/(1.0+u[j]*u[j]*B[i]*B[i]);
	}
	Series1->AddXY(B[i],sxx[i],"",clRed);  // ������ �� �� ��������
	Series2->AddXY(B[i],sxy[i],"",clRed);
	Series5->AddXY(B[i],sxx[i],"",clRed);
	LineSeries5->AddXY(B[i],sxy[i],"",clRed);


}
Button3->Enabled=1;
//-------------------------------------------------
//- ������������ ����������� ��������
for(int i=0;i<l;i++)
{
	s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
	Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]); // 1/B
	LineSeries1->AddXY(B[i],s_eff[i],"",clRed);
	LineSeries7->AddXY(B[i],Rh_eff[i],"",clRed);
}
//-------------------------------------------------
long double d=LabeledEdit1->Text.ToDouble(); // 1e-5; // ������� �������
long double cb=LabeledEdit2->Text.ToDouble(); //3; // ��������� � � b
long double I=LabeledEdit3->Text.ToDouble(); //1e-3; // ��� �� 1e-3 �� 1e-4


//-------------------------------------------------------------------------------------
for(int i=0;i<l;i++)
{
   //Us(B)=cb/d*I/s(B)
   //Uy(B)=Rh(B)*I*B/d

   // ��� � ���� ��������� ����� ����������� ���.
Us[i]=cb/d*I/s_eff[i];
Uy[i]=Rh_eff[i]*I/d;   // Rh_eff[i]*I*B[i]/d;

	LineSeries3->AddXY(B[i],Us[i],"",clRed);
	LineSeries9->AddXY(B[i],Uy[i],"",clRed);
}

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
void __fastcall TForm1::Button3Click(TObject *Sender)
{
srand(time(NULL));
long double y[l]={0}; // ��� ��� sxx
long double y1[l]={0};  // ��� ��� sxy
long double vz[3]={0};  // � ��� � ��� � %
ShumAdding(Us,y,vz,Edit5->Text.ToDouble(),l);
Form1->Memo1->Lines->Add(FloatToStr(vz[0]));
Form1->Edit1->Text=FloatToStr(vz[1]); // ���
Form1->Edit3->Text=FloatToStr(vz[2]);

ShumAdding(Uy,y1,vz,Edit5->Text.ToDouble(),l);
Form1->Memo1->Lines->Add(FloatToStr(vz[0]));
Form1->Edit2->Text=FloatToStr(vz[1]); // ���
Form1->Edit4->Text=FloatToStr(vz[2]);

Series3->Clear();
Series4->Clear();
for(int i=0;i<l;i++)
{
 Series3->AddXY(B[i],y[i],"",clRed);  // ������� ������� � �����
 Us_n[i]=y[i];                        // � ��������� �� ��� ��������� �������
 Series4->AddXY(B[i],y1[i],"",clRed);
 Uy_n[i]=y1[i];
}
Button8->Enabled=1;
}
//---------------------------------------------------------------------------
// ���������� ����������� �����������
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
if(Key=='�') Key='E';  // ������� � ������� �� ����������
if (Key=='�') Key='E';
if ((Key>='0' && Key<='9') || Key==',' || Key=='\b' || Key=='E' ) ;
else
Key=0;
}
//---------------------------------------------------------------------------
// ��������� ������ ����, �� ���� ����� �� �� ��� ������)
void __fastcall TForm1::Button5Click(TObject *Sender)
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

void ParamsKRT(void)
{
long double x=Form1->LabeledEdit4->Text.ToDouble();//0.22; // ������� ������ ������
long double T=Form1->LabeledEdit5->Text.ToDouble();//77; // �����������
long double m0=9.10938188E-31; // ����� ��������� ���� ��� � �� ����� � �����-��:)
long double mh=0.443*m0; // ����� ������� �����
long double ml=0.001*m0; // ����� ������ �����

long double ph=Form1->LabeledEdit8->Text.ToDouble();//1.0E22; // ������������ ������� �����
long double pl=pow(ml/mh,1.5)*ph*50; // ������������ ������ ����� ��������� �� 50
long double n=-pow(niSob(T,x),2)/ph;
long double A=Form1->LabeledEdit6->Text.ToDouble();//5; // 5-8
long double k=Form1->LabeledEdit7->Text.ToDouble();//1.3; //1.3-1.5
long double ue=A*pow(T/77.0,-k); // ����������� ���������� � ������� 77-300�

long double uh=0.005*ue;   // ����������� ������� �
long double ul=0.1*ue;     // ������ ����� � ������ �����������
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


// �������� ������ ��������
void __fastcall TForm1::Button8Click(TObject *Sender)
{
long double d=LabeledEdit1->Text.ToDouble(); // 1e-5; // ������� �������
long double cb=LabeledEdit2->Text.ToDouble(); //3; // ��������� � � b
long double I=LabeledEdit3->Text.ToDouble(); //1e-3; // ��� �� 1e-3 �� 1e-4
LineSeries3->Clear();
LineSeries9->Clear();
LineSeries1->Clear();
LineSeries7->Clear();
for(int i=0;i<l;i++)
{
  // ������ ������� � �������� �����������
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

// ���������� ��� ��� ���������� ��������

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
int h=Edit7->Text.ToInt(); // ��� �� �����������
int T1=80; // ��������� �����������
int Tmax=300; // �������� �����������

int koef=1;// ��������� ����������� ����
int endkoef=5;  // �������� �����������

int h_koef=1; // ��� �� ������ ����
Edit8->Text=IntToStr(80); // ����� ����� �������, �������� - ��� ��� ������������� � ����������� �������!
g_hsize->Text=FloatToStr(0.0125);
UnicodeString oldName; // ������ ��� �����
UnicodeString fName;   // ����� ��� �����

// ���� � ������� ��� ���� �������
if (sg1->Execute()) {
	Memo1->Lines->Add( sg1->FileName); // ������� ��� � ����
}
oldName = sg1->FileName;     // � ����������

for (int T=T1; T <= Tmax; T+=h)  // ���� �� ����������� � �������� �����
{
     LabeledEdit5->Text=IntToStr(T);  // ��� ������� ��������� �������� �����������
	 Button1->Click(); // �������� ������ ����������
     // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_params_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button2->Click(); // ��������� ��������� �����


	 RadioButton1->Checked=true; // �������� ��������� ������ ��������
	 // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button4->Click();     	 // ��������� ��� ����� ���������� ������� �������
	 fName="T_"+IntToStr(T)+"_tenzor_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // ��������� 11 ����� ���������� ������� �������
	 Button11->Click();


	 RadioButton4->Checked=true; // �������� ��������� ������ ���������� ��������
     // ����� ��� �����
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_vseZnachenia_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 Button4->Click();     	 // ��������� ��� ����� ���������� ������� ��������
	 fName="T_"+IntToStr(T)+"_Us_Uy_ideal_11Znacheniy_"+Edit3->Text+".txt";
	 sg1->FileName=oldName+fName;
	 // ��������� 11 ����� ���������� ������� ��������
	 Button11->Click();

     for (int i=koef; i <= endkoef; i+=h_koef) // ����� ���� ���������� ���� � ��������������
	 {
		  Edit5->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1]))); // ������ ������������
		  Edit6->Text=FloatToStr(100.0/(long double)(i*fabs(Uy[l-1])));
		  Button3->Click(); // ��������� ����

		  RadioButton2->Checked=true; // �������� ����������� ������ ����������� �������
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 Button4->Click();     	 // ��������� ��� ����� ���������� ������� �������
		 fName="T_"+IntToStr(T)+"_Us_Uy_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		 sg1->FileName=oldName+fName;
		 // ��������� 11 ����� ���������� ������� �������
		 Button11->Click();

		 // ��������� ����������
		 Button10->Click();

		 // ��������� ���������� ���������� (��� �����)-------------------------
		 RadioButton3->Checked=true; // �������� ������������� ������
		 fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		 sg1->FileName=oldName+fName;
		 Button4->Click();
         //---------------------------------------------------------------------

		  Button8->Click(); // �������� ������
		  //--------------------------------------------------------------------
		  // ���� ��������� ���������� ����������
		  RadioButton2->Checked=1;
		  fName="T_"+IntToStr(T)+"_tenzor_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  Button4->Click();// ������ ���������
		  fName="T_"+IntToStr(T)+"_tenzor_11Znacheniy_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  // ��������� 11 ����� �������
	      Button11->Click();
		  //--------------------------------------------------------------------
		  //-------��������� ����������� ����������-----------------------------

		  //1T_80_Us_Uy_vseZnachenia_filtr_k_50.txt
		  fName="T_"+IntToStr(T)+"_Us_Uy_vseZnachenia_filtr_k_"+IntToStr(i)+".txt";
		  sg1->FileName=oldName+fName;
		  RadioButton4->Checked=1;
		  Button12->Click();

		  //-------�������� ������ ��� ������������� ��������
		  Button8->Click();
		  //--------------------------------------------------------------------
          //--------------------------------------------------------------------
		  RadioButton2->Checked=1;// ��������� ���������� ������������� ��������
		  fName="T_"+IntToStr(T)+"_tenzor_filt_vseZnachenia_k_"+IntToStr(i)+"_sko_p_xx"+Edit3->Text+"_sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  Button4->Click();
		  // � ���� ��������: ��� ����� � 11 �����.
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
// ���� �������� ������������� �������� ���������� ������������!

int length=Edit8->Text.ToInt();

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

 delete [] x;
 delete [] y;    */

  extrapolate2Degree(Series3, 0, 2.75, 0.01,Series3);
  extrapolate2Degree(Series4, 0, 2.75, 0.01,Series4);



Tr_Filter(Series3,Series7,Edit8->Text.ToInt(),5000,15,25);
Tr_Filter(Series4,Series8,Edit8->Text.ToInt(),5000,15,25);

}



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
// ������� ��������� ��� ����� ���� ��������
// �.�. ������������� � ������ ������� ��� �������� ������������.
void SavingAllPoints(TLineSeries* Series7,TLineSeries* Series8)
{
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



void __fastcall TForm1::Button6Click(TObject *Sender)
{



extrapolate5Degree(Series2, 0, 2.5, 0.2,Series4);



}
//---------------------------------------------------------------------------



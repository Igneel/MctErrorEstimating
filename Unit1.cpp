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
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
}
const int l=11;  // ���������� �����
long double q=1.60217646E-19; // ����� ���������
long double h=0.1; // �������� ����
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
// ������ �������� ������������
void __fastcall TForm1::Button1Click(TObject *Sender)
{
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
		if(j==0) sxy[i]*=-1;
	}
	Series1->AddXY(B[i],sxx[i],"",clRed);  // ������ �� �� ��������
	Series2->AddXY(B[i],sxy[i],"",clRed);
	Series5->AddXY(B[i],sxx[i],"",clRed);
	LineSeries5->AddXY(B[i],sxy[i],"",clRed);


}
//-------------------------------------------------
//- ������������ ����������� ��������
for(int i=0;i<l;i++)
{
	s_eff[i]=(sxx[i]*sxx[i]+sxy[i]*sxy[i])/sxx[i];
	Rh_eff[i]=sxy[i]/(sxx[i]*sxx[i]+sxy[i]*sxy[i]);
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
Uy[i]=Rh_eff[i]*I*B[i]/d;

	LineSeries3->AddXY(B[i],Us[i],"",clRed);
	LineSeries9->AddXY(B[i],Uy[i],"",clRed);
}

}
//---------------------------------------------------------------------------
// ���������� ����������� ��� ����������, ���������� ������ ��� ���� ����� �� �������
// ����� ����� ������� ��� �� � ���� ��������� �������� - � ���� ��� ������
void RoundM(long double * x,int length)
{
int S=1000;
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
// ���������� ����������� ��� ����
void __fastcall TForm1::Button2Click(TObject *Sender)
{
TStringList * tsl=new TStringList();
wchar_t *s;

RoundM(sxx,l);
RoundM(sxy,l);

for(int i=0;i<l;i++)
{

tsl->Add(FloatToStr(B[i])+"\t"+FloatToStr(sxx[i])+"\t"+FloatToStr(sxy[i]));
s=tsl->Strings[i].w_str() ;
for(int j=0;j<tsl->Strings[i].Length();j++)
if(s[j]==',')
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);

}
if (sg1->Execute()) {
tsl->SaveToFile(sg1->FileName);
}
}
//---------------------------------------------------------------------------
// �������������� �����-�������
void BoxMull(long double *z,int l)
{
long double x,y;

int i;
for(i=0;i<l-1;i+=2)
{
double s=3;
while (s>1 || s<=0)
{
int a=rand();
x=(-1000+rand()%2000)/1000.0;
y=(-1000+rand()%2000)/1000.0;
s=x*x+y*y;
}
z[i]=x*sqrt(-2*log(s)/s);
z[i+1]=y*sqrt(-2*log(s)/s);
i=i;
}
}


//--------------------------------------------------------------------------
// ������ �������� ������������� ����������
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
// ������ ��������������� ��������
//-------------------------------------------------

 long double Mo (long double *x,int l)
 {
 long double M=0;
 for(int i=0;i<l;i++)
 M+=x[i];
 return M/l;
 }

//---------------------------------------------------------------------------
// ���������� �������� ��������������� �������� ����.
void ShumAdding(long double *x,long double *out,long double *ret, long double koeff)
{


long double y[l]={0}; // ��� ��� ������ �������
BoxMull(y,l);

for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;
  ret[0]=Mo(y,l); // �������������� ��������
  ret[1]=Sko(x,out,l);  // ���
  ret[2]=ret[0]/Mo(x,l)*100; // ��� � %  �� ��������������� �������� ��������� �������
}

// ��������� ����
void __fastcall TForm1::Button3Click(TObject *Sender)
{
srand(time(NULL));
long double y[l]={0}; // ��� ��� sxx
long double y1[l]={0};  // ��� ��� sxy
long double vz[3]={0};  // � ��� � ��� � %
ShumAdding(Us,y,vz,Edit5->Text.ToDouble());
Form1->Memo1->Lines->Add(FloatToStr(vz[0]));
Form1->Edit1->Text=FloatToStr(vz[1]); // ���
Form1->Edit3->Text=FloatToStr(vz[2]);

ShumAdding(Uy,y1,vz,Edit5->Text.ToDouble());
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

}
//---------------------------------------------------------------------------
// ���������� ����������� �����������
void __fastcall TForm1::Button4Click(TObject *Sender)
{
TStringList * tsl=new TStringList();
wchar_t *s;
for(int i=0;i<l;i++)
{
tsl->Add(FloatToStr(Form1->Series3->XValues->Value[i])+"\t"+Form1->Series3->YValues->Value[i]+"\t"+Form1->Series4->YValues->Value[i]);
s=tsl->Strings[i].w_str() ;
for(int j=0;j<tsl->Strings[i].Length();j++)
if(s[j]==',')
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);

}
if (sg1->Execute()) {
tsl->SaveToFile(sg1->FileName);
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

 // ��� %  �� ��������������� ��������
 Edit3->Text=FloatToStr(Edit1->Text.ToDouble()/Mo(sxx,l)*100);
 Edit4->Text=FloatToStr(Edit2->Text.ToDouble()/Mo(sxy,l)*100);

}
//---------------------------------------------------------------------------
// ���������� ����������� ������������ ����������
long double niSob(long double T, long double x)

{
long double k=1.380648813E-23/1.60217646E-19; // ���������� ��������� � ��������-�������
long double Eg=fabs(-0.302+1.93*x-0.81*x*x+0.832*x*x*x+5.35E-4*(1-2*x)*T);  // ������ ���� ������������� ��?
return  (5.585-3.82*x+0.001753*T-0.001364*x*T)*1E20*pow(Eg,1.5)*pow(T,1.5)*exp(-Eg/2/k/T); // ����������� ������������

}

// ���������� ����������
void __fastcall TForm1::Button6Click(TObject *Sender)
{
long double x=LabeledEdit4->Text.ToDouble();//0.22; // ������� ������ ������
long double T=LabeledEdit5->Text.ToDouble();//77; // �����������
long double m0=9.10938188E-31; // ����� ��������� ���� ��� � �� ����� � �����-��:)
long double mh=0.443*m0; // ����� ������� �����
long double ml=0.001*m0; // ����� ������ �����

long double ph=LabeledEdit8->Text.ToDouble();//1.0E22; // ������������ ������� �����
long double pl=pow(ml/mh,1.5)*ph*100; // ������������ ������ ����� ��������� �� 100

long double n=-pow(niSob(T,x),2)/ph;
long double A=LabeledEdit6->Text.ToDouble();//5; // 5-8
long double k=LabeledEdit7->Text.ToDouble();//1.3; //1.3-1.5
long double ue=A*pow(T/77.0,-k); // ����������� ���������� � ������� 77-300�

long double uh=0.005*ue;   // ����������� ������� �
long double ul=0.1*ue;     // ������ ����� � ������ �����������
ue*=-1;

g_Nz_par->Cells[1][1]=FloatToStr(n);
g_Nz_par->Cells[2][1]=FloatToStr(ue);
g_Nz_par->Cells[1][2]=FloatToStr(pl);
g_Nz_par->Cells[2][2]=FloatToStr(ul);
g_Nz_par->Cells[1][3]=FloatToStr(ph);
g_Nz_par->Cells[2][3]=FloatToStr(uh);
Button1->Enabled=true;
}
//---------------------------------------------------------------------------

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
if(B[i])
Rh_eff_n[i]=d*Uy_n[i]/I/B[i];
else
Rh_eff_n[i]=0;//d*Uy_n[i]/I;




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


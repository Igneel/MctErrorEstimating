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
const int l=201;//11;  // ���������� �����
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
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh);
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);
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
		sxy[i]+=(j==0?-1:1)*q*n[j]*u[j]*u[j]*B[i]/(1.0+u[j]*u[j]*B[i]*B[i]);
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
//int a=rand();
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
Button8->Enabled=1;
}
//---------------------------------------------------------------------------
// ���������� ����������� �����������
void __fastcall TForm1::Button4Click(TObject *Sender)
{
if (RadioButton2->Checked)
{
	 SavingAllPoints(Series3,Series4);
}
if (RadioButton3->Checked)
{
     SavingAllPoints(Series7,Series8);
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

void ParamsKRT(void)
{
long double x=Form1->LabeledEdit4->Text.ToDouble();//0.22; // ������� ������ ������
long double T=Form1->LabeledEdit5->Text.ToDouble();//77; // �����������
long double m0=9.10938188E-31; // ����� ��������� ���� ��� � �� ����� � �����-��:)
long double mh=0.443*m0; // ����� ������� �����
long double ml=0.001*m0; // ����� ������ �����

long double ph=Form1->LabeledEdit8->Text.ToDouble();//1.0E22; // ������������ ������� �����
long double pl=pow(ml/mh,1.5)*ph*100; // ������������ ������ ����� ��������� �� 100

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

// ���������� ����������
void __fastcall TForm1::Button6Click(TObject *Sender)
{
ParamsKRT();
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

void __fastcall TForm1::Button9Click(TObject *Sender)
{
int h=Edit7->Text.ToInt(); // ��� �� �����������
int T1=80; // ��������� �����������
int Tmax=300; // �������� �����������
int koef=50;// ��������� ����������� ����
int endkoef=150;  // �������� �����������
int h_koef=50; // ��� �� ������ ����
UnicodeString oldName;
UnicodeString fName;
if (sg1->Execute()) {
	Memo1->Lines->Add( sg1->FileName);
}
oldName = sg1->FileName;

for (int T=T1; T <= Tmax; T+=h)
{
     LabeledEdit5->Text=IntToStr(T);
	 Button1->Click(); // ����������
	 for (int i=koef; i <= endkoef; i+=h_koef)
	 {
		  Edit5->Text=IntToStr(i);
		  Edit6->Text=IntToStr(i);
		  Button3->Click(); // ���������

		  // � ����� ���� ��������� ��������� ����������.

		  // ���� ��������� ���������� ����������
		  RadioButton2->Checked=1;
		  Button4->Click();// ������ ���������


		  Button10->Click();// ����������� ��
		  RadioButton3->Checked=1;// ��������� ���������� ����������
		  Button4->Click();
		  // � ���� ��������: ��� ����� � 11 �����.
          Button11->Click();

		  Button8->Click(); // �������� ������
		  fName="T"+IntToStr(T)+" k"+IntToStr(i)+" sko_p_xx"+Edit3->Text+" sko_p_xy"+Edit3->Text+".txt";
		  sg1->FileName=oldName+fName;
		  //Memo1->Lines->Add(sg1->FileName);
		  //-----------------------------------
		  //-----------------------------------
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
			tsl->SaveToFile(sg1->FileName);

 //---------------------------------------------
 //---------------------------------------------
	 }
}
// � ������ ��� ������:
// �������� ����������� � �������� �����
// 1. �������� ��������� ������
// ������� �������
// ��������� ��� � Us � Uy
// ��������� �������� ������
// ��������� ��� �� � �������� ������
// ������ ��� ��� ���������� ������� ����
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button10Click(TObject *Sender)
{
int length=Edit8->Text.ToInt();

double *x=new double[l+length];
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
}

delete [] x;
delete [] y;
            // �� ���-�� �� ����� ����� �����, ���� ������������� ��-����.
Tr_Filter(Series3,Series7,Edit8->Text.ToInt(),5000,15,25);
Tr_Filter(Series4,Series8,Edit8->Text.ToInt(),5000,15,25);
}
//---------------------------------------------------------------------------
// ��������!!! �������� �� ��������!!! ������������ ����������!!!---------------
double Filter (const double in[], double out[], int sizeIn, int length, double Fdisk, double Fpropysk,double Fzatyh)
{
int N = length; //����� ������� 20
long double Fd = Fdisk; //������� ������������� ������� ������ 2000
long double Fs = Fpropysk; //������� ����� ������ �����������  10
long double Fx = Fzatyh; //������� ������ ������ ���������    20

long double *H= new long double [N] ; //���������� �������������� �������
long double *H_id= new long double [N]; //��������� ���������� ��������������
long double *W= new long double [N]; //������� �������

/*long double H [N] = {0}; //���������� �������������� �������
long double H_id [N] = {0}; //��������� ���������� ��������������
long double W [N] = {0}; //������� �������  */

//������ ���������� �������������� �������
long double Fc = (Fs + Fx) / (2 * Fd);

for (int i=0;i<N;i++)
{
if (i==0) H_id[i] = 2*M_PI*Fc;
else H_id[i] = sinl(2*M_PI*Fc*i )/(M_PI*i);
// ������� ������� ��������
W [i] = 0.42 - 0.5 * cosl((2*M_PI*i) /( N-1)) + 0.08 * cosl((4*M_PI*i) /( N-1));
H [i] = H_id[i] * W[i];
}

//���������� ���������� ��������������
long double SUM=0;
for (int i=0; i<N; i++) SUM +=H[i];
for (int i=0; i<N; i++) H[i]/=SUM; //����� ������������� ����� 1

//----------------------------------------------------------------
// ������ ������������� �������
//for(int i=0;i<N;i++)
//Form1->Memo2->Lines->Add(FloatToStr(H[i]));
//----------------------------------------------------------------

//���������� ������� ������
for (int i=0; i<sizeIn; i++)
{
out[i]=0.;
for (int j=0; j<(i>N-1?N-1:i); j++)// �� ����� ������� �������
out[i]+= H[j]*in[i-j];
}
delete [] H;
delete [] H_id;
delete [] W;
return (N-1)/2.0;
}
// ������� �������� ��� �������.  ----------------------------------------------
double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh)
{
if(!inS->YValues->Count)   // ���� ������ - ������, �������.
{
ShowMessage("������ ������!");
return 0;
}
int size=inS->YValues->Count;  // �������� ������
//size=inS->XValues->Count;  // �������� ������
double *in=new double[size];  // �������� ������
for(int i=0;i<size;i++)       // ��������
in[i]=inS->YValues->Value[i];
double *out=new double[size]; // �������� ������ ��� �������� ��������
double k=Filter(in,out,size,length,Fdisk,Fpropysk,Fzatyh); // �������� ������
 k*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// ��������� ����� ���
//----------------------------------------------
//---------���������� ��� ���������� ���������� ����
// �������� - ��� ���� �������� �� ��������� ��� ������ ����������� ������ ��
// ��� �����.
/*double *in2=new double[size];  // �������� ������
for(int i=0;i<size;i++)       // ��������
in2[i]=inS->XValues->Value[i];
double *out2=new double[size]; // �������� ������ ��� �������� ��������
double k2=Filter(in2,out2,size,length,Fdisk,Fpropysk,Fzatyh); // �������� ������
k2*=(inS->XValues->MaxValue-inS->XValues->MinValue)/(double)inS->XValues->Count;// ��������� ����� ���
*/

//----------------------------------------------
 outS->Clear(); // ������ ������, ����� �������
for(int i=0;i<size;i++) // �������
{
outS->AddXY(inS->XValues->Value[i]-k,out[i],"",clBlue);
//Form1->Series2->AddY(out2[i]-k+k2,"",clBlue);
//Form1->Series3->AddY(inS->XValues->Value[i]-k,"",clRed);
//Form1->Series4->AddY(out[i],"",clBlack);
//Form1->Series5->AddY(out2[i],"",clYellow);

 }
delete[] in;  // �����������
//delete[] in2;  // �����������
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

if(!Saving1)
return;

double points[11]={0};
double shag=0.2;
for (int i=1; i < 11; i++) {
    points[i]=points[i-1]+shag;
}

TStringList * tsl=new TStringList();
wchar_t *s;
 //int counting=0;
for (int i = 0; i < 11; i++) {
int index=0;
double r=4;
	for(int k=0;k<l;k++)
	{
	
	if(fabs(Saving1->XValues->Value[k]-points[i])<=r)
	{
		r=fabs(Saving1->XValues->Value[k]-points[i]);
		index=k;
		}
	else
		break;
	}
		
	tsl->Add(FloatToStr(Saving1->XValues->Value[index])+"\t"+Saving1->YValues->Value[index]+"\t"+Saving2->YValues->Value[index]);
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
delete tsl;
}
//---------------------------------------------------------------------------

void LoadingDataFromFile(TLineSeries * Series1,TLineSeries * Series2)
{
TStringList * tls=new TStringList();
if (Form1->sg1->Execute()) {
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

}
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
TStringList * tsl=new TStringList();
wchar_t *s;
for(int i=0;i<Series7->YValues->Count;i++)
{
tsl->Add(FloatToStr(Series7->XValues->Value[i])+"\t"+Series7->YValues->Value[i]+"\t"+Series8->YValues->Value[i]);
s=tsl->Strings[i].w_str() ;
for(int j=0;j<tsl->Strings[i].Length();j++)
if(s[j]==',')
	s[j]='.';
tsl->Delete(i);
tsl->Add(s);

}
if (Form1->sg1->Execute()) {
tsl->SaveToFile(Form1->sg1->FileName);
}
delete tsl;
}

//---------------------------------------------------------------------------


#pragma hdrstop

#include "FilteringUnit.h"

//---------------------------------------------------------------------------
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
}
delete[] in;  // �����������
//delete[] in2;  // �����������
delete[] out;
//delete[] out2;
return k;
}


// ������� �������� ��� �������.  ----------------------------------------------
double TrForMassiveFilter(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh)
{

// ����� ����������� ������ ������ �������� ��������.


//int size=inS->YValues->Count;  // �������� ������
double *in=new double[lengthMassive];  // �������� ������
for(int i=0;i<lengthMassive;i++)       // ��������
{
	in[i]=inY[i];
}
double *out=new double[lengthMassive]; // �������� ������ ��� �������� ��������
double k=Filter(in,out,lengthMassive,lengthFilter,Fdisk,Fpropysk,Fzatyh); // �������� ������
 k*=(inB[lengthMassive-1]-inB[0])/(double)lengthMassive;// ��������� ����� ���
 // ���� ��� ��� �������� � ������� ���� �� ���������.

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
for(int i=0;i<lengthMassive;i++) // �������
{
	outB[i]=inB[i]-k;
	outY[i]=out[i];
}

delete[] in;  // �����������
//delete[] in2;  // �����������
delete[] out;
//delete[] out2;
return k;
}
//----------------------------------------------
//----------------------------------------------

// ������� ���������� ����������� �������.

long double D(long double deltaP, long double deltaS)
{
/*
����� ��� ������� ����� �������.
*/
	long double a1=5.309E-3;
	long double a2=7.114E-2;
	long double a3=-4.761E-1;
	long double a4=-2.66E-3;
	long double a5=-5.941E-1;
	long double a6=-4.278E-1;

return log10(deltaS*(a1*log10(deltaP)*log10(deltaP)+a2*
	log10(deltaP)+a3)+a4*log10(deltaP)*log10(deltaP)+a5*log10(deltaP)+a6);
}

long double f(long double deltaP, long double deltaS)
{
/*
����� ��� ������� ����� �������.
*/

return 11.01217+0.51244*(log10(deltaP)-log10(deltaS));

}

int calcutaleFilterLength(long double deltaP, long double deltaS, long double deltaF)
{
/*
������������ ����� ������� ��� ������ ����������� �������������.
�������� ������� �� �������� �������� ��� ������� ������ ������, �������� 410.

deltaP - ��������������� � ������ �����������.

deltaS - ��������������� � ������ ����������.

deltaF - ������ ������ �����������, ������������� �� ������� �������������.

N=D(deltaP,deltaS)/deltaF-f(deltaP,deltaS)*deltaF+1;

*/

return D(deltaP,deltaS)/deltaF-f(deltaP,deltaS)*deltaF+1;
}

long double fapprox(long double x)
{
return x*x;
}

long double OptimFilter(long double deltaP, long double deltaS, long double FPropusk, long double Fpodavl, long double FDiskr)
{
long double FPropuskN=FPropusk/FDiskr;
long double FPodavlN=Fpodavl/FDiskr;
long double deltaF=FPodavlN-FPropuskN;

int filterLength=calcutaleFilterLength(deltaP,deltaS,deltaF);

double weigthP = 1;
double weigthS = deltaS/deltaP;

//boost::math::tools::remez_minimax<double> *remez= new boost::math::tools::remez_minimax<double>
 //	( NULL, 1, 2, 0.1, 0.4);
	// ������� ��� �������������.
	// Order of the numerator polynomial.
	//Order of the denominator polynomial.
	// ������� ����� ��������� �����������. End points of the range to optimise over.


//remez();

//filter();

}

//----------------------------------------------
//----------------------------------------------
#pragma package(smart_init)

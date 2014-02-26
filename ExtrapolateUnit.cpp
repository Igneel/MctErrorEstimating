//---------------------------------------------------------------------------

#pragma hdrstop

#include "ExtrapolateUnit.h"
//---------------------------------------------------------------------------

#define DEBUG 1

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ������ �������������.
// ���������� ������������.
long double determinant (long double ** Arr,int size);
// ������� ��������� ����������� ������������.
// ��� ����������� �������.
// �������� �� ��������, ������������ extrapolate.

// ���������� ���-�� �����
//��������� ��������� �� ������� ������, ����������� � ������������ �������� �� �, �������� ����, ��������� �� �������� ������.
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out,const int powPolinom)
{
int lengthCoef=powPolinom+1;  // ���-�� �������������
long double *koef=new long double[lengthCoef];  // ������ �������������

int length=Series->YValues->Count; // ����� �������� �������
int lengthOut = (maxX-minX)/hX+1; // ���-�� ������������ ��������
long double *inX=new long double [length];
long double *inY=new long double [length];
long double *x= new long double [lengthOut];

for(int i=0;i<lengthOut;i++)
{
x[i]=minX+hX*i;  // ��������� ������ �
}

for (int i = 0; i < length; i++) {
	inX[i]=Series->XValues->Value[i];
	inY[i]=Series->YValues->Value[i];
}
out->Clear();
curveFittingUniversal(inX,inY,length,koef,powPolinom);  // ��������� �������� � �������� ������������

int a=lengthCoef;
for (int i = 0; i <lengthOut; i++)
{                         // ��������� ������������������ �������
out->AddXY(x[i], pow(x[i],a-1)*koef[0]+pow(x[i],a-2)*koef[1]+pow(x[i],a-3)*koef[2]+pow(x[i],a-4)*koef[3]+pow(x[i],1)*koef[4]+koef[5],"",clRed);
}

delete[] inX;    // �����������
delete[] inY;
delete[] x;
delete[] koef;

return lengthOut; // ������.
}


int extrapolate2Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out)
{
const int lengthCoef=3;  // ���-�� �������������
long double koef[lengthCoef]={0};  // ������ �������������

int length=Series->YValues->Count; // ����� �������� �������
int lengthOut = (maxX-minX)/hX+1; // ���-�� ������������ ��������
long double *inX=new long double [length];
long double *inY=new long double [length];
long double *x= new long double [lengthOut];

for(int i=0;i<lengthOut;i++)
{
x[i]=minX+hX*i;  // ��������� ������ �
}

for (int i = 0; i < length; i++) {
	inX[i]=Series->XValues->Value[i];
	inY[i]=Series->YValues->Value[i];
}
out->Clear();
// ��������� �������� � �������� ������������
curveFittingUniversal(inX,inY,length,koef,2);
int a=lengthCoef;
for (int i = 0; i <lengthOut; i++)
{                         // ��������� ������������������ �������
out->AddXY(x[i], pow(x[i],a-1)*koef[0]+pow(x[i],a-2)*koef[1]+koef[2],"",clRed);
}

delete[] inX;    // �����������
delete[] inY;
delete[] x;

return lengthOut; // ������.
}

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
//-----------curveFittingUniversal----------------------------------------------
int curveFittingUniversal(long double * inX, long double *inY, const int length,long double *outKoef,const int powPolinom)
{

	// ��� ����� ����.
	// �� ���� - ��������� �������
	// ����������� � ���������������� �� � �����������
	// ������ ������ ������� ������� ������� (��� ���), � ������ ����� ������ ������� ����.

	int a=powPolinom+1;
	// ��������� f(x)=x^2*p1+x*p2+p3

	long double ** fullMatrix=new long double*[length];

	for (int i = 0; i < length; i++) {
		fullMatrix[i]=new long double[a+1];

	}

	long double **K=new long double *[a+1];
	for (int i = 0; i < a+1; i++) {
        K[i]=new long double [a+1];
	}

	long double **K5= new long double* [a];
	long double *Ks=new long double [a];

	long double ***delta=new long double **[a];  // ������� ��� ���������� ������, ���� �� �������� - �������� �����.

		for (int i = 0; i < a; i++) {
			 delta[i]=new long double*[a];
			for (int j = 0; j < a; j++) {
				delta[i][j]=new long double[a];

		}}

	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	long double *p=new long double [a];
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------

	/*
	����, ����� ��������� ����������:
	*/
    long double temp1,temp2,temp3;
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	//--------------------------------------------------------------------------
	/*
	��������� ������� ����

	x^a x^a-1 ...  x^0 y
	*/
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < a; j++) {
		// �������� �������� � �������
		// �� ��������, ������ - ������� ������
		if(DEBUG)
		{
		temp2=inX[i];
			temp1=((a-j-1)==0?1:pow(inX[i],a-j-1));
		}
			fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

		}
		// ��������� ������� ����������� ����� �� �������� �����.
		if(DEBUG)
		temp2= inY[i];
		fullMatrix[i][a]=inY[i];
	}

	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
			if(DEBUG)
			{
			temp1=fullMatrix[k][i];
			temp2=fullMatrix[k][j];
			temp3=K[i][j];
			}
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
				if(DEBUG)
			temp3=K[i][j];

	}}}
	// ������������ ��������, ��������� ��������
	// K5 - �������� a ����� � a ��������, �.�. ��� ��������� ������.


	// Ks - �������� �� 2*a �����, ��������� �����.


	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
        if(DEBUG)
			temp3=K[i][j];
		K5[i][j]=K[i][j]; // �������� ������ 2a-1 �����
		//blabla=K5[i][j];
		}
	}

	// � 6�� ������ - ������� �������������� 6 � 7 �����
	for (int i = 0; i < a; i++) {

		K5[a-1][i]=(K[a-1][i]+K[a][i])/2;
		if(DEBUG)
			temp3=K5[a-1][i];
	}

	for (int i = 0; i < a-1; i++) {

		Ks[i]=K[i][a];
		if(DEBUG)
			temp3=Ks[i];
	}
	Ks[a-1]=(K[a-1][a]+K[a][a])/2;

	long double d0=determinant(K5,a); // ����������� �������� +

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
			for (int k = 0; k < a; k++) {
				if (k!=i)
				{
					// ���������� ������� �� ������� ����� �������� ������������ ��� ����������.
					 delta[i][j][k]=K5[j][k];
				}
				else
				{
					// ������� ������ ���������� ���������� ���������� �������.
					delta[i][j][k]=Ks[j];
				}
				//blabla=delta[i][j][k];
	}}}

	for (int i = 0; i < a; i++) {
		outKoef[i]=p[i]=determinant(delta[i],a)/d0;
	}


	delete[] p;
	// �����������.
	for (int i = 0; i < a; i++) {
		delete[] K5[i];
	}

	delete[] K5;


	for (int i = 0; i < a+1; i++) {
        delete[] K[i];
	}
	delete[] K;

	delete[] Ks;

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

	delete[] delta;

	return a;
}
//---------------------------------------------------------------------------
int curveFittingRational(long double * inX, long double *inY, const int length,long double *outKoef,const int numeratorDegree, const int denominatorDegree)
{

}

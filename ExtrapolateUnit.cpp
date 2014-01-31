//---------------------------------------------------------------------------

#pragma hdrstop
//#include <boost/numeric/ublas/lu.hpp>
//#include <boost/numeric/ublas/matrix.hpp>
//#include <boost/numeric/ublas/vector.hpp>
#include "ExtrapolateUnit.h"
#pragma package(smart_init)
#pragma link "Series"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ������ �������������.
// ���������� ������������.
long double determinant (long double ** Arr,int size);
// ������� ��������� ����������� ������������.
// ��� ����������� �������.
// �������� �� ��������, ������������ extrapolate.
// ����������� ��� ���������, ���� ����������� ���������� ��� ����.


void curveFitting5(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
void curveFitting2(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
// ������������� ��� �������� ����� �������
// ���������� ���-�� �����
//��������� ��������� �� ������� ������, ����������� � ������������ �������� �� �, �������� ����, ��������� �� �������� ������.
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out)
{
const int lengthCoef=6;  // ���-�� �������������
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
curveFitting5(inX,inY,0,length,koef);  // ��������� �������� � �������� ������������

int a=lengthCoef;
for (int i = 0; i <lengthOut; i++)
{                         // ��������� ������������������ �������
out->AddXY(x[i], pow(x[i],a-1)*koef[0]+pow(x[i],a-2)*koef[1]+pow(x[i],a-3)*koef[2]+pow(x[i],a-4)*koef[3]+pow(x[i],1)*koef[4]+koef[5],"",clRed);
}

delete[] inX;    // �����������
delete[] inY;
delete[] x;

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
curveFitting2(inX,inY,0,length,koef);  // ��������� �������� � �������� ������������

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



//-----------curveFitting5----------------------------------------------------
void curveFitting5(long double * inX, long double *inY, long double * out, int length,long double *outKoef)
{
const int a=6;
// ��������� f(x)=x^5*p1+x^4*p2+x^3*p3+x^2*p4+x*p5+p6
//long double * fullMatrix[a+1];
long double ** fullMatrix=new long double*[length];

for (int i = 0; i < length; i++) {
	fullMatrix[i]=new long double[a+1];

}

long double K[a+1][a+1]={0}; // ���� �������� ��������� ������������ x'*x
long double *K5[a];
long double Ks[a]={0};

long double **delta[a];  // ������� ��� ���������� ������, ���� �� �������� - �������� �����.

	for (int i = 0; i < a; i++) {
		 delta[i]=new long double*[a];
		for (int j = 0; j < a; j++) {
			delta[i][j]=new long double[a];

	}}
/*
��������� ������� ����

-y x^0 x^1 x^2 x^3 x^4 x^5  7 �������� � length �����

x^5 x^4 x^3 x^2 x^1 x^0 y
*/
for (int i = 0; i < length; i++) {
	for (int j = 0; j < a; j++) {
	// �������� �������� � �������
	// �� ��������, ������ - ������� ������
		fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

	}
	// ������� ����������� �����.
	fullMatrix[i][a]=inY[i];
}

	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
	}}}
	// ������������ ��������, ��������� ��������
	// K5 - �������� 6 ����� � 6 ��������, �.�. ��� ��������� ������.

	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	// Ks - �������� �� 6 �����, ��������� �����.


	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
		K5[i][j]=K[i][j]; // �������� ������ 5 �����
		//blabla=K5[i][j];
		}
	}

	// � 6�� ������ - ������� �������������� 6 � 7 �����
	for (int i = 0; i < a; i++) {
		K5[a-1][i]=(K[a-1][i]+K[a][i])/2;
	}

	for (int i = 0; i < a-1; i++) {
		Ks[i]=K[i][a];
	}
	Ks[a-1]=(K[a-1][a]+K[a][a])/2;

	long double d0=determinant(K5,a); // ����������� �������� +

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
		for (int k = 0; k < a; k++) {
		if (k!=i) {
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

	long double p[a]={0};

	for (int i = 0; i < a; i++) {
		outKoef[i]=p[i]=determinant(delta[i],a)/d0;
	}


	if(out!=0)  // ���� ��� ������ - ��������� �������
	{
	for (int i = 0; i <length; i++) {
		out[i]=pow(inX[i],a-1)*p[0]+pow(inX[i],a-2)*p[1]+pow(inX[i],a-3)*p[2]+pow(inX[i],a-4)*p[3]+pow(inX[i],1)*p[4]+p[5];
	}
    }


	// �����������.
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

/*using namespace boost::numeric::ublas;

template<class T>
void solve(matrix<T> &A, vector<T> &X, vector<T> &B)
{
  permutation_matrix<T> P(B.size());
  lu_factorize(A,P);
  X = B;
  lu_substitute(A,P,X);
}

void curveFitting2WithBoost(long double * inX, long double *inY, long double * out, int length,long double *outKoef)
{



}   */

//-----------curveFitting2----------------------------------------------------
void curveFitting2(long double * inX, long double *inY, long double * out, int length,long double *outKoef)
{

	// ��� ����� ����.
	// �� ���� - ��������� �������
	// ����������� � ���������������� �� � �����������
	// ������ ������ ������� ������� ������� (��� ���), � ������ ����� ������ ������� ����.

	const int a=3;
	// ��������� f(x)=x^2*p1+x*p2+p3

	long double ** fullMatrix=new long double*[length];

	for (int i = 0; i < length; i++) {
		fullMatrix[i]=new long double[a+1];

	}

	long double K[a+1][a+1]={0}; // ���� �������� ��������� ������������ x'*x
	long double *K5[a];
	long double Ks[a]={0};

	long double **delta[a];  // ������� ��� ���������� ������, ���� �� �������� - �������� �����.

		for (int i = 0; i < a; i++) {
			 delta[i]=new long double*[a];
			for (int j = 0; j < a; j++) {
				delta[i][j]=new long double[a];

		}}
	/*
	��������� ������� ����

	x^2 x^1 x^0 y
	*/
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < a; j++) {
		// �������� �������� � �������
		// �� ��������, ������ - ������� ������
			fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

		}
		// ������� ����������� �����.
		fullMatrix[i][a]=inY[i];
	}

	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
	}}}
	// ������������ ��������, ��������� ��������
	// K5 - �������� 3 ����� � 3 ��������, �.�. ��� ��������� ������.

	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	// Ks - �������� �� 6 �����, ��������� �����.


	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
		K5[i][j]=K[i][j]; // �������� ������ 5 �����
		//blabla=K5[i][j];
		}
	}

	// � 6�� ������ - ������� �������������� 6 � 7 �����
	for (int i = 0; i < a; i++) {
		K5[a-1][i]=(K[a-1][i]+K[a][i])/2;
	}

	for (int i = 0; i < a-1; i++) {
		Ks[i]=K[i][a];
	}
	Ks[a-1]=(K[a-1][a]+K[a][a])/2;

	long double d0=determinant(K5,a); // ����������� �������� +

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
		for (int k = 0; k < a; k++) {
		if (k!=i) {
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

	long double p[a]={0};

	for (int i = 0; i < a; i++) {
		outKoef[i]=p[i]=determinant(delta[i],a)/d0;
	}


	if(out!=0)  // ���� ��� ������ - ��������� �������
	{
	for (int i = 0; i <length; i++) {
		out[i]=pow(inX[i],a-1)*p[0]+pow(inX[i],a-2)*p[1]+p[2];
	}
    }


	// �����������.
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



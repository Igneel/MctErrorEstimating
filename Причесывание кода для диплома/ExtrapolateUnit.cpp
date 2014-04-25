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


long double determinant (std::vector<std::vector<long double> > & Arr,int size)
{
int i,j;
long double det=0;
std::vector<std::vector<long double> > matr;
switch (size) {
case 1: return Arr[0][0];
case 2: return Arr[0][0]*Arr[1][1]-Arr[0][1]*Arr[1][0];
default:
  matr.resize(size-1);
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
  return det;
}
}

int curveFittingUniversal(std::vector<long double> * inX, std::vector<long double> *inY, std::vector<long double> *outKoef,const int powPolinom)
{
  int lenght=(*inY).size();
  // ��������� �������
  // ����������� � ���������������� �� � �����������
  // ������ ������ ������� ������� �������

  int a=powPolinom+1;
  // ��������� f(x)=x^2*p1+x*p2+p3

  std::vector<std::vector<long double> > fullMatrix;
  std::vector<std::vector<long double> > K;
  std::vector<std::vector<long double> > K5;
  std::vector<long double> Ks;
    std::vector<std::vector<std::vector<long double> > > delta;
  std::vector<long double> p;

  fullMatrix.resize(lenght);
  K.resize(a+1);

  for(int i=0;i<=a;i++)
    K[i].resize(a+1);

  K5.resize(a);

  for(int i=0;i<a;i++)
    K5[i].resize(a);

  Ks.resize(a);

  delta.resize(a);
  for(int i=0;i<a;i++)
  {
    delta[i].resize(a);
    for (int j=0; j < a; j++) {
            delta[i][j].resize(a);
    }
  }

    p.resize(a);
  /*
  ��������� ������� ����

  x^a x^a-1 ...  x^0 y
  */
  for (int i = 0; i < lenght; i++) {
    for (int j = 0; j < a; j++) {
    // �������� �������� � �������
    // �� ��������, ������ - ������� ������
      fullMatrix[i].push_back(((a-j-1)==0?1:pow((*inX)[i],a-j-1)));
    }
    // ��������� ������� ����������� ����� �� �������� �����.
    fullMatrix[i].push_back((*inY)[i]);
  }

  for (int i = 0; i <= a; i++) {
    for (int j = 0; j <= a; j++) {
      for (int k=0; k < lenght; k++) {
        K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
  }}}
  // ������������ ��������, ��������� ��������
  // K5 - �������� a ����� � a ��������, �.�. ��� ��������� ������.


  // Ks - �������� �� 2*a �����, ��������� �����.


  for (int i = 0; i < a-1; i++) {
    for (int j = 0; j < a; j++) {
    K5[i][j]=K[i][j]; // �������� ������ 2a-1 �����
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
  }}}

  for (int i = 0; i < a; i++) {
    (*outKoef)[i]=p[i]=determinant(delta[i],a)/d0;
  }
  return a;
}
//---------------------------------------------------------------------------

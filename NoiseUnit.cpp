//---------------------------------------------------------------------------


#pragma hdrstop

#include "NoiseUnit.h"

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// ���������� ����������� ��� ����
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
// ��� � �������� �� -1 �� 1
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

long double Sko (const long double *x0,const long double *x,int l)
{
long double z=0;
for(int i=0;i<l;i++)
z+= pow(fabs((fabs(x[i])-fabs(x0[i]))),2);
z/=(long double)l;
return sqrt(z);
}

//---------------------------------------------------
// ������ ��������������� ��������
//-------------------------------------------------

 long double Mo (const long double *x,int l)
 {
 long double M=0;
 for(int i=0;i<l;i++)
 M+=x[i];
 return M/l;
 }

//---------------------------------------------------------------------------
// ���������� �������� ��������������� �������� ����.
void ShumAdding(const long double *x,long double *out,long double *ret, long double koeff,const int l)
{
long double *y= new long double[l]; // ��� ��� ������ �������
BoxMull(y,l);
for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;
	// ��������� ���� ������������ ��� 1/koef
  ret[0]=Mo(y,l); // �������������� ��������
  ret[1]=Sko(x,out,l);  // ���
  ret[2]=ret[0]/Mo(x,l)*100; // ��� � %  �� ��������������� �������� ��������� �������
  delete[] y;
}

#pragma package(smart_init)

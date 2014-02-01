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
    long double s;
	int i;
	// ��� ��������� ���� ������ ���������%)
	// ��� � ���� ���� �� ��������, ���
	// ���� l - ����� ��������, �� �� ������� �� �������)
	for(i=0;i<l;i+=2)
	{
		do
		{
			x=(-1000+rand()%2000)/1000.0;
			y=(-1000+rand()%2000)/1000.0;
			// ��� � �������� �� -1 �� 1
			s=x*x+y*y;
		}while (s>1.0 || s<=0.0);
		z[i]=x*sqrt(-2.0*log(s)/s);
		if((i+1)<l)
			z[i+1]=y*sqrt(-2.0*log(s)/s);
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
/*for (int i = 0; i < l; i++) {
    y[i]=0;
}     */

BoxMull(y,l);
for(int i=0;i<l;i++)
	out[i]=x[i]+y[i]/koeff;

if(fabs(out[l-1])>10000)
{
long double temp1=x[l-1];
long double temp2=out[l-1];
long double temp3=y[l-1];
long double temp4=koeff;
ShowMessage("������ �������:)");
}
	// ��������� ���� ������������ ��� 1/koef
  ret[0]=Mo(y,l); // �������������� ��������
  ret[1]=Sko(x,out,l);  // ���
  ret[2]=ret[0]/Mo(x,l)*100; // ��� � %  �� ��������������� �������� ��������� �������
  delete[] y;
}

#pragma package(smart_init)

//---------------------------------------------------------------------------
#include <math.h>
#pragma hdrstop

#include "ExtrapolateUnit.h"
#pragma package(smart_init)
#pragma link "Series"
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
// Модуль экстраполяции.
// Вычисление определителя.
long double determinant (long double ** Arr,int size);
// Функция вычисляет подгоночные коэффициенты.
// Для поперечного сигнала.
// Напрямую не вызывать, пользоваться extrapolate.



void curveFitting5(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
void curveFitting2(long double * inX, long double *inY, long double * out, int length,long double *outKoef);
// Экстраполяция для полинома пятой степени
// Возвращает кол-во точек
//Принимает указатель на входные данные, минимальное и максимальное значения по х, величину шага, указатель на выходные данные.
int extrapolate5Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out)
{
const int lengthCoef=6;  // кол-во коэффициентов
long double koef[lengthCoef]={0};  // массив коэффициентов

int length=Series->YValues->Count; // длина входного массива
int lengthOut = (maxX-minX)/hX+1; // кол-во возвращаемых значений
long double *inX=new long double [length];
long double *inY=new long double [length];
long double *x= new long double [lengthOut];

for(int i=0;i<lengthOut;i++)
{
x[i]=minX+hX*i;  // заполняем массив х
}

for (int i = 0; i < length; i++) {
	inX[i]=Series->XValues->Value[i];
	inY[i]=Series->YValues->Value[i];
}
out->Clear();
curveFitting5(inX,inY,0,length,koef);  // выполняем подгонку и получаем коэффициенты

int a=lengthCoef;
for (int i = 0; i <lengthOut; i++)
{                         // вычисляем экстраполированную функцию
out->AddXY(x[i], pow(x[i],a-1)*koef[0]+pow(x[i],a-2)*koef[1]+pow(x[i],a-3)*koef[2]+pow(x[i],a-4)*koef[3]+pow(x[i],1)*koef[4]+koef[5],"",clRed);
}

delete[] inX;    // прибираемся
delete[] inY;
delete[] x;

return lengthOut; // уходим.
}


int extrapolate2Degree(TLineSeries * Series, long double minX, long double maxX, long double hX,TLineSeries *out)
{
const int lengthCoef=3;  // кол-во коэффициентов
long double koef[lengthCoef]={0};  // массив коэффициентов

int length=Series->YValues->Count; // длина входного массива
int lengthOut = (maxX-minX)/hX+1; // кол-во возвращаемых значений
long double *inX=new long double [length];
long double *inY=new long double [length];
long double *x= new long double [lengthOut];

for(int i=0;i<lengthOut;i++)
{
x[i]=minX+hX*i;  // заполняем массив х
}

for (int i = 0; i < length; i++) {
	inX[i]=Series->XValues->Value[i];
	inY[i]=Series->YValues->Value[i];
}
out->Clear();
curveFitting2(inX,inY,0,length,koef);  // выполняем подгонку и получаем коэффициенты

int a=lengthCoef;
for (int i = 0; i <lengthOut; i++)
{                         // вычисляем экстраполированную функцию
out->AddXY(x[i], pow(x[i],a-1)*koef[0]+pow(x[i],a-2)*koef[1]+koef[2],"",clRed);
}

delete[] inX;    // прибираемся
delete[] inY;
delete[] x;

return lengthOut; // уходим.
}



//-----------curveFitting5----------------------------------------------------
void curveFitting5(long double * inX, long double *inY, long double * out, int length,long double *outKoef)
{
const int a=6;
// выражение f(x)=x^5*p1+x^4*p2+x^3*p3+x^2*p4+x*p5+p6
//long double * fullMatrix[a+1];
long double ** fullMatrix=new long double*[length];

for (int i = 0; i < length; i++) {
	fullMatrix[i]=new long double[a+1];

}

long double K[a+1][a+1]={0}; // сюда сохраним результат перемножения x'*x
long double *K5[a];
long double Ks[a]={0};

long double **delta[a];  // матрицы для вычисления корней, один из столбцов - сободные члены.

	for (int i = 0; i < a; i++) {
		 delta[i]=new long double*[a];
		for (int j = 0; j < a; j++) {
			delta[i][j]=new long double[a];

	}}
/*
формируем матрицу вида

-y x^0 x^1 x^2 x^3 x^4 x^5  7 столбцов и length строк

x^5 x^4 x^3 x^2 x^1 x^0 y
*/
for (int i = 0; i < length; i++) {
	for (int j = 0; j < a; j++) {
	// копируем значения в матрицу
	// по столбцам, шестой - столбец единиц
		fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

	}
	// седьмой формируется здесь.
	fullMatrix[i][a]=inY[i];
}

	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
	}}}
	// перемножение работает, проверено матлабом
	// K5 - содержит 6 строк и 6 столбцов, т.е. без свободных членов.

	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	// Ks - столблец из 6 строк, свободные члены.


	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
		K5[i][j]=K[i][j]; // копируем первые 5 строк
		//blabla=K5[i][j];
		}
	}

	// а 6ая строка - среднее арифметическое 6 и 7 строк
	for (int i = 0; i < a; i++) {
		K5[a-1][i]=(K[a-1][i]+K[a][i])/2;
	}

	for (int i = 0; i < a-1; i++) {
		Ks[i]=K[i][a];
	}
	Ks[a-1]=(K[a-1][a]+K[a][a])/2;

	long double d0=determinant(K5,a); // детерминант правилен +

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
		for (int k = 0; k < a; k++) {
		if (k!=i) {
		// составляем матрицы по которым будем находить детерминанты для переменных.
		 delta[i][j][k]=K5[j][k];
		}
		else
		{
		// столбец нужной переменной заменяется свободными членами.
		delta[i][j][k]=Ks[j];
		}
		//blabla=delta[i][j][k];
	}}}

	long double p[a]={0};

	for (int i = 0; i < a; i++) {
		outKoef[i]=p[i]=determinant(delta[i],a)/d0;
	}


	if(out!=0)  // если нас просят - вычисляем функцию
	{
	for (int i = 0; i <length; i++) {
		out[i]=pow(inX[i],a-1)*p[0]+pow(inX[i],a-2)*p[1]+pow(inX[i],a-3)*p[2]+pow(inX[i],a-4)*p[3]+pow(inX[i],1)*p[4]+p[5];
	}
    }


// прибираемся.
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
//-----------curveFitting2----------------------------------------------------
void curveFitting2(long double * inX, long double *inY, long double * out, int length,long double *outKoef)
{
const int a=3;
// выражение f(x)=x^2*p1+x*p2+p3

long double ** fullMatrix=new long double*[length];

for (int i = 0; i < length; i++) {
	fullMatrix[i]=new long double[a+1];

}

long double K[a+1][a+1]={0}; // сюда сохраним результат перемножения x'*x
long double *K5[a];
long double Ks[a]={0};

long double **delta[a];  // матрицы для вычисления корней, один из столбцов - сободные члены.

	for (int i = 0; i < a; i++) {
		 delta[i]=new long double*[a];
		for (int j = 0; j < a; j++) {
			delta[i][j]=new long double[a];

	}}
/*
формируем матрицу вида

x^2 x^1 x^0 y
*/
for (int i = 0; i < length; i++) {
	for (int j = 0; j < a; j++) {
	// копируем значения в матрицу
	// по столбцам, шестой - столбец единиц
		fullMatrix[i][j]=((a-j-1)==0?1:pow(inX[i],a-j-1));

	}
	// седьмой формируется здесь.
	fullMatrix[i][a]=inY[i];
}

	for (int i = 0; i <= a; i++) {
		for (int j = 0; j <= a; j++) {
			for (int k=0; k < length; k++) {
				K[i][j]+=fullMatrix[k][i]*fullMatrix[k][j];
	}}}
	// перемножение работает, проверено матлабом
	// K5 - содержит 3 строк и 3 столбцов, т.е. без свободных членов.

	for (int i = 0; i < a; i++)
		K5[i]= new long double[a];
	// Ks - столблец из 6 строк, свободные члены.


	for (int i = 0; i < a-1; i++) {
		for (int j = 0; j < a; j++) {
		K5[i][j]=K[i][j]; // копируем первые 5 строк
		//blabla=K5[i][j];
		}
	}

	// а 6ая строка - среднее арифметическое 6 и 7 строк
	for (int i = 0; i < a; i++) {
		K5[a-1][i]=(K[a-1][i]+K[a][i])/2;
	}

	for (int i = 0; i < a-1; i++) {
		Ks[i]=K[i][a];
	}
	Ks[a-1]=(K[a-1][a]+K[a][a])/2;

	long double d0=determinant(K5,a); // детерминант правилен +

	for (int i = 0; i < a; i++) {
		for (int j = 0; j < a; j++) {
		for (int k = 0; k < a; k++) {
		if (k!=i) {
		// составляем матрицы по которым будем находить детерминанты для переменных.
		 delta[i][j][k]=K5[j][k];
		}
		else
		{
		// столбец нужной переменной заменяется свободными членами.
		delta[i][j][k]=Ks[j];
		}
		//blabla=delta[i][j][k];
	}}}

	long double p[a]={0};

	for (int i = 0; i < a; i++) {
		outKoef[i]=p[i]=determinant(delta[i],a)/d0;
	}


	if(out!=0)  // если нас просят - вычисляем функцию
	{
	for (int i = 0; i <length; i++) {
		out[i]=pow(inX[i],a-1)*p[0]+pow(inX[i],a-2)*p[1]+p[2];
	}
    }


// прибираемся.
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



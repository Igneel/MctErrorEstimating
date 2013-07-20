//---------------------------------------------------------------------------

#ifndef NoiseUnitH
#define NoiseUnitH

#include <vcl.h>
#include <math.h>
long double Mo (long double *x,int l);
long double Sko (long double *x0,long double *x,int l);
void ShumAdding(long double *x,long double *out,long double *ret, long double koeff,const int l);
//---------------------------------------------------------------------------
#endif

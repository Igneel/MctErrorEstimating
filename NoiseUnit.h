//---------------------------------------------------------------------------

#ifndef NoiseUnitH
#define NoiseUnitH

#include <vcl.h>
#include <math.h>
long double Mo (const long double *x,int l);
long double Sko (const long double *x0,long double *x,int l);
void ShumAdding(const long double *x,long double *out,long double *ret, long double koeff,const int l);
//---------------------------------------------------------------------------
#endif

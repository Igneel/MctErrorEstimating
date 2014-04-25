//---------------------------------------------------------------------------

#ifndef NoiseUnitH
#define NoiseUnitH

#include <vcl.h>
#include <vector>
#include <math.h>
long double Mo (const long double *x,int l);
long double Mo (std::vector<long double> const &x,int l);
long double Sko (const long double *x0,const long double *x,int l);
long double Sko (std::vector<long double> const &x0,std::vector<long double> const &x,int l);
void ShumAdding(const long double *x,long double *out,long double *ret, long double koeff,const int l);
void ShumAdding(std::vector<long double> const &x,std::vector<long double> &out,std::vector<long double> &ret, long double koeff,const int l);
//---------------------------------------------------------------------------
#endif

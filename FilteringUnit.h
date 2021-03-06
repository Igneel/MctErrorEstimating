//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include <VCLTee.Series.hpp>
#include <math.h>

//#include <boost\math\tools\remez.hpp>

double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);
double TrForMassiveFilter(long double *inB,long double *inY,long double* outB,long double *outY,
int lengthMassive,int lengthFilter,double Fdisk, double Fpropysk,double Fzatyh);

//---------------------------------------------------------------------------
#endif

//---------------------------------------------------------------------------

#ifndef FilteringUnitH
#define FilteringUnitH

#include "Series.hpp"
#include <math.h>


double Tr_Filter(TLineSeries *inS,TLineSeries *outS,int length,double Fdisk, double Fpropysk,double Fzatyh);
//---------------------------------------------------------------------------
#endif

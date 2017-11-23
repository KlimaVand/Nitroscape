#ifndef PIGSTABLE_H
	#define PIGSTABLE_H

#include "../base/common.h"
#include "stable.h"
#include "pigstysection.h"
class swineStock;

class PigHousing :public stable
{



   private:
	   PigHousing(){};
public:
   PigHousing(char* aname,const int aIndex,const base* aOwner);
   PigHousing(const PigHousing& aStable);
   virtual ~PigHousing();

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);

   virtual void ReadDefaultParameters();

   virtual double CalcMinimumVentilationRate();
   virtual double CalcMaximumVentilationRate();
};

#endif

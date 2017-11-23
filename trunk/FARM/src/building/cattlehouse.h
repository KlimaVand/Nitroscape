/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/

#ifndef CATSTABLE_H
	#define CATSTABLE_H

#include "../base/common.h"
#include "stable.h"
class dairyStock;

class cattlehouse :public stable
{
	dairyStock* Livestock;

   public:
   cattlehouse(char* aname,const int aIndex,const base* aOwner);
   ~cattlehouse();
   cattlehouse(const cattlehouse& acattlehouse);
	string GetAnimalType();
   void SetLivestock(dairyStock* aLivestock);

   virtual void DailyUpdate();
   virtual void ReadParameters(fstream* file);
   virtual void ReadDefaultParameters();

};

#endif

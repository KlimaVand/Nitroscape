#ifndef PIGSTYSECTION_H
	#define PIGSTYSECTION_H

#include "../economy/genAsset.h"
//#include "PigStyTypes.h"
#include "animalSection.h"

class pigstysection: public animalSection
{



   int DaysInSection;


	public:
pigstysection(const pigstysection& s);
	pigstysection(const animalSection& s);
      pigstysection(char* aName,const int aIndex,const base* aOwner);
	   virtual ~pigstysection();

	public:



	//get functions

   double GetDaysInSection() {return DaysInSection;};
	double GetCapacity(double ProductionCyclus);

	virtual void ReadParameters(fstream* file, bool defaultValus);
	virtual void ReadDefaultParameters(fstream *file);

};

#endif

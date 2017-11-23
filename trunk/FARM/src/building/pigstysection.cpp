#include "../base/common.h"
#include "pigstysection.h"

pigstysection::pigstysection(char* aName,const int aIndex,const base* aOwner)
   	:animalSection(aName,aIndex,aOwner)
{

   DaysInSection     = 0;
}
pigstysection::pigstysection(const pigstysection& s):animalSection(s)
{
	DaysInSection=s.DaysInSection;
}
pigstysection::pigstysection(const animalSection& s):animalSection(s)
{

}
pigstysection::~pigstysection()
{
}

double pigstysection::GetCapacity(double ProductionCyclus)
{
	return Places*ProductionCyclus/DaysInSection;
}

/**
 * read user input file
 */
void pigstysection::ReadParameters(fstream *file, bool defaultValus)
{
	animalSection::ReadParameters(file, defaultValus);
	Setfile(file);
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
	namestr=(string) GetName()+Indexstr;
   FindSection(namestr);


   GetParameter("DaysInSection",&DaysInSection);
   Setfile(NULL);
}

void pigstysection::ReadDefaultParameters(fstream *file)
{
	animalSection::ReadDefaultParameters(file);
	Setfile(file);
	string namestr;
	char Indexstr[10];
	sprintf(Indexstr,"(%d)",Index);
	namestr=(string) GetName()+Indexstr;
	FindSection(namestr);
	GetParameter("DaysInSection",&DaysInSection);
	Setfile(NULL);
}

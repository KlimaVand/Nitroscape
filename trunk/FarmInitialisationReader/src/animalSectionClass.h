#ifndef HOUSINGSECTIONCLASS_H_
#define HOUSINGSECTIONCLASS_H_

#ifndef __BCplusplus__
using namespace std;
#endif
//!
#include "xmlParser.h"
#include "sectionRecordClass.h"
#include "base\linklist.h"

class animalSectionClass
{
	int sectionID;
	double surfaceArea;
	int floorType;
	int manureDestination;
	int manureType;
	int livestockType;
   linkList <sectionRecordClass> *theSectionRecords;

public:

	//constructor
	animalSectionClass ();
	~animalSectionClass( ){};

//	void setlivestockType(int aVal){livestockType=aVal;}
//	int getlivestockType(){return livestockType;}
	void setsurfaceArea(double aVal){surfaceArea = aVal;}
	double getsurfaceArea(){return surfaceArea;}
	int getmanureDestination(){return manureDestination;};
	int getsectionID(){return sectionID;}
	int getfloorType(){return floorType;}

	bool readAnimalSection(XMLNode *aNodeSection);
	int getLivestockType(){return livestockType;};
	int findLivestockCategory();
	void writeSectionToBuildingsFile(ofstream *outfile, string sectionName);
	int getLivestockName(int aVal, string *nameString);
};
#endif



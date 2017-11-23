#ifndef HOUSINGCLASS_H_
#define HOUSINGCLASS_H_

#include "xmlParser.h"
#include "base/linklist.h"
#include "animalSectionClass.h"

#ifndef __BCplusplus__
using namespace std;
#endif
//!

class animalHousingClass
{
	int type;
	double planArea;
	double height;
	int ID;
	int houseNumber;
	int ventilation;
	linkList <animalSectionClass> *theSections;

public:

	//constructor
//	animalHousingClass (int theHouseNumber){houseNumber=theHouseNumber;};
	animalHousingClass ();

	~animalHousingClass( );
	bool readHousing(XMLNode *aNodeHouses);
	linkList <animalSectionClass> * GettheSections() {return theSections;}
	int getID(){return ID;}
	int gettype(){return type;}
	int getHousingType();
	void writeHousingToBuildingsFile(ofstream *outfile, string houseName);
	void addPigSpecificSections(int manuretoID,string ManureToName,string houseName,int numberOfSection,ofstream *outfile, string contains);
	void CheckSections();
		void writeSection(ofstream *outfile, int i,animalSectionClass* aanimalSectionClass,string houseName,string nameString);
};
#endif



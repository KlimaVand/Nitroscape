
#include "animalHousingClass.h"
#include "animalSectionClass.h"
#include "base\linklist.h"
#include "base\message.h"

//!

animalHousingClass::animalHousingClass ()
{
	type=0;
	planArea=0.0;
	height=0.0;
	ID=0;
	houseNumber=0;
	theSections = new linkList <animalSectionClass>;
};

animalHousingClass:: ~ animalHousingClass()
{
	delete theSections;
};



bool animalHousingClass::readHousing(XMLNode *aNodeHouses)
{
	bool retVal=true;
	XMLNode xNode;

	xNode=aNodeHouses->getChildNode("lstkbuilding_id");
	if (xNode.getText())
		ID=atoi(xNode.getText());
	else
		retVal= false;
	xNode=aNodeHouses->getChildNode("buildingtypeid");
	if (xNode.getText())
		type=atoi(xNode.getText());
	else
	{
		type= 0;
		retVal=false;
	}
	xNode=aNodeHouses->getChildNode("buildingarea");
	if (xNode.getText())
		planArea = atof(xNode.getText());
	else
		planArea = 0.0;
	xNode=aNodeHouses->getChildNode("buildingheight");
	if (xNode.getText())
		height = atof(xNode.getText());
	else
		height = 0.0;
	xNode=aNodeHouses->getChildNode("buildingventilationtypeid");
	if (xNode.getText())
		ventilation = atof(xNode.getText());
	else
		ventilation = 0.0;

	int numsections =aNodeHouses->nChildNode("lstkbuildingsection");
	for (int j=0; j<numsections; j++)
	{
		XMLNode sectionNode=aNodeHouses->getChildNode("lstkbuildingsection",j);
		animalSectionClass *aanimalSectionClass = new animalSectionClass();
		if (!aanimalSectionClass->readAnimalSection(&sectionNode))
		{
			string outString ="Data missing in section ";
			char *achar;
			itoa(j,achar,100);
			string tempString = achar;
			outString.append(tempString);
			outString.append(" of housing ");
			itoa(ID,achar,100);
			tempString = achar;
			outString.append(tempString);
			theMessage->WarningWithDisplay(outString.c_str());
		}
		theSections->InsertLast(aanimalSectionClass);
	}
	return retVal;
}
int animalHousingClass::getHousingType()
{
	int housingCategory=0;
	for (int i=0; i<theSections->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		int housingType=aanimalSectionClass->findLivestockCategory();
		if ((housingCategory>0)&&(!housingCategory==housingType))
    	{
    		cout << "Warning - two species in one animal house" << endl;
            cout << "housingCategory " << housingCategory << " housingType " << housingType << endl;
    		housingCategory=-999;
  		}
    	else
    		housingCategory=housingType;
	}
	return housingCategory;
}

void animalHousingClass::writeHousingToBuildingsFile(ofstream *outfile, string houseName)
{
	*outfile << "ID" << "\t"	<< ID << endl;
	*outfile << "Area" << "\t"	<< planArea << endl;
	*outfile << "height" << "\t"	<< height << endl;
	if (ventilation==1)
		*outfile << "controlledVent" << "\t" << "0" << endl;
	else
		*outfile << "controlledVent" << "\t" << "1" << endl;
	string sectionName;
	char num[3];
	for (int i=0; i<theSections->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		sectionName.assign("[");
		sectionName.append(houseName);
		itoa(i, num, 10);
		sectionName.append(".Section(");
		sectionName.append(num);
		sectionName.append(")]");
		*outfile << sectionName << endl;
		aanimalSectionClass->writeSectionToBuildingsFile(outfile,sectionName);
	}
}

#include "animalSectionClass.h"
#include "xmlParser.h"
#include "base\message.h"
#include "manureStorageClass.h"


animalSectionClass::animalSectionClass ()
{
	sectionID = 0;
	surfaceArea=0.0;
	floorType=0;
	manureDestination=0;
	manureType=0;
	livestockType=0;
	theSectionRecords = new linkList <sectionRecordClass>;
};

bool animalSectionClass::readAnimalSection(XMLNode *sectionNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=sectionNode->getChildNode("surface");
	if (xNode.getText())
		surfaceArea = atof(xNode.getText());
	else
		surfaceArea=0.0;
	xNode=sectionNode->getChildNode("lstkflooringtypeid");
	if (xNode.getText())
		floorType = atoi(xNode.getText());
	else
	{
		floorType=0;
		retVal=false;
	}
	xNode=sectionNode->getChildNode("lstkmanuretypeid");
	if (xNode.getText())
		manureType = atoi(xNode.getText());
	else
		manureType=0;
	xNode=sectionNode->getChildNode("manuredestination");
	if (xNode.getText())
		manureDestination = atoi(xNode.getText());
	else
		if (manureType==5)
			manureDestination=-1;
		else
			manureDestination=0;
	xNode=sectionNode->getChildNode("sectionid");
	if (xNode.getText())
		sectionID = atoi(xNode.getText());
	else
	{
		sectionID=0;
		retVal=false;
	}
	if (retVal)
   {
      int numSectionRecords=sectionNode->nChildNode("lstkhousingrecord");
      for (int i=0;i<numSectionRecords; i++)
      {
         XMLNode sectionRecordNode =sectionNode->getChildNode("lstkhousingrecord",i);
         sectionRecordClass * asectionRecordClass = new sectionRecordClass();
         retVal=asectionRecordClass->readsectionRecord(&sectionRecordNode);
         if (retVal)
         {
            theSectionRecords->InsertLast(asectionRecordClass);
            //findLivestockType();
         }
      }
   }
	return retVal;
}

/*
 * livestockCategory = 2 (cattle), 1 (pigs), 2
 */
int animalSectionClass::findLivestockCategory()
{
	int livestockCategory=0;
    for (int i=0;i<theSectionRecords->NumOfNodes(); i++)
    {
        sectionRecordClass * asectionRecordClass = theSectionRecords->ElementAtNumber(i);
        livestockType=asectionRecordClass->getlivestockType();
    	if (livestockType<= 12)
    		if (livestockCategory==2)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=1;
    	if ((livestockType>=13)&&(livestockType<=15))
    		if (livestockCategory==1)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=2;
    	if ((livestockType>=27)&&(livestockType<=33))
    		if (!livestockCategory==3)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=3;
    	if ((livestockType>=16)&&(livestockType<=26))
    		if (!livestockCategory==4)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=4;
    	if ((livestockType>=34)&&(livestockType<=36))
    		if (!livestockCategory==5)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=5;
    }
	return livestockCategory;
}

void animalSectionClass::writeSectionToBuildingsFile(ofstream *outfile, string sectionName)
{
	string nameString;
	if (getLivestockName(livestockType,&nameString)==0)
		*outfile << "Contains" << "\t"	<< nameString  << endl;
	else
		theMessage->WarningWithDisplay("Section contains no livestock");
	string floorname;
	floorname.assign(sectionName);
	floorname.append(".floor(0)]");
	*outfile << floorname << endl;

	*outfile << "Area" << "\t"	<< getsurfaceArea() << endl;
	*outfile << "ManureToID" << "\t"	<< getmanureDestination()<< endl;
	*outfile << "ProportionFouled" << "\t"	<< "1.0"  << endl;
	*outfile << "ExcretaPartition" << "\t"	<< "1.0"  << endl;
	//*outfile << "ManureType" << "\t"	<< manureType  << endl;
	//*outfile << "LivestockType" << "\t"	<< livestockType  << endl;
	nameString.clear();
	manureStorageClass *aclass = new manureStorageClass();
	if (aclass->getManureName(manureType,&nameString)==0)
		*outfile << "ManureType" << "\t" << nameString  << endl;
	else
		theMessage->WarningWithDisplay("Manure type missing from section");
	delete aclass;
}

int animalSectionClass::getLivestockName(int aVal, string *nameString)
{
	/* THIS IS INCORRECT!!!
	 * 0 = dairy cow, large, 1 = dairy cow small, 2 = dairy heifer >1 yr, 3 = dairy calf, female, 0.5-1 yr, 4 = dairy calf, female, 0-0.5 yr
	 * 5 = dairy calves, 6 = suckler cow, 7 = beef, female, >1 yr, 7 = beef, female, 0.5-1yr, 8 = beef, female, 0-0.5 yr, 9 = beef male, >1 yr, 10 = beef male, 0.5-1yr
	 * 11 = beef, male 0-0.5 yr
	 * 	 = breeding sow + suckling piglets, 1 = piglets, 2 = finishers, 3 = replacement sows
	 *
	 */

	int retVal=0;
	switch (aVal) {
	case 0: nameString->assign("DAIRY");
	break;
	case 1: nameString->assign("DAIRY");
	break;
	case 2: nameString->assign("HEIFERS3");
	break;
	case 3: nameString->assign("HEIFERS2");
	break;
	case 4: nameString->assign("HEIFERS1");
	break;
	case 5: nameString->assign("CALVES");
	break;
	case 6: nameString->assign("SUCKLER");
	break;
	case 7: nameString->assign("FBEEF3");
	break;
	case 8: nameString->assign("FBEEF2");
	break;
	case 9: nameString->assign("FBEEF1");
	break;
	case 10: nameString->assign("MBEEF3");
	break;
	case 11: nameString->assign("MBEEF2");
	break;
	case 12: nameString->assign("MBEEF1");
	break;
	case 13: nameString->assign("SOWS");
	break;
	case 14: nameString->assign("PIGLETS");
	break;
	case 15: nameString->assign("BACONERS");
	break;
	case 16: nameString->assign("NEWSOWS");
	break;
	default:
	{
		theMessage->WarningWithDisplay("Unknown livestock category");
		retVal=2;
	}
	}
	return retVal;
}

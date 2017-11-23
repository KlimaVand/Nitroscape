/**
 * Class for <lstkbuildingsection>
 *
 * Contains list of sectionRecordClass for the nodes <lstkhousingrecord>.
 */
#include "animalSectionClass.h"
#include "xmlParser.h"
#include "base/message.h"
#include "manureStorageClass.h"
#include "tools/convertData.h"

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

animalSectionClass::animalSectionClass (animalSectionClass *ananimalSectionClass, int thelivestockType)
{
	sectionID = ananimalSectionClass->getsectionID();
	surfaceArea=ananimalSectionClass->getsurfaceArea();
	floorType=ananimalSectionClass->getfloorType();
	manureDestination=ananimalSectionClass->getmanureDestination();
	manureType=ananimalSectionClass->getmanureType();
	livestockType=thelivestockType;
	theSectionRecords = new linkList <sectionRecordClass>;
	for (int i=0; i<ananimalSectionClass->getTheSectionRecords()->NumOfNodes(); i++)
	{
		sectionRecordClass *asectionRecordClass = ananimalSectionClass->getTheSectionRecords()->ElementAtNumber(i);
		if (asectionRecordClass->getlivestockType()==thelivestockType)
		{
			sectionRecordClass * anewsectionRecordClass = new sectionRecordClass(*asectionRecordClass);
			theSectionRecords->InsertLast(anewsectionRecordClass);
		}
	}
};



bool animalSectionClass::readAnimalSection(XMLNode *sectionNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=sectionNode->getChildNode("sectionid");
	if (xNode.getText())
		sectionID = atoi(xNode.getText());
	else
	{
		sectionID=0;
		retVal=false;
	}
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
	if (manureType==0)
	{
		string outString ="Manure type missing in section with ID ";
		char achar[20];
		itoa(sectionID,achar,20);
		string tempString;
		tempString = achar;
		outString.append(tempString);
		theMessage->WarningWithDisplay(outString.c_str());
	}
	xNode=sectionNode->getChildNode("manuredestination");
	if (xNode.getText())
		manureDestination = atoi(xNode.getText());
	else
		if (manureType==5)
			manureDestination=-1;
		else
			manureDestination=0;
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
	    if (livestockType == 0) {
		livestockType = asectionRecordClass->getlivestockType();
	    }
         } else {
		cout << log_warning << "animal section #" << sectionID << ", record #" << i << " failed to parse XML!" << endl;
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
        int theLivestockType=asectionRecordClass->getlivestockType();
    	if (theLivestockType<= 12) {
    		if (livestockCategory==2)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=1;
	}
    	if ((theLivestockType>=13)&&(theLivestockType<=15)) {
    		if (livestockCategory==1)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=2;
	}
		if (theLivestockType==27) {
    		if (!livestockCategory==2)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=2;
		}
    	if ((theLivestockType>=28)&&(theLivestockType<=33)) {
    		if (!livestockCategory==3)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=3;
	}
    	if ((theLivestockType>=16)&&(theLivestockType<=26)) {
    		if (!livestockCategory==4)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=4;
	}
    	if ((theLivestockType>=34)&&(theLivestockType<=36)) {
    		if (!livestockCategory==5)
    		{
    			cout << "Warning - two species in one animal section" << endl;
    			livestockCategory=-999;
    		}
    		else
    			livestockCategory=5;
	}
    }
	return livestockCategory;
}

int animalSectionClass::writeSectionToBuildingsFile(string ManureToName,ofstream *outfile, string sectionName, string contains)
{
	string nameString;
	int retVal=0;
	*outfile << "Alias\tDefault" << endl; //TODO
	*outfile << "Contains" << "\t"	<< contains  << endl;

	string floorname;
	floorname.assign(sectionName);
	floorname.append(".floor(0)]");
	*outfile << floorname << endl;
	*outfile << "Alias" << "\t"	<< "Default"  << endl;
	*outfile << "Area" << "\t"	<< getsurfaceArea() << endl;
	*outfile << "ManureToID" << "\t"	<< getmanureDestination()<< endl;
	*outfile << "ProportionFouled" << "\t"	<< "1.0"  << endl;
	*outfile << "ExcretaPartition" << "\t"	<< "1.0"  << endl;
	*outfile << "ManureToName" << "\t"	<< ManureToName << endl;
	convertData conv;

	//*outfile << "LivestockType" << "\t"	<< livestockType  << endl;
	nameString.clear();
	if (getManureName(&nameString)==0)
		*outfile << "ManureType" << "\t" << nameString  << endl;
	else {
		theMessage->WarningWithDisplay("Manure type missing from section");
		retVal = 2;
	}
	return retVal;
}

int animalSectionClass::getLivestockName(string *nameString)
{
	int retVal=0;
	/*
	 * Generated from the database using
	 * SELECT 'case ' || lstktypeid || ': nameString->assign("' || fasset_type || '"); break;' FROM f_lstktype_lut ORDER BY lstktypeid;
	 *
	 * TODO: use the XML with description > lstktype > fasset_type
	 */
	switch (livestockType) {
	case 1: nameString->assign("DAIRY"); break;
	case 2: nameString->assign("DAIRY"); break;
	case 3: nameString->assign("HEIFERS3"); break;
	case 4: nameString->assign("HEIFERS2"); break;
	case 5: nameString->assign("HEIFERS1"); break;
	case 6: nameString->assign("SUCKLER"); break;
	case 7: nameString->assign("FBEEF1"); break;
	case 8: nameString->assign("FBEEF2"); break;
	case 9: nameString->assign("FBEEF3"); break;
	case 10: nameString->assign("MBEEF1"); break;
	case 11: nameString->assign("MBEEF2"); break;
	case 12: nameString->assign("MBEEF3"); break;
	case 13: nameString->assign("SOWS"); break;
	case 14: nameString->assign("Weaner"); break;
	case 15: nameString->assign("Finisher(0)"); break;
	case 16: nameString->assign("NEWSOWS"); break;
	case 17: nameString->assign("DAIRY"); break;
	case 18: nameString->assign("FBEEF1"); break;
	case 19: nameString->assign("HEIFERS3"); break;
	case 20: nameString->assign("FBEEF1"); break;
	case 21: nameString->assign("FBEEF1"); break;
	case 22: nameString->assign("FBEEF2"); break;
	case 23: nameString->assign("FBEEF1"); break;
	case 24: nameString->assign("DAIRY"); break;
	case 25: nameString->assign("DAIRY"); break;
	case 26: nameString->assign("DAIRY"); break;
	case 27: nameString->assign("PIGLETS"); break;
	case 28: nameString->assign("PIGLETS"); break;
	case 29: nameString->assign("PIGLETS"); break;
	case 30: nameString->assign("PIGLETS"); break;
	case 31: nameString->assign("PIGLETS"); break;
	case 32: nameString->assign("PIGLETS"); break;
	case 33: nameString->assign("PIGLETS"); break;
	case 34: nameString->assign("DAIRY"); break;
	case 35: nameString->assign("PIGLETS"); break;
	case 36: nameString->assign("DAIRY"); break;
	default:
		theMessage->WarningWithDisplay("For the animal section, unknown livestock type name: ", livestockType);
		retVal=2;
	}
	return retVal;
}

int animalSectionClass::getManureName(string *nameString)
{
	int retVal=0;
	/*
	 * Generated from the database using
	 * SELECT 'case ' || manuretypeid || ': return "' || fasset_type || '"; break;' FROM f_manuretype_lut ORDER BY manuretypeid;
	 *
	 * TODO: use the XML with description > manuretype > fasset_type
	 */
	switch (manureType) {
	case 1: nameString->assign("CATTLE-SLURRY"); break;
	case 2: nameString->assign("SLURRY"); break;
	case 3: nameString->assign("SLURRY"); break;
	case 4: nameString->assign("SLURRY"); break;
	case 5: nameString->assign("CATTLE-FYM"); break;
	case 6: nameString->assign("PIG-FYM"); break;
	case 7: nameString->assign("FYM"); break;
	case 8: nameString->assign("FYM"); break;
	case 9: nameString->assign("FYM"); break;
	case 10: nameString->assign("SOLID-MANURE"); break;
	case 11: nameString->assign("FLUID-MANURE"); break;
	case 12: nameString->assign("FLUID-MANURE"); break;
	case 13: nameString->assign("FLUID-MANURE"); break;
	case 14: nameString->assign("SOLID-MANURE"); break;
	case 15: nameString->assign("SOLID-MANURE"); break;
	case 16: nameString->assign("SOLID-MANURE"); break;
	case 17: nameString->assign("SOLID-MANURE"); break;
	case 18: nameString->assign("SLURRY"); break;
	case 19: nameString->assign("PIG-SLURRY-DEGAS"); break;
	case 20: nameString->assign("SLURRY"); break;
	case 21: nameString->assign("SOLID-MANURE"); break;
	case 22: nameString->assign("SOLID-MANURE"); break;
	case 23: nameString->assign("FYM"); break;
	case 24: nameString->assign("FYM"); break;
	case 26: nameString->assign("FYM"); break;
	default:
		theMessage->WarningWithDisplay("For the animal section, unknown manure category: ", manureType);
		retVal=2;
	}
	return retVal;
}

bool animalSectionClass::isPig()
{
	/*
	 * Generated from the database using
	 * select 'case ' || lstktypeid || ': return ' || case when fasset_category = 'pig' then 'true' else 'false' end || '; break;' from f_lstktype_lut order by lstktypeid;
	 *
	 * TODO: use the XML with description > lstktype > fasset_type
	 */
	switch (livestockType) {
	case 1: return false; break;
	case 2: return false; break;
	case 3: return false; break;
	case 4: return false; break;
	case 5: return false; break;
	case 6: return false; break;
	case 7: return false; break;
	case 8: return false; break;
	case 9: return false; break;
	case 10: return false; break;
	case 11: return false; break;
	case 12: return false; break;
	case 13: return true; break;
	case 14: return true; break;
	case 15: return true; break;
	case 16: return true; break;
	case 17: return false; break;
	case 18: return false; break;
	case 19: return false; break;
	case 20: return false; break;
	case 21: return false; break;
	case 22: return false; break;
	case 23: return false; break;
	case 24: return false; break;
	case 25: return false; break;
	case 26: return false; break;
	case 27: return true; break;
	case 28: return true; break;
	case 29: return true; break;
	case 30: return true; break;
	case 31: return true; break;
	case 32: return true; break;
	case 33: return true; break;
	case 34: return false; break;
	case 35: return true; break;
	case 36: return false; break;
	default:
		theMessage->WarningWithDisplay("For the animal section, unknown livestock category: ", livestockType);
		return false;
	}
}

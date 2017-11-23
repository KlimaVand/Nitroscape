#include "manureStorageClass.h"
#include "xmlParser.h"
#include "base/common.h"
#include "base/message.h"


manureStorageClass::manureStorageClass ()
{
	manurestoreid = 0;
	type = 0;
	surfaceArea=0.0;
	manureType=0;
	storeType=0;
};

bool manureStorageClass::readManureStorage(XMLNode *aNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=aNode->getChildNode("manurestoreid");
	if (xNode.getText())
		manurestoreid = atoi(xNode.getText());
	else
	{
		theMessage->WarningWithDisplay("Manurestoreid missing");
		manurestoreid=0;
		retVal=false;
	}
	xNode=aNode->getChildNode("manuresurfacearea");
	if (xNode.getText())
	{
		surfaceArea = atof(xNode.getText());
		if (surfaceArea==0.0)
		{
			theMessage->WarningWithDisplay("manuresurfacearea == 0 for manure store with ID ", manurestoreid);
			retVal=false;
		}
	}
	else
	{
		surfaceArea=0.0;
		string outString ="manuresurfacearea missing for manure store with ID ";
		char achar[50];
		itoa(manurestoreid,achar,50);
		string tempString;
		tempString = achar;
		outString.append(tempString);
		theMessage->WarningWithDisplay(outString.c_str());
		retVal=false;
	}
	xNode=aNode->getChildNode("manuretypeid");
	if (xNode.getText())
		manureType = atoi(xNode.getText());
	else
	{
		manureType=0;
		theMessage->WarningWithDisplay("manuretypeid missing");
		retVal=false;
	}
	xNode=aNode->getChildNode("manurestoretypeid");
	if (xNode.getText())
		storeType = atoi(xNode.getText());
	else
	{
		theMessage->WarningWithDisplay("manurestoretypeid missing");
		storeType=0;
		retVal=false;
	}
	return retVal;
}

void manureStorageClass::printContents()
{
	cout << " manurestoreid " << manurestoreid;
	cout << " type " << type;
	cout << " surfaceArea " << surfaceArea;
	cout << " manureType " << manureType;
   cout << endl;
};

void manureStorageClass::writeStorageToBuildingsFile(ofstream *outfile)
{
	*outfile << "Area" << "\t"	<< surfaceArea << endl;
	*outfile << "ManureStoreID" << "\t"	<< manurestoreid << endl;
	string nameString;
	if (getManureName(&nameString)==0)
	*outfile << "ManureType" << "\t"	<< nameString << endl;
	else
		theMessage->WarningWithDisplay("Manure type missing from storage");
}

/**
 * Same code as animalSectionClass::getManureName.
 *
 * @todo factorize!
 */
int manureStorageClass::getManureName(string *nameString)
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
		theMessage->WarningWithDisplay("For the manure storate, unknown manure category: ", manureType);
		retVal=2;
	}
	return retVal;
}

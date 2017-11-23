#include "manureStorageClass.h"
#include "xmlParser.h"
#include "base\common.h"
#include "base\message.h"


manureStorageClass::manureStorageClass ()
{
	manurestoreid = 0;
	type = 0;
	surfaceArea=0.0;
	manureType=0;
};

bool manureStorageClass::readManureStorage(XMLNode *aNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=aNode->getChildNode("manuresurfacearea");
	if (xNode.getText())
		surfaceArea = atof(xNode.getText());
	else
	{
		surfaceArea==0.0;
		theMessage->WarningWithDisplay("manuresurfacearea missing");
		retVal=false;
	}
	xNode=aNode->getChildNode("manurestoreid");
	if (xNode.getText())
		manurestoreid = atoi(xNode.getText());
	else
	{
		theMessage->WarningWithDisplay("manurestoreid missing");
		manurestoreid=0;
		retVal=false;
	}
	xNode=aNode->getChildNode("manurestoretypeid");
	if (xNode.getText())
		type = atoi(xNode.getText());
	else
	{
		type = 0;
		theMessage->WarningWithDisplay("manurestoretypeid missing");
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
	if (getManureName(manureType,&nameString)==0)
	*outfile << "ManureType" << "\t"	<< nameString << endl;
	else
		theMessage->WarningWithDisplay("Manure type missing from storage");
}

int manureStorageClass::getManureName(int aVal, string *nameString)
{

	int retVal=0;
	switch (aVal) {
	case 0:
	case 1:
	case 2:
	case 3:
	case 4:nameString->assign("SLURRY");
	break;
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:nameString->assign("FYM");
	break;
	case 11:
	case 12:
	case 13:nameString->assign("SLURRY");
	break;
	case 14:
	case 15:
	case 16:
	case 17:nameString->assign("FYM");
	break;
	case 18:
	case 19:
	case 20:nameString->assign("SLURRY");
	break;
	default:
	{
		theMessage->WarningWithDisplay("Unknown livestock category");
		retVal=2;
	}
	}
	return retVal;
}

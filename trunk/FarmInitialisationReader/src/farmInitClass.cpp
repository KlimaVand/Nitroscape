#include "farmInitClass.h"
#include "base\common.h"
#include "base\message.h"
#include "animalHousingClass.h"
#include "animalSectionClass.h"
#include "manureStorageClass.h"
#include <cstring>
#include "fieldOpSow.h"

extern bool verbosity;

farmInitClass::farmInitClass()
{
	theFields = new linkList<cropSeriesClass>;
	theHousing = new linkList<animalHousingClass>;
	theManureStorage = new linkList <manureStorageClass>;
	//theCropOps = new linkList<fieldOp>;
}

farmInitClass::~farmInitClass()
{
	delete theFields;
	delete theHousing;
	delete theManureStorage;
	//delete theCropOps;
}
bool farmInitClass::initFarm(XMLNode *aNodeFarm)
{
	bool retVal=true;
	xNodeFarm = aNodeFarm;
	
	XMLNode xNode=aNodeFarm->getChildNode("farmid",0);
	if (xNode.getText())
		farmID= atoi(xNode.getText());
	else
	{
		farmID=0;
		retVal=false;
	}
	int numFields = xNodeFarm->nChildNode("field");
	int fieldID=0;
	for (int i=0; i<numFields; i++)
	{
		XMLNode xFieldNode=xNodeFarm->getChildNode("field",i);
		xNode = xFieldNode.getChildNode("fieldid");
		if (xNode.getText())
			fieldID=atoi(xNode.getText());
		else
			theMessage->WarningWithDisplay("Field identifier missing from field number ",i);
		cropSeriesClass *acropSeriesClass = new cropSeriesClass(fieldID);
		theFields->InsertLast(acropSeriesClass);
	}
	int numHouses = xNodeFarm->nChildNode("lstkbuilding");
	for (int i=0; i<numHouses; i++)
	{
		animalHousingClass *aanimalHousingClass = new animalHousingClass();
		theHousing->InsertLast(aanimalHousingClass);
	}
	int numManureStores = xNodeFarm->nChildNode("manurestore");
	for (int i=0; i<numManureStores; i++)
	{
		manureStorageClass *amanureStorageClass = new manureStorageClass();
		theManureStorage->InsertLast(amanureStorageClass);
	}
	retVal=initBuildings();
	return retVal;
}
bool farmInitClass::writeFarmdat(bool pig, bool dairy, bool beef, bool poultry, bool isWithoutFields)
{
	bool retVal=true;
	ofstream farmfile;
	farmfile.open("farm.dat");
#ifdef __BCplusplus__
	if (farmfile)
#else
	if (farmfile.is_open())
#endif
	{
		farmfile << "[farm]" << endl;

		farmfile << "ScaleBuildings" << "\t" << "1" << endl;
		farmfile << "SellAllStraw" << "\t" << "1" << endl;
		farmfile << "Watering" << "\t" << "0" << endl;
		farmfile << "FarmID	" << "\t" << "0" << endl;
		farmfile << "[livestock]" << endl;
		if (pig)
			farmfile << "pigfarm" << "\t" << "1" << endl;
		else
			farmfile << "pigfarm" << "\t" << "0" << endl;
		if (beef)
			farmfile << "beeffarm" << "\t" << "1" << endl;
		else
			farmfile << "beeffarm" << "\t" << "0" << endl;
		if (dairy)
			farmfile << "dairyfarm" << "\t" << "1" << endl;
		else
			farmfile << "dairyfarm" << "\t" << "0" << endl;
		if (poultry)
			farmfile << "poultryfarm" << "\t" << "1" << endl;
		else
			farmfile << "poultryfarm" << "\t" << "0" << endl;
		if (isWithoutFields)
			farmfile << "WithoutFields" << "\t" << "1" << endl;
		else
			farmfile << "WithoutFields" << "\t" << "0" << endl;


		farmfile << "[end]" << endl;
		farmfile.close();
	}
	else
		retVal=false;
	return retVal;
}

bool farmInitClass::writeFielddat()
{
	bool retVal=true;
	fielddat.open("fields.dat");
#ifdef __BCplusplus__
	if (fielddat)
#else
	if (fielddat.is_open())
#endif
	{
		int numFields=theFields->NumOfNodes();
		for (int i=0;i<numFields;i++)
		  {
			XMLNode xNodeFields=xNodeFarm->getChildNode("field",i);
			XMLNode xNode=xNodeFields.getChildNode("fieldarea");
			int soiltype=4;
			double fieldarea;
			if (xNode.getText())
				fieldarea = atof(xNode.getText());
			else
			{
				retVal=false;
				string outString ="Data missing for field ";
				char *achar;
				itoa(i,achar,100);
				string tempString = achar;
				outString.append(tempString);
				itoa(farmID,achar,100);
				tempString = achar;
				outString.append(tempString);
				theMessage->WarningWithDisplay(outString.c_str());
			}
			fielddat << "[field(" << i << ")]" << endl;
			fielddat << "area" << "\t" << fieldarea << endl;
			soiltype = 4;
			fielddat << "JBNO" << "\t" << soiltype << endl;
			fielddat << "fertility	H" << endl;
			fielddat << "CROP0	F1" << endl;
			fielddat << "CROP1	B5" << endl;
			fielddat << "CROP2	W1" << endl;
			fielddat << "CROP3	BE" << endl;
			fielddat << "HUG-N0	0" << endl;
			fielddat << "HUG-N1	0" << endl;
			fielddat << "HUG-N2	0" << endl;
			fielddat << "HUG-N3	0" << endl;
		  }
		fielddat << "[end]" << endl;
		fielddat.close();
	}
	else
		retVal=false;
	return retVal;
}

void farmInitClass::initFieldsfnn(int number)
{
	char yearNum[2];
	sprintf(yearNum,"%d",number);
/*	char filename[20];
	strcpy(filename,"fields.f");
	if (number<10)
		strcat(filename,"0");
	strcat(filename,yearNum);*/
	string filename ="fields.f";
	if (number<10)
		filename.append("0");
	filename.append(yearNum);
	char tempPath[256];
	getcwd(tempPath, 256);
	if (verbosity)
		cout << "writing " << tempPath << " " << filename << endl;
	fieldsfnn = new ofstream();
	fieldsfnn->open(filename.c_str(),ios::out);
	
#ifdef __BCplusplus__
	if (fieldsfnn)
#else
	if (fieldsfnn->is_open())
#endif
	{
		getcwd(tempPath, 256);
		if (verbosity)
			cout << "writing " << tempPath << " " << filename << endl;
	}
	else
		cout << "Error opening " << " " << filename << endl;
}

void farmInitClass::closeFieldsfnn()
{
	*fieldsfnn << "[end]" << endl;
	fieldsfnn->close();
	char tempPath[256];
	getcwd(tempPath, 256);
	if (verbosity)
		cout << "Closing in path " << tempPath << endl;
}

bool farmInitClass::writeFieldsfnn(int startYear, int endYear)
{
	bool retVal=true;
	int number=0;
	for (int year=startYear; year<endYear+1;year++)
	{
		initFieldsfnn(number);
		int numFields=theFields->NumOfNodes();
		for (int i=0;i<numFields;i++)
		{
			char fieldNum[2];
			sprintf(fieldNum,"%d",i);
			*fieldsfnn << "[fieldOrder(" << fieldNum << ")]" << endl;
			*fieldsfnn << "Automatic	0" << endl;
			cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
			if (acropSeriesClass->getisGood())
			{
				*fieldsfnn << "Ha" << "\t" << acropSeriesClass->getfieldArea()<< endl;
				*fieldsfnn << "CROP" << "\t" << acropSeriesClass->getCropName((year-startYear)) << "____";
				if (i<10)
					*fieldsfnn << "0" << i << endl;
				else
					*fieldsfnn << i << endl;
			}
			else
			{
				char aChar1[20];
				itoa(farmID,aChar1,19);
				char aChar2[4];
				itoa(i,aChar2,3);
				strcat(aChar1,", field ");
				theMessage->Warning("Invalid or missing field information in farm ID ", aChar1, aChar2);
			}
			if (retVal)
				retVal=acropSeriesClass->OutputOpsTofnn(fieldsfnn,year);
			*fieldsfnn << endl;
			if (verbosity)
				cout << endl;
		}
		closeFieldsfnn();
		number++;
	}
	//	return retVal;
	return true;
}

bool farmInitClass::readFieldOps(int startYear, int endYear)
{
	bool retVal=true;
	int numFields=theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	{
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
		XMLNode xNodeField=xNodeFarm->getChildNode("field",i);
		if (!xNodeField.isEmpty())
		{
			XMLNode xNode = xNodeField.getChildNode("fieldarea");
			if (xNode.isEmpty())
			{
				cout << "Cannot find field area" << endl;
				retVal=false;
				acropSeriesClass->setisGood(false);
			}
			else
			{
				  string areaStr(xNode.getText());
				  double fieldArea= atof(areaStr.c_str());
				  acropSeriesClass->setfieldArea(fieldArea);
			}
		}
		else
		{
			cout << "Cannot find field node " << endl;
			retVal=false;
		}
		if (retVal)
			retVal=acropSeriesClass->addOperations(&xNodeField,startYear, endYear);
	}
	return retVal;
};

bool farmInitClass::writeFieldOps(bool appendit)
{
	bool retVal=true;
	fstream outfile;
	if (appendit)
		outfile.open("outfile.txt",fstream::out | fstream::app);
	else
		outfile.open("outfile.txt",fstream::out);
#ifdef __BCplusplus__
	if (outfile)
#else
	if (outfile.is_open())
#endif
	{
		char dum[200];
		if (verbosity)
			cout << "writing to directory " << getcwd(dum,200) << endl;
		int numFields=theFields->NumOfNodes();
		for (int i=0;i<numFields;i++)
		{
			cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
			acropSeriesClass->OutputOps(&outfile,farmID);
		}
		outfile.close();
	}
	else
		retVal=false;
	return retVal;
};

bool farmInitClass::writeCattlefnn(float cattleArr[], int numCattleCats)
{
	bool ret_val=true;
	string filename="cattle.fnn";
	char tempPath[256];
	getcwd(tempPath, 256);
	if (verbosity)
		cout << "Writing to " << tempPath << " " << filename << endl;
	cattlefnn = new ofstream();
	cattlefnn->open(filename.c_str(),ios::out);

	/*
	 * 0 = dairy cow, large, 1 = dairy cow small, 2 = dairy heifer >1 yr, 3 = dairy calf, female, 0.5-1 yr, 4 = dairy calf, female, 0-0.5 yr
	 * 5 = suckler cow, 6 = beef, female, >1 yr, 7 = beef, female, 0.5-1yr, 8 = beef, female, 0-0.5 yr, 9 = beef male, >1 yr, 10 = beef male, 0.5-1yr
	 * 11 = beef, male 0-0.5 yr
	 */
#ifdef __BCplusplus__
	if (cattlefnn)
#else
	if (cattlefnn->is_open())
#endif
	{
		for (int i=0; i<numCattleCats; i++)
		{
			switch (i) {
			case 0: *cattlefnn << "DAIRY" << "\t" << cattleArr[i] << endl;
				break;
			case 1: *cattlefnn << "DAIRY" << "\t" << cattleArr[i] << endl;
				break;
			case 2: *cattlefnn << "HEIFERS1" << "\t" << cattleArr[i] << endl;
				break;
			case 3: *cattlefnn << "HEIFERS2" << "\t" << cattleArr[i] << endl;
				break;
			case 4: *cattlefnn << "HEIFERS3" << "\t" << cattleArr[i] << endl;
				break;
			case 5: *cattlefnn << "CALVES" << "\t" << cattleArr[i] << endl;
				break;
			case 6: *cattlefnn << "SUCKLER" << "\t" << cattleArr[i] << endl;
				break;
			case 7: *cattlefnn << "FBEEF3" << "\t" << cattleArr[i] << endl;
				break;
			case 8: *cattlefnn << "FBEEF2" << "\t" << cattleArr[i] << endl;
				break;
			case 9: *cattlefnn << "FBEEF1" << "\t" << cattleArr[i] << endl;
				break;
			case 10: *cattlefnn << "MBEEF3" << "\t" << cattleArr[i] << endl;
				break;
			case 11: *cattlefnn << "MBEEF2" << "\t" << cattleArr[i] << endl;
				break;
			case 12: *cattlefnn << "MBEEF4" << "\t" << cattleArr[i] << endl;
				break;
			default: cout << "unknown cattle class" << endl;
			ret_val=2;
			}
		}
		*cattlefnn << "[end]" << endl;
		cattlefnn->close();
		char tempPath[256];
		getcwd(tempPath, 256);
		if (verbosity)
			cout << "Closing file " << tempPath << " " << filename << endl;
	}
	else
	{
		cout << "Error opening " << " " << filename << endl;
		ret_val=false;
	}
	return ret_val;
}
bool farmInitClass::writePigsfnn(float pigArr[], int numPigCats)
{
	bool ret_val=true;
	string filename="pigs.fnn";
	char tempPath[256];
	getcwd(tempPath, 256);
	if (verbosity)
		cout << "Writing to " << tempPath << " " << filename << endl;
	pigsfnn = new ofstream();
	pigsfnn->open(filename.c_str(),ios::out);

	/*
	 * 0 = breeding sow + suckling piglets, 1 = piglets, 2 = finishers, 3 = replacement sows
	 */
#ifdef __BCplusplus__
	if (pigsfnn)
#else
	if (pigsfnn->is_open())
#endif
	{
		getcwd(tempPath, 256);
		*pigsfnn << "[SwineStock(0)]" << endl;
		if (verbosity)
			cout << "writing " << tempPath << " " << filename << endl;
		for (int i=0; i<numPigCats; i++)
		{
			if (verbosity)
				cout << i << " " << pigArr[i] << endl;
			switch (i) {
			case 0: *pigsfnn << "SOWS" << "\t" << pigArr[i] << endl;
				if (verbosity)
					cout << "SOWS" << "\t" << pigArr[i] << endl;
				break;
			case 1: *pigsfnn << "PIGLETS" << "\t" << pigArr[i] << endl;
				if (verbosity)
					cout << "PIGLETS" << "\t" << pigArr[i] << endl;
				break;
			case 2: *pigsfnn << "BACONERS" << "\t" << pigArr[i] << endl;
				if (verbosity)
					cout << "BACONERS" << "\t" << pigArr[i] << endl;
				break;
			case 3: *pigsfnn << "NEWSOWS" << "\t" << pigArr[i] << endl;
				if (verbosity)
					cout << "NEWSOWS" << "\t" << pigArr[i] << endl;
				break;
			default: cout << "unknown pig class" << endl;
			ret_val=2;
			}
		}
		*pigsfnn << "[end]" << endl;
		pigsfnn->close();
		char tempPath[256];
		getcwd(tempPath, 256);
		if (verbosity)
			cout << "Closing " << tempPath << " " << filename << endl;
	}
	else
	{
		cout << "Error opening " << " " << filename << endl;
		ret_val=false;
	}
	return ret_val;
}

bool farmInitClass::readLivestock(bool *dairyPresent, bool *beefPresent, bool *pigsPresent, bool *poultryPresent)
{
	bool retVal = true;
	*dairyPresent=false;
	*beefPresent=false;
	*pigsPresent=false;
	*poultryPresent = false;
	/*
	 * 1 = dairy cow, large, 2 = dairy cow small, 3 = dairy heifer >1 yr, 4 = dairy calf, female, 0.5-1 yr, 5 = dairy calf, female, 0-0.5 yr
	 * 6 = suckler cow, 7 = beef, female, >1 yr, 8 = beef, female, 0.5-1yr, 9 = beef, female, 0-0.5 yr, 10 = beef male, >1 yr, 11 = beef male, 0.5-1yr
	 * 12 = beef, male 0-0.5 yr,
	 * 13 = breeding sow + suckling piglets, 14 = piglets, 15 = finishers, 16 = replacement sows
	 */
	for (int i=0;i<numCattleCats;i++)
		cattleArr[i]=0;
	for (int i=0;i<numPigCats;i++)
		pigArr[i]=0;
	for (int i=0;i<numPoultryCats;i++)
		poultryArr[i]=0;

	int numLivestock = xNodeFarm->nChildNode("farmlstkbalance");
	for (int i=0;i<numLivestock;i++)
	{
		XMLNode xNodeLivestock=xNodeFarm->getChildNode("farmlstkbalance",i);
		if (xNodeLivestock.isEmpty())
		{
			cout << "Node farmlstkbalance is missing or empty" << endl;
			return false;
		}
		XMLNode xNode = xNodeLivestock.getChildNode("lstktypeid");
		if (!xNode.getText())
		{
			cout << "Node lstktypeid is missing or empty" << endl;
			return false;
		}
		int livestockType = atoi(xNode.getText());
		float livestockStart=0.0;
		float livestockEnd=0.0;
		float livestockAve=0.0;

		switch (livestockType)
		{
			//cattle
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
			case 9:
			case 10:
			case 11:
			case 12:
				xNode=xNodeLivestock.getChildNode("lstkpresentatstartyear");
				if (xNode.getText())
					livestockStart = atoi(xNode.getText());
				else
				{
					cout << "lstkpresentatstartyear missing" << endl;
					retVal=false;
				}
				xNode=xNodeLivestock.getChildNode("lstkpresentatendyear");
				if (xNode.getText())
					livestockEnd = atoi(xNode.getText());
				else
				{
					cout << "lstkpresentatendyear missing" << endl;
					retVal=false;
				}
				livestockAve=(livestockStart+livestockEnd)/2;
				cattleArr[livestockType-1]=livestockAve;
				break;
				//pigs
			case 13:
			case 14:
			case 15:
			case 16:
				if (livestockType==13)
				{
					xNode=xNodeLivestock.getChildNode("lstkpresentatstartyear");
					livestockStart = atoi(xNode.getText());
					xNode=xNodeLivestock.getChildNode("lstkpresentatendyear");
					livestockEnd = atoi(xNode.getText());
					livestockAve=(livestockStart+livestockEnd)/2;
					pigArr[livestockType-(12+1)]=livestockAve;
				}
				else
				{
					xNode=xNodeLivestock.getChildNode("lstksoldthisyear");
					double livestockSold = atoi(xNode.getText());
					pigArr[livestockType-(12+1)]=livestockSold;
				}
				break;
			case 17:
			case 18:
			case 19:
			case 20:
			case 21:
			case 22:
			case 23:
			case 24:
			case 25:
			case 26:
				break;
			case 27:
			case 28:
			case 29:
				poultryArr[livestockType-(26+1)]=livestockAve;
				break;
			case 30:
			case 31:
			case 32:
			case 33:
			case 34:
			case 35:
			case 36:
				break;

			default:
				cout << "unknown livestock category" << endl;
		}
	}
	for (int i=0;i<numDairyCats;i++)
		if (cattleArr[i]>0)
			*dairyPresent=true;
	for (int i=numDairyCats-1;i<numBeefCats;i++)
		if (cattleArr[i]>0)
			*beefPresent=true;
	for (int i=0;i<numPigCats;i++)
		if (pigArr[i]>0)
			*pigsPresent=true;
	for (int i=0;i<numPoultryCats;i++)
		if (poultryArr[i]>0)
			*poultryPresent=true;
	return retVal;
}

bool farmInitClass::writeLivestockfnn(bool dairyPresent, bool beefPresent, bool pigsPresent, bool poultryPresent)
{
	bool retVal=true;
	if ((dairyPresent)||(beefPresent))
		retVal=writeCattlefnn(cattleArr, numCattleCats);
	if ((pigsPresent)&&(retVal))
		retVal=writePigsfnn(pigArr, numPigCats);
	return retVal;
}
bool farmInitClass::initBuildings()
{
	bool retVal=true;
	int numHouses = theHousing->NumOfNodes();
	for (int i=0; i<numHouses; i++)
	  {
		XMLNode xNodeHouses=xNodeFarm->getChildNode("lstkbuilding",i);
		animalHousingClass *aanimalHousingClass = theHousing->ElementAtNumber(i);

		if (!aanimalHousingClass->readHousing(&xNodeHouses))
		{
			string outString ="Data missing in house ";
			char *achar;
			itoa(aanimalHousingClass->getID(),achar,100);
			string tempString = achar;
			outString.append(tempString);
			theMessage->WarningWithDisplay(outString.c_str());
			retVal=false;
		}
	  }
	int numStorage = theManureStorage->NumOfNodes();
	for (int i=0; i<numStorage; i++)
	  {
		XMLNode xNodeStorage=xNodeFarm->getChildNode("manurestore",i);
		manureStorageClass *amanureStorageClass = theManureStorage->ElementAtNumber(i);

		if (!amanureStorageClass->readManureStorage(&xNodeStorage))
		{
			amanureStorageClass->printContents();
			string outString ="Data missing in manure storage ";
			char achar[50];
			itoa(amanureStorageClass->getmanurestoreid(),achar,100);
			string tempString = achar;
			outString.append(tempString);
			theMessage->WarningWithDisplay(outString.c_str());
			retVal=false;
		}
	  }
	if (retVal)
	{
		if (!checkBuildings())
			retVal=false;
	}
	return retVal;
}

bool farmInitClass::checkBuildings()
{
	bool retVal=true;
	int numHouses = theHousing->NumOfNodes();
	for (int i=0; i<numHouses; i++)
	{
		animalHousingClass *aanimalHousingClass = theHousing->ElementAtNumber(i);
		for (int j=0; j<aanimalHousingClass->GettheSections()->NumOfNodes(); j++)
		{
			bool gotit=false;
			int destStore = aanimalHousingClass->GettheSections()->ElementAtNumber(j)->getmanureDestination();
			if (!destStore==-1)
			{
				for (int k=0; k<theManureStorage->NumOfNodes(); k++)
				{
					int recipientStore = theManureStorage->ElementAtNumber(k)->getmanurestoreid();
					if (verbosity)
						cout << destStore << " " << recipientStore << endl;
					if (destStore==recipientStore)
						gotit=true;
				}
				if (!gotit)
				{
					retVal=false;
					cout << "No recipient store for animal section " << aanimalHousingClass->GettheSections()->ElementAtNumber(j)->getsectionID() << endl;
					cout << "Missing storage ID = " << aanimalHousingClass->GettheSections()->ElementAtNumber(j)->getmanureDestination() << endl;
					cout << "House ID = " << aanimalHousingClass->getID() << endl;
				}
			}
		}
	}
	return retVal;
}

bool farmInitClass::writeBuildings()
{
	bool retVal=true;
	int numHouses = theHousing->NumOfNodes();
	runBuildings = new ofstream();
	runBuildings->open("RunBuildings.dat");
#ifdef __BCplusplus__
	if (runBuildings)
#else
	if (runBuildings->is_open())
#endif
	{
		if (numHouses>0)
		{
			int numCattleHousing =0;
			int numPigHousing =0;
			int numPoultryHousing =0;
			int numOtherHousing =0;
			for (int i=0; i<numHouses; i++)
			{
				animalHousingClass *aanimalHousingClass = theHousing->ElementAtNumber(i);
				int housingType = aanimalHousingClass->getHousingType();
				if (housingType>0)
				{
					if (housingType==-999)
						retVal=false;
					if (retVal)
					{
						string housingName;
						char num[3];
						switch (housingType)
						{
							case 1:
								 itoa(numCattleHousing, num, 10);
								 housingName.assign("Cattlestable(");
								 numCattleHousing++;
								 break;

							case 2:
								 itoa(numPigHousing, num, 10);
								 housingName.assign("Pigstable(");
								 numPigHousing++;
								 break;
							case 3:
								 itoa(numPoultryHousing, num, 10);
								 housingName.assign("Poultrystable(");
								 numPoultryHousing++;
								 break;
							case 4:
								 itoa(numOtherHousing, num, 10);
								 housingName.assign("Otherstable(");
								 numOtherHousing++;
								 break;
						}
						 housingName.append(num);
						 housingName.append(")");
						*runBuildings << "[" << housingName << "]" << endl;
						aanimalHousingClass->writeHousingToBuildingsFile(runBuildings,housingName);
					}
				}
			}
		}
		int numManureStores = theManureStorage->NumOfNodes();
		if (numManureStores>0)
		{
			int numSlurryStores=0;
			int numDungHeaps =0;
			int manureStoreType=0;
			for (int i=0; i<numManureStores; i++)
			{
				manureStorageClass *amanureStorageClass = theManureStorage->ElementAtNumber(i);
				manureStoreType=amanureStorageClass->getType();
				string manureStoreName;
				char num[3];
				switch (manureStoreType)
				{
				case 1:
				case 2:
				case 3:
				case 4:
						 itoa(numSlurryStores, num, 10);
						 manureStoreName.assign("Slurrystore(");
						 //*runBuildings << "Manuretype" << "\t" <<"SLURRY" << endl;
						 numSlurryStores++;
						 break;
				case 6:
				case 7:
				case 8:
				case 9:
					 itoa(numDungHeaps, num, 10);
					 manureStoreName.assign("DungHeap(");
					 numDungHeaps++;
					 break;
				}

				manureStoreName.append(num);
				manureStoreName.append(")");
				*runBuildings << "[" << manureStoreName << "]" << endl;
				amanureStorageClass->writeStorageToBuildingsFile(runBuildings);
			}
		}
		*runBuildings << "[end]" << endl;
		runBuildings->close();
	}
	else
		retVal=false;
	return retVal;
}

void farmInitClass::FindCropOps(string cropName, int opNum, fstream *outfile)
{
	int numFields=theFields->NumOfNodes();
	for (int j=0;j<numFields;j++)
	{
		cout << "reading field no " << j << endl;
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(j);
		linkList<fieldOp> *theOperations = acropSeriesClass->gettheOperations();
		bool isOk=false;
		linkList<fieldOp> *aList = new linkList<fieldOp>;
		for (int k=0; k<theOperations->NumOfNodes(); k++)
		{
			fieldOp * afieldOp = theOperations->ElementAtNumber(k);
			bool skip = false;
			if (afieldOp->getopCode()==2)
			{
				if (((fieldOpSow *)afieldOp)->getplantName()==cropName)
					isOk=true;
				else
					skip = true;
			}
			if (!skip)
				aList->InsertLast(afieldOp);
			if (afieldOp->getopCode()==9)
			{
				if (isOk)
				{
					for (int L=0; L<aList->NumOfNodes(); L++)
					{
						fieldOp * afieldOp = aList->ElementAtNumber(L);
						if (afieldOp->getopCode()==opNum)
						{
							*outfile << farmID << "\t" << acropSeriesClass->getseriesNumber() << "\t";
							afieldOp->outputOps(outfile,0,true);
							*outfile << endl;
							cout << endl;
						}
					}
				}
				aList->ForgetAll();
				isOk=false;
			}
		}
	}
}


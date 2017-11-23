#include "farmInitClass.h"
#include "common.h"

farmInitClass::farmInitClass()
{
}

farmInitClass::~farmInitClass()
{
	delete theFields;
}
void farmInitClass::initFarm(XMLNode *aNodeFarm)
{
	xNodeFarm = aNodeFarm;
	theFields = new linkList<cropSeriesClass>;
	
	int numFields = xNodeFarm->nChildNode("fields");
	for (int i=0; i<numFields; i++)
	{
		cropSeriesClass *acropSeriesClass = new cropSeriesClass(i);
		theFields->InsertLast(acropSeriesClass);
	}
}
void farmInitClass::writeFarmdat(bool pig, bool dairy, bool beef)
{
	ofstream farmfile;
	farmfile.open("farm.dat");
	farmfile << "[farm]" << endl;

	farmfile << "ScaleBuildings" << "\t" << "1" << endl;
	farmfile << "SellAllStraw" << "\t" << "1" << endl;
	farmfile << "Watering" << "\t" << "0" << endl;
	farmfile << "FarmID	" << "\t" << "0" << endl;
	farmfile << "[livestock]" << endl;
	if (pig)
		farmfile << "pigfarm" << "\t" << "0" << endl;
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
	farmfile << "[end]" << endl;
	farmfile.close();
}

void farmInitClass::initFielddat()
{
	fielddat.open("fields.dat");
	int numFields=theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	  {
		XMLNode xNodeFields=xNodeFarm->getChildNode("fields",i);
		XMLNode xNode=xNodeFields.getChildNode("fieldarea");
		int soiltype=4;
		float fieldarea = atof(xNode.getText());
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

void farmInitClass::initFieldsfnn(int number)
{
	char yearNum[2];
	sprintf(yearNum,"%d",number);
	char filename[20];
	strcpy(filename,"fields.f");
	if (number<10)
		strcat(filename,"0");
	strcat(filename,yearNum);
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "writing " << tempPath << " " << filename << endl;		
	fieldsfnn = new ofstream();
	fieldsfnn->open(filename,ios::out);
	
	if (fieldsfnn->is_open())
	{
		getcwd(tempPath, 256);
		cout << "writing " << tempPath << " " << filename << endl;		
	}
	else
		cout << "Error opening " << " " << filename << endl;
}

void farmInitClass::closeFieldsfnn()
{
	*fieldsfnn << "[end]" << endl;
	cout << "[end]" << endl;
	fieldsfnn->close();
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "Closing in path " << tempPath << endl;
}

void farmInitClass::writeFieldsfnn(int startYear, int endYear)
{
	bool contin=true;
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
			if (acropSeriesClass->OutputOpsTofnn(fieldsfnn,year))
				contin=false;
			*fieldsfnn << endl;
			cout << endl;
		}
		closeFieldsfnn();
		number++;
	}
}

void farmInitClass::readFieldOps(int startYear, int endYear)
{
	int numFields=theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	{
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
		XMLNode xNodeFields=xNodeFarm->getChildNode("fields",i);
		acropSeriesClass->addOperations(&xNodeFields,startYear, endYear);
	}
};
void farmInitClass::writeFieldOps()
{
	fstream outfile;
	outfile.open("outfile.txt",fstream::out);
	char dum[200];
	cout << "writing to directory " << getcwd(dum,200) << endl;
	int numFields=theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	{
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
		acropSeriesClass->OutputOps(&outfile,1);
		outfile << endl;
		cout << endl;
	}
	outfile.close();
};

int farmInitClass::writeCattlefnn(int yearNumber, float cattleArr[], int numCattleCats)
{
	int ret_val=0;
	char yearNum[2];
	sprintf(yearNum,"%d",yearNumber);
	char filename[20];
	strcpy(filename,"cattle.f");
	if (yearNumber<10)
		strcat(filename,"0");
	strcat(filename,yearNum);
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "writing " << tempPath << " " << filename << endl;
	cattlefnn = new ofstream();
	cattlefnn->open(filename,ios::out);

	/*
	 * 0 = dairy cow, large, 1 = dairy cow small, 2 = dairy heifer >1 yr, 3 = dairy calf, female, 0.5-1 yr, 4 = dairy calf, female, 0-0.5 yr
	 * 5 = suckler cow, 6 = beef, female, >1 yr, 7 = beef, female, 0.5-1yr, 8 = beef, female, 0-0.5 yr, 9 = beef male, >1 yr, 10 = beef male, 0.5-1yr
	 * 11 = beef, male 0-0.5 yr
	 */
	if (cattlefnn->is_open())
	{
		getcwd(tempPath, 256);
		cout << "Writing to " << tempPath << " " << filename << endl;
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
			case 5: *cattlefnn << "SUCKLER" << "\t" << cattleArr[i] << endl;
				break;
			case 6: *cattlefnn << "FBEEF1" << "\t" << cattleArr[i] << endl;
				break;
			case 7: *cattlefnn << "FBEEF2" << "\t" << cattleArr[i] << endl;
				break;
			case 8: *cattlefnn << "FBEEF3" << "\t" << cattleArr[i] << endl;
				break;
			case 9: *cattlefnn << "MBEEF1" << "\t" << cattleArr[i] << endl;
				break;
			case 10: *cattlefnn << "MBEEF2" << "\t" << cattleArr[i] << endl;
				break;
			case 11: *cattlefnn << "MBEEF3" << "\t" << cattleArr[i] << endl;
				break;
			default: cout << "unknown cattle class" << endl;
			ret_val=2;
			}
		}
		*cattlefnn << "[end]" << endl;
		cattlefnn->close();
		char tempPath[256];
		getcwd(tempPath, 256);
		cout << "Closing file " << tempPath << " " << filename << endl;
	}
	else
	{
		cout << "Error opening " << " " << filename << endl;
		ret_val=1;
	}
	return ret_val;
}
int farmInitClass::writePigsfnn(int number, float pigArr[], int numPigCats)
{
	int ret_val=0;
	char yearNum[2];
	sprintf(yearNum,"%d",number);
	char filename[20];
	strcpy(filename,"pigs.f");
	if (number<10)
		strcat(filename,"0");
	strcat(filename,yearNum);
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "Writing to " << tempPath << " " << filename << endl;
	pigsfnn = new ofstream();
	pigsfnn->open(filename,ios::out);

	/*
	 * 0 = breeding sow + suckling piglets, 1 = piglets, 2 = finishers, 3 = replacement sows
	 */
	if (pigsfnn->is_open())
	{
		getcwd(tempPath, 256);
		cout << "writing " << tempPath << " " << filename << endl;
		for (int i=0; i<numPigCats; i++)
		{
			switch (i) {
			case 0: *pigsfnn << "SOWS" << "\t" << pigArr[i] << endl;
				break;
			case 1: *pigsfnn << "PIGLETS" << "\t" << pigArr[i] << endl;
				break;
			case 2: *pigsfnn << "BACONERS" << "\t" << pigArr[i] << endl;
				break;
			case 3: *pigsfnn << "NEWSOWS" << "\t" << pigArr[i] << endl;
				break;
			default: cout << "unknown pig class" << endl;
			ret_val=2;
			}
		}
		*pigsfnn << "[end]" << endl;
		cout << "[end]" << endl;
		pigsfnn->close();
		char tempPath[256];
		getcwd(tempPath, 256);
		cout << "Closing " << tempPath << " " << filename << endl;
	}
	else
	{
		cout << "Error opening " << " " << filename << endl;
		ret_val=1;
	}
	return ret_val;
}

int farmInitClass::writeLivestockfnn(int startYear, int endYear, bool *dairyPresent, bool *beefPresent, bool *pigsPresent)
{
	*dairyPresent=false;
	*beefPresent=false;
	*pigsPresent=false;
	int numDairyCats = 4;
	int numBeefCats = 8;
	int numCattleCats = numBeefCats + numDairyCats;
	int numPigCats = 4;
	int number=0;
	/*
	 * 1 = dairy cow, large, 2 = dairy cow small, 3 = dairy heifer >1 yr, 4 = dairy calf, female, 0.5-1 yr, 5 = dairy calf, female, 0-0.5 yr
	 * 6 = suckler cow, 7 = beef, female, >1 yr, 8 = beef, female, 0.5-1yr, 9 = beef, female, 0-0.5 yr, 10 = beef male, >1 yr, 11 = beef male, 0.5-1yr
	 * 12 = beef, male 0-0.5 yr,
	 * 13 = breeding sow + suckling piglets, 14 = piglets, 15 = finishers, 16 = replacement sows
	 */
	float cattleArr[numCattleCats];
	for (int i=0;i<numCattleCats;i++)
		cattleArr[i]=0;
	float pigArr[numPigCats];
	for (int i=0;i<numPigCats;i++)
		pigArr[i]=0;

	int numlivestockYearNodes = xNodeFarm->nChildNode("lstkyear");
	int year=0;
	while (year<(endYear-startYear))
	{
		XMLNode livestockYearNode = xNodeFarm->getChildNode("lstkyear",year);
		if (livestockYearNode.isEmpty())
			return 1;
		else
		{
			int checkYear=0;
			XMLNode xNode = livestockYearNode.getChildNode("year");
			if (xNode.isEmpty())
				return 2;
			else
				checkYear = atoi(xNode.getText());
			cout << "yr " << checkYear << endl;
			int numLivestock = livestockYearNode.nChildNode("farmlstkbalance");
			for (int i=0;i<numLivestock;i++)
			{
				XMLNode xNodeLivestock=livestockYearNode.getChildNode("farmlstkbalance",i);
				if (xNodeLivestock.isEmpty())
					return 3;
				xNode=xNodeLivestock.getChildNode("lstktypeid");
				int livestockType = atoi(xNode.getText());
				float livestockStart=0.0;
				float livestockEnd=0.0;
				float livestockAve=0.0;
				xNode=xNodeLivestock.getChildNode("lstkpresentatstartyear");
				livestockStart = atoi(xNode.getText());
				xNode=xNodeLivestock.getChildNode("lstkpresentatendyear");
				livestockEnd = atoi(xNode.getText());
				livestockAve=(livestockStart+livestockEnd)/2;
				if (livestockType<=numCattleCats)
					cattleArr[livestockType-1]=livestockAve;
				else
					pigArr[livestockType-(numCattleCats+1)]=livestockAve;
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
			writeCattlefnn(year, cattleArr, numCattleCats);
			writePigsfnn(year, pigArr, numPigCats);
			year++;
		}
	}


}


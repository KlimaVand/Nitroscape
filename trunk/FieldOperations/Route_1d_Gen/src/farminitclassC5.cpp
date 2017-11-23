#include "farmInitClassC5.h"
#include "common.h"
#include "unix_util.h"

using namespace std;

farmInitClassC5::farmInitClassC5(int aNCU)
{
	NCU=aNCU;
	theFields = new linkList<cropSeriesClass>;
}

farmInitClassC5::~farmInitClassC5()
{
}

void farmInitClassC5::writeFarmdat(char *outputDirectory, bool pig, bool dairy, bool beef)
{
	chdir(outputDirectory);
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

void farmInitClassC5::initFielddat(char *outputDirectory)
{
	chdir(outputDirectory);
#ifdef __BCplusplus__
	fielddat.open("fields.dat", ios::out);
#else
	fielddat.open("fields.dat");
#endif

	int numFields=theFields->NumOfNodes();
   for (int i=0;i<numFields;i++)
     {
      fielddat << "[field(" << i << ")]" << endl;
      fielddat << "area" << "\t" << "1" << endl;
      int soiltype = 4;
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

void farmInitClassC5::initFieldsfnn(int number)
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

	fieldsfnn = new fstream();
	fieldsfnn->open(filename,ios::out);
#ifdef __BCplusplus__
	if (fieldsfnn)
#else
	if (fieldsfnn->is_open())
#endif

	{
		getcwd(tempPath, 256);
		cout << "writing " << tempPath << " " << filename << endl;
	}
	else
		cout << "Error opening " << " " << filename << endl;
}

void farmInitClassC5::closeFieldsfnn()
{
	*fieldsfnn << "[end]" << endl;
	cout << "[end]" << endl;
	fieldsfnn->close();
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "Closing in path " << tempPath << endl;
}

void farmInitClassC5::writeFieldsfnn(char *outputDirectory)
{
	chdir(outputDirectory);
	bool contin=true;
	int number=0;
	int startYear = getFirstYear();
	int endYear = getLastYear();
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

bool farmInitClassC5::readFieldOps(ifstream *opFile)
{
	bool ret_val;
   cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(0);
   ret_val=acropSeriesClass->ReadFieldOps(opFile);
};

void farmInitClassC5::writeFieldOps()
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

int farmInitClassC5::getFirstYear()
{
	int startYear =99999;
	int dumYear;
	int numFields = theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	{
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
		dumYear=acropSeriesClass->getFirstYear();
		if (dumYear<startYear)
			startYear=dumYear;
	}
	return startYear;
}
int farmInitClassC5::getLastYear()
{
	int lastYear =0;
	int dumYear;
	int numFields = theFields->NumOfNodes();
	for (int i=0;i<numFields;i++)
	{
		cropSeriesClass *acropSeriesClass = theFields->ElementAtNumber(i);
		dumYear=acropSeriesClass->getLastYear();
		if (dumYear>lastYear)
			lastYear=dumYear;
	}
	return lastYear;
}


bool farmInitClassC5::processFieldOps(ifstream *opFile, char *outputDir, int *aNCU)
{
	bool ret_val=true;
	bool nextNCU=false;
	bool nextSeries=false;
	int cropSeries =-1;
	NCU=*aNCU;
	int testNCU = NCU;
	bool first = true;
	while ((ret_val)&&(!nextNCU))
	{
		int testcropSeries=0;
		if(first)
		{
			char NCU_char[30];
			itoa(NCU,NCU_char,30);
         strcpy(directoryName,outputDir);
			strcat(directoryName,NCU_char);
			do_makeDirectory(directoryName);
			testNCU=NCU;
		}
		else
			*opFile >> testNCU;
		if (testNCU!=NCU)//new NCU
		{
			*aNCU = testNCU;
			nextNCU=true;
			writeFieldsfnn(directoryName);
		}
		else//read this NCU
		{
			*opFile >> testcropSeries;
			cout << "NCU " << testNCU << " series " << testcropSeries << " ";
			if (first)
			{
				first = false;
			}
			cropSeriesClass * acropSeriesClass;
			if (testcropSeries!=cropSeries)
			{
				cropSeries=testcropSeries;
				acropSeriesClass = new cropSeriesClass(cropSeries);
				theFields->InsertLast(acropSeriesClass);
			}
			linkList<cropSeriesClass>::PS P;
			theFields->PeekTail(P);
			acropSeriesClass = P->element;
			if (acropSeriesClass->ReadFieldOps(opFile))
			{
				ret_val=false;
				theFields->Remove(P);
         	writeFieldsfnn(directoryName);
			}
		}
	}
   return ret_val;
}

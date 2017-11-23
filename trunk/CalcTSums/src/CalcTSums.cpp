
#include <iostream>
#include <cstring>
#include "base\bsTime.h"
#include "tools\fileAccess.h"
#include "base\message.h"
#include "climateNCU_MARS_ATEAM.05.Extract_Data.Func.V2.h"

bsTime theTime;
message *theMessage;
bool verbosity = true;
fileAccess hd;

using namespace std;

int getMetData(int ID, int startDay, int startYear, int endDay, int endYear, char * inputDir,
	char * tpvStem, char * radStem, char * ncu_mars, int baseYear, int range, double results[][366][6]);
int getNCUfromHSMU(int,char[100]);

//! 0 = mean temperature, Celsius; 1=max temperature, Celsius; 2=precipitation (mm); 3=vapour pressure; 4=global radiation, MJ/m**2; 5=potential evapotranspiration (mm)
   double results[50][366][6];

int main()
{
#ifdef __WINDOWS__
#ifdef __PORTABLE__
   char inputDir[] = "c:\\Documents and Settings\\njh\\workspace\\nitroscape\\trunk\\CalcTSums\\InputData\\";
   char outputDir[] = "c:\\Documents and Settings\\njh\\workspace\\nitroscape\\trunk\\CalcTSums\\OutputData\\";
   char metdataInputDir[] = "D:\\";
	char tpvStem[] 	= "MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#else
	char inputDir[] = "c:\\Users\\NJH\\workspace\\nitro\\trunk\\CalcTSums\\InputData\\";
	char outputDir[] = "c:\\Users\\NJH\\workspace\\nitro\\trunk\\CalcTSums\\OutputData\\";
	char metdataInputDir[] = "c:\\Datastore\\JRCMetdatastore\\";
	char tpvStem[] 	= "tpv\\MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
	char radStem[]		= "rad\\radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#endif
#else
   char inputDir[] = "/home/njh/workspace/InputFiles/Route_1/";
   char metdataInputDir[] = "/home/njh/workspace/datastore/";
	char tpvStem[] 	= "tpv/MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad/radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char outputDir[] = "/home/njh/workspace/OutputFiles/Route_1d/";
#endif

   theMessage = new message();
   string warningsFileName;
   warningsFileName.assign(outputDir);
   warningsFileName.append("warnings.txt");
   char temp[200];
   strcpy(temp,warningsFileName.c_str());
   theMessage->InitMessage(temp);

   char ncu_mars[]	= "climateNCU_MARS_ATEAM.ncus.txt";
   int baseYear	= 1991;
   int range	= 10;
   int startNCU = -1;//2037;
   int endNCU = -1;//2037;
	int ID =10;
   int startDay=1;
   int startYear = 1991;
   int endDay = 365;
   int endYear = 1993;
   for (int i=0; i<50; i++)
   	for (int j=0; j<388; j++)
   		for (int k=0; k<6; k++)
		   	results[i][j][k]=0.0;
   string tempString;
   tempString.assign(inputDir);
   if (hd.changeDir(tempString))
	   cout << "Failed to change directory" << endl;
	getMetData(ID, startDay, startYear, endDay, endYear, metdataInputDir,
			tpvStem, radStem, ncu_mars, baseYear, range, results);
   char metFileName[200];
	char * atempname = "metfile";
   sprintf(metFileName,"%s\%s.clm",outputDir,atempname);
	fstream *metFile = new fstream();
   metFile->open(metFileName,fstream::out);
   theTime.SetTime(endYear,endDay);
   int year=(endYear-startYear);
   double TSum=300.0;
   int sowDate=0;
   while (year>=0)
   {
	   int day=(theTime.daysInYear(0)-1);
	   bool gotit=false;
	   double cumTemperature=0;
	   while ((day>=0)&&(!gotit))
	   {
        double meanTemp = results[year][day][0];
        cumTemperature +=meanTemp;
        if (cumTemperature>=TSum)
        {
        	cout << "day " << day << endl;
        	gotit=true;
        }
        //cout << (year + startYear) << "\t"<< (day +1) << "\t" << meanTemp << endl;
/*        *metFile << (year + startYear) << "\t" << month << "\t" << dayInMonth << "\t" << minTemp << "\t" << maxTemp << "\t"
              << globalRad << "\t" << precip << "\t" << epot<< endl;*/

      	theTime.SubtractOneDay();
      	day--;
	   }
  	year--;
  }
 // metFile->close();
	delete theMessage;
	return 0;
}

#include "MetDataExtractionRoutinesMARSREMO.h"
#include "message.h"
#include "bsTime.h"

//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
bool verbosity = true;


//!Choice of algorithm for deriving timing of field operations
int routeNumber = 1;

extern int getNCU(int,char[100]);
extern int getNCUfromHSMU(int,char[100]);

//! 0 = mean temperature, Celsius; 1=max temperature, Celsius; 2=precipitation (mm); 3=vapour pressure; 4=global radiation, MJ/m**2; 5=potential evapotranspiration (mm)
   double results[50][366][6];

int main(int argc,char *argv[])
{
#ifdef __WINDOWS__
//   char outputDir[] = "c:\\Documents and Settings\\njh\\Dokumenter\\My Dropbox\\CropManagement\\OutputFiles\\";
	#ifdef __PORTABLE__
		char inputDir[] = "c:\\Documents and Settings\\njh\\workspace\\nitroscape\\trunk\\MARStoFASSET2\\InputData\\";
		char outputDir[] = "c:\\Documents and Settings\\njh\\workspace\\nitroscape\\trunk\\MARStoFASSET2\\OutputData\\";
		char metdataInputDir[] = "D:\\";
		char tpvStem[] 	= "MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
		char radStem[]		= "radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
	#else
	    char inputDir[] = "c:\\Users\\NJH\\workspace\\nitro\\trunk\\MARStoFASSET2\\InputData\\";
	   	char outputDir[] = "C:\\Users\\NJH\\workspace\\nitro\\trunk\\MARStoFASSET2\\OutputData\\";
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
   chdir(inputDir);
	getMetData(ID, startDay, startYear, endDay, endYear, metdataInputDir,
	tpvStem, radStem, ncu_mars, baseYear, range, results);
   char metFileName[200];
	char * atempname = "metfile";
   sprintf(metFileName,"%s\%s.clm",outputDir,atempname);
	fstream *metFile = new fstream();
   metFile->open(metFileName,fstream::out);
   theTime.SetTime(startYear,startDay);
	for (int year=0; year<((endYear-startYear)+1); year++)
   {
   	for (int day=0; day<(theTime.daysInYear(0)); day++)
      {
      	int month = theTime.GetMonth();
         int dayInMonth=theTime.GetDay();
         double meanTemp = results[year][day][0];
         double maxTemp = results[year][day][1];
         double precip = results[year][day][2];
         double vapourPress = results[year][day][3];
         double globalRad = results[year][day][4];
         double epot = results[year][day][5];
         double minTemp =  meanTemp - (maxTemp-meanTemp);
         *metFile << (year + startYear) << "\t" << month << "\t" << dayInMonth << "\t" << minTemp << "\t" << maxTemp << "\t"
               << globalRad << "\t" << precip << "\t" << epot<< endl;
         cout << (year + startYear) << "\t" << month << "\t" << dayInMonth << "\t" << minTemp << "\t" << maxTemp << "\t"
               << globalRad << "\t" << precip << "\t" << epot << endl;
       	theTime.AddOneDay();
      }
   }
   metFile->close();
}

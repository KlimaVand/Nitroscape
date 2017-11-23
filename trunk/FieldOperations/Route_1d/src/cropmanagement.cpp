#include "modelControlClass.h"
#include "message.h"
#include "NCUCropClass.h"

//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
bool verbosity = false;

//!Choice of algorithm for deriving timing of field operations
int routeNumber = 1;

extern int getNCU(int,char[256]);
extern int getNCUfromHSMU(int,char[256]);

int main(int argc,char *argv[])
{
   int startDay = 1;
   int startYear = 1971;
   int endDay = 365;
   int endYear = 2000;
#ifdef __WINDOWS__
   char inputDir[] = "c:\\Users\\NJH\\workspace\\nitro\\trunk\\FieldOperations\\InputData\\Route_1d\\";
   //char inputDir[] = "c:\\Documents and Settings\\njh\\workspace\\nitroscape\\trunk\\FieldOperations\\InputData\\Route_1d\\";

   char outputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
#ifdef __PORTABLE__
   char metdataInputDir[] = "D:\\";
	char tpvStem[] 	= "MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#else
   char metdataInputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\Met_data\\datastore\\";
   char tpvStem[] 	= "tpv\\MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad\\radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#endif
#else
   char inputDir[] = "C:/Users/NJH/workspace/nitro/trunk/FieldOperations/InputData/Route_1d/";
   char metdataInputDir[] = "/home/njh/workspace/datastore/";
	char tpvStem[] 	= "tpv/MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad/radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char outputDir[] = "c:/Users/NJH/workspace/nitro/trunk/FieldOperations/Output/Route_1d/";
#endif

   typedef char string100[256];
   string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
   strcpy(OutputDirectory, outputDir);
//   strcpy(OutputDirectory, "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\Route 1\\output\\");
   strcpy(FN1,OutputDirectory);
   strcat(FN1,"warnings.txt");
   strcpy(FN2,OutputDirectory);
   strcat(FN2,"logfile.txt");
   strcpy(FN3,OutputDirectory);
   strcat(FN3,"debug.txt");      //debug file name
   theMessage = new message();
   theMessage->InitMessage(FN1,FN2,FN3);

   char ncu_mars[]	= "climateNCU_MARS_ATEAM.ncus.txt";
   int baseYear	= 1971;
   int range	= 10;
//	int startNCU = 0;//2037;
//	int endNCU = 2037;
	int startNCU = -1;//2037;
	int endNCU = -1;//2037;

	chdir(inputDir);
   char * atempname = "data4nick";
   //char * atempname = "TestData1";
   char croprotFileName[200];
	sprintf(croprotFileName,"%s\%s.txt",inputDir,atempname);
	fstream *rotationFile = new fstream();
   rotationFile->open(croprotFileName,fstream::in);
#ifdef __BCplusplus__
   if (!rotationFile)
#else
   if (!rotationFile->is_open())
#endif
	      theMessage->FatalError("NCUClass: crop rotation file ", croprotFileName, " not found");

   int maxNUCs = 50000;
   bool bootstrap = true;  //set true if NCUI is to be read from file

	modelControlClass *aControlClass = new modelControlClass();
//   aControlClass->Initialise(startDay,  startYear,  endDay,  endYear, maxNUCs, baseYear, range,
  //		startNCU,  endNCU, metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, rotationFile, true);

   char *outfileName = "outfile.txt";
  // aControlClass->calcCropManagement(inputDir, outputDir, outfileName);

 	aControlClass->modelControlClass::processCropManagement(startDay,  startYear,  endDay,  endYear, maxNUCs, baseYear, range,
  		startNCU,  endNCU, metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, outputDir, rotationFile, outfileName, true, verbosity);



   cout << "Finished" << endl;
   delete aControlClass;
   rotationFile->close();
   delete rotationFile;
   delete theMessage;
}

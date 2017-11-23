#include "modelControlClass.h"
#include "base\message.h"
#include "NCUCropClass.h"
#include "tools/fileAccess.h"

//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
bool verbosity = false;

//!Choice of algorithm for deriving timing of field operations
int routeNumber = 1;

extern int getNCU(int,char[100]);
extern int getNCUfromHSMU(int,char[100]);

int main(int argc,char *argv[])
{
	char ncu_mars[]	= "climateNCU_MARS_ATEAM.ncus.txt";
	int baseYear	= 1971;
	int range	= 10;
	bool zeroTillage=false;
	bool coverCropping=false;
	fileAccess hd;
	string commandFileName = "command.txt";
	hd.openFile(commandFileName,false);
	string input=hd.getLineFile();
	char inputDir[2000] ;
	strcpy(inputDir,input.c_str());
	int asci=inputDir[input.size()-1];
	if(asci==13)
	{
		inputDir[input.size()-1]='\0';
	}


	char outputDir[2000] ;
	string output=hd.getLineFile();
	strcpy(outputDir,output.c_str());
	asci=outputDir[output.size()-1];
	if(asci==13)
	{
		outputDir[output.size()-1]='\0';
	}

	typedef char string100[256];
	string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
	strcpy(OutputDirectory, outputDir);
	strcpy(FN1,OutputDirectory);
	strcat(FN1,"warnings.txt");
	strcpy(FN2,OutputDirectory);
	strcat(FN2,"logfile.txt");
	strcpy(FN3,OutputDirectory);
	strcat(FN3,"debug.txt");      //debug file name
	theMessage = new message();
	theMessage->InitMessage(FN1,FN2,FN3);

	string atempname;// "DummyCroprot5";
	atempname=hd.getLineFile();
	char croprotFileName[2000];
	strcpy(croprotFileName,atempname.c_str());
	asci=croprotFileName[atempname.size()-1];
	if(asci==13)
	{
		croprotFileName[atempname.size()-1]='\0';
	}

	int startDay = hd.GetIntFromFile();
	int startYear = hd.GetIntFromFile();;
	if (startYear<baseYear)
		theMessage->FatalError("Start year is before start of dataset");
	int endDay = hd.GetIntFromFile();
	int endYear = hd.GetIntFromFile();
	int dumint=0;
	dumint=hd.GetIntFromFile();
	if (dumint==1)
		zeroTillage=true;
	dumint=hd.GetIntFromFile();
	if (dumint==1)
		coverCropping=true;

	hd.closeFile();
	hd.changeDir(input);

#ifdef __PORTABLE__
	char metdataInputDir[] = "D:\\";
	char tpvStem[] 	= "MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
	char radStem[]		= "radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#else
	char metdataInputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\Met_data\\datastore\\";
	char tpvStem[] 	= "tpv\\MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
	char radStem[]		= "rad\\radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#endif

	//	int startNCU = 0;//2037;
	//	int endNCU = 2037;
	int startNCU = -1;//2037;
	int endNCU = -1;//2037;


	//char * atempname = "data4nick";

	char LongcroprotFileName[2000];
	sprintf(LongcroprotFileName,"%s\%s.txt",inputDir,croprotFileName);
	fstream *rotationFile = new fstream();
	rotationFile->open(LongcroprotFileName,fstream::in);
#ifdef __BCplusplus__
	if (!rotationFile)
#else
	if (!rotationFile->is_open())
#endif
			//	      theMessage->FatalError("NCUClass: crop rotation file ", croprotFileName.c_str(), " not found");
			theMessage->FatalError("NCUClass: crop rotation file ", croprotFileName, " not found");

	int maxNUCs = 50;
	bool bootstrap = true;  //set true if NCUI is to be read from file

	modelControlClass *aControlClass = new modelControlClass();
	char *outfileName = "outfile.txt";

	aControlClass->processCropManagement(startDay,  startYear,  endDay,  endYear, maxNUCs, 1, baseYear, range,
			startNCU,  endNCU, metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, outputDir, rotationFile, outfileName, true,
			verbosity, zeroTillage, coverCropping);

	cout << "Finished" << endl;
	delete aControlClass;
	rotationFile->close();
	delete rotationFile;
	delete theMessage;
}



/*
 * int main(int argc,char *argv[])
{
	fileAccess hd;
	string commandFileName = "command.txt";
	hd.openFile(commandFileName,false);
#ifdef __WINDOWS__
   string inputDir = hd.getLineFile();
   //disable the following line if reading from command.txt
   inputDir = "c://Users//NJH//workspace//nitro//trunk//FieldOperations//InputData//Route_2A//";
   string metdataInputDir = "//home//njh//workspace//datastore//";
   string tpvStem 	= "tpv//MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   string radStem		= "rad//radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   string outputDir = hd.getLineFile();//"/Users/NJH/workspace/nitro/trunk/FieldOperations/OutputFiles/Route_2A/";
#else
#ifdef __PORTABLE__
   char metdataInputDir[] = "D:\\";
	char tpvStem[] 	= "MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#else
   char metdataInputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\Met_data\\datastore\\";
   char tpvStem[] 	= "tpv\\MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad\\radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#endif
#endif
   char * croprotFileName;
   strcpy(croprotFileName, hd.getLineFile().c_str());
   int startDay = hd.GetIntFromFile();
   int startYear = hd.GetIntFromFile();
   int endDay = hd.GetIntFromFile();
   int endYear = hd.GetIntFromFile();
   hd.closeFile();

   typedef char string100[100];
   string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
   strcpy(OutputDirectory, outputDir.c_str());
//   strcpy(OutputDirectory, "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\Route 1\\output\\");
   strcpy(FN1,OutputDirectory);
   strcat(FN1,"warnings.txt");
   strcpy(FN2,OutputDirectory);
   strcat(FN2,"logfile.txt");
   strcpy(FN3,OutputDirectory);
   strcat(FN3,"debug.txt");      //debug file name
   theMessage->InitMessage(FN1,FN2,FN3);


   string ncu_mars	= "climateNCU_MARS_ATEAM.ncus.txt";
   int baseYear	= 1971;
   int range	= 10;
//	int startNCU = 0;//2037;
//	int endNCU = 2037;
	int startNCU = -1;//2037;
	int endNCU = -1;//2037;

	//hd.changeDir(inputDir);
	hd.changeDir("..//InputData//Route_2A");
	cout << hd.getCurrentPath() << endl;
//   char * atempname = "filelayout_example";
//   char * atempname = "data4nick1";
	//remove when chdir problem sorted
/*	string tempstring = croprotFileName;
	croprotFileName = inputDir + tempstring;*/
/*	char croprotFileLongName[200];
	sprintf(croprotFileLongName,"%s\%s.txt",inputDir.c_str(),croprotFileName);
	fstream *rotationFile = new fstream();
	rotationFile->open(croprotFileLongName,fstream::in);
//	hd.openFile(croprotFileName,false);
 */

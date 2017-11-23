/*! \mainpage Nitroscape Component 5 Route 1 file reader

   Group of C++ classes that are used to write or read the field operations to be used in
   NitroEurope Component 5.
   The file cropOpReader.cpp provides entry to a simple program that reads the file that provides the detailed
   ecosystem models with field operation data. The program creates instances of classes specific to individual
   field operations (e.g. tillage, harvesting). These classes contain functions that access operation-specific
   data (e.g. amount of nitrate applied in a fertiliser application). The program outputs the year, Julian day,
   operation name and operation code to console.

   For Windows,use __WINDOWS__ compiler directive, for Borland 5.02 use also __BCplusplus__, for CYGWIN, use __ANSICPP__
   For Linux, use __ANSICPP__ only

   NJH 2.2.10

*/



#include "base\message.h"
#include "base\bstime.h"
#include "farmInitClassC5.h"
#include "base\unix_util.h"
#include "tools\fileAccess.h"


void writeSystemDat(linkList <farmInitClassC5> *theFarms, string destDirectory, int startYear, int stopYear);

//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
int verbosityLevel = 0;

fileAccess hd;

//char cropCodes [30] [30] [20];
string cropCodes [30] [20];

linkList <farmInitClassC5> *theFarms;

//!main function for generating FASSET input files from Route 2B
int main(int argc,char *argv[])
{
#ifdef __WINDOWS__
   //char inputDir[] = "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
   //char outputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
   char metdataInputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\Met_data\\datastore\\";
   char tpvStem[] 	= "tpv\\MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad\\radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
#else
   char inputDir[] = "/home/njh/workspace/InputFiles/Route_1d_Gen/";
   char metdataInputDir[] = "/home/njh/workspace/datastore/";
	char tpvStem[] 	= "tpv/MARSATEAMNCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char radStem[]		= "rad/radCRU_NCU.x86_64-redhat-linux_g++4.1.2_LITTLE_ENDIAN";
   char outputDir[] = "/home/njh/workspace/OutputFiles/Route_1d/";
#endif


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
   if (!theMessage->InitMessage(FN1,FN2,FN3,false))
	   cout << "Error opening message files" << endl;

   string atempname;
   atempname=hd.getLineFile();
   char opFileName[200];
   strcpy(opFileName,atempname.c_str());
   asci=opFileName[atempname.size()-1];
   if(asci==13)
   {
	   opFileName[atempname.size()-1]='\0';
   }
   char opFileLongName[2000];
   strcpy(opFileLongName,inputDir);
   strcat(opFileLongName,opFileName);
   ifstream *opFile = new ifstream();
   opFile->open(opFileLongName,fstream::in);
   if (!opFile->is_open())
	   theMessage->FatalError("Crop operation file ", opFileLongName, " not found");


   //!Open the file containing model-specific crop identifiers
   ifstream *cropCodesfile = new ifstream();
   char * cropfilename = "FASSETcodes";
   char cropfilefullname[200];
   sprintf(cropfilefullname,"%s\%s.txt",inputDir,cropfilename);
   cropCodesfile->open(cropfilefullname,fstream::in);
   if (!cropCodesfile->is_open())
	   theMessage->FatalError("Crop code file ", cropfilename, " not found");
   int i = 0;
   while (!cropCodesfile->eof())
   {
	   *cropCodesfile >> cropCodes[i][0] >> cropCodes[i][1];
	   cout << cropCodes[i][0] << " " << cropCodes[i][1] << endl;     //for debugging only
	   i++;
   }
   cropCodesfile->close();
   delete cropCodesfile;

   theFarms = new linkList <farmInitClassC5>;
   
   int startYr=0;
   int endYr=0;
   bool notfinished = true;  //only here because with the Borland compiler, is_open does not want to return false at end of file
   int NCU;
   *opFile >> NCU;
#ifdef __BCplusplus__
   while ((opFile)&&(notfinished))
#else
	   while ((!opFile->eof())&&(notfinished))
#endif
	   {
		   while (notfinished)
		   {
			   farmInitClassC5 * aFarm = new farmInitClassC5(NCU);
			   aFarm->creatFarmDirectory(outputDir);
			   notfinished=aFarm->processFieldOps(opFile,inputDir, &NCU);
			   aFarm->writeFieldsfnn(aFarm->getOutputDirectoryName(),&startYr, &endYr);
			   aFarm->initFielddat(aFarm->getOutputDirectoryName());
			   bool pig=false;
			   bool dairy=false;
			   bool beef=false;
			   aFarm->writeFarmdat(aFarm->getOutputDirectoryName(), pig, dairy, beef);
			   theFarms->InsertLast(aFarm);
		   }
	   }
   opFile->close();
   string systemDir;
   systemDir.assign("C:\\Trash\\");
   writeSystemDat(theFarms,systemDir,startYr, endYr);
   delete theFarms;
   delete opFile;
   delete theMessage;
}


void writeSystemDat(linkList <farmInitClassC5> *theFarms, string destDirectory, int startYear, int stopYear)
{
	string sysFileName;
	sysFileName.assign(destDirectory);
	cout << destDirectory << endl;
	sysFileName.append("system.dat");
	ofstream *systFile = new ofstream();
	systFile->open(sysFileName.c_str());
	for (int i=0;i<theFarms->NumOfNodes(); i++)
	{
		farmInitClassC5 *afarm = theFarms->ElementAtNumber(i);
		string outString;
		outString.assign("[run(");
		char achar[3];
		itoa(i,achar,100);
		string tempString = achar;
		outString.append(tempString);
		outString.append(")]");

		*systFile << outString << endl;
		*systFile << "StartYear " << startYear << endl;
		*systFile << "StopYear " << stopYear << endl;
		*systFile << "EnvironmentalIndicator        1/4" << endl;
		*systFile << "InputDirectory  " << afarm->getOutputDirectoryName() << "\\" << endl;
		*systFile << "OutputDirectory "  << destDirectory << "output\\ " <<  endl;
		*systFile << "ClimateDirectory " << "\t" << afarm->getOutputDirectoryName() << endl;
		*systFile << "ClimateFileType 0 " << endl;
		outString.assign("\\foulum.clm");
		*systFile <<"ClimateFileName " << outString  <<  endl;
		*systFile <<"ForcedOperations        1"<<  endl;
		*systFile <<"VolatModel    1"<<  endl;
		*systFile <<"ForcedHarvest     1"<<  endl;
		*systFile <<"HarvestWaste  0.05"<<  endl;
		*systFile <<"StrawLeft 0.45"<<  endl;
	}
	*systFile << "[end]" << endl;
	systFile->close();
}

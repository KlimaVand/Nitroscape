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



#include "message.h"
#include "bstime.h"
#include "farmInitClassC5.h"
#include "unix_util.h"

//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
bool verbosity = false;


//char cropCodes [30] [30] [20];
string cropCodes [30] [20];

//!main function for Route 1 file reader
int main(int argc,char *argv[])
{
#ifdef __WINDOWS__
   char inputDir[] = "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
   char outputDir[] = "C:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
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


//!This sets up message reporting - largely irrelevant here
   typedef char string100[100];
   string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
   strcpy(OutputDirectory, inputDir);
   strcpy(FN1,OutputDirectory);
   strcat(FN1,"warnings.txt");
   strcpy(FN2,OutputDirectory);
   strcat(FN2,"logfile.txt");
   strcpy(FN3,OutputDirectory);
   strcat(FN3,"debug.txt");      //debug file name
   theMessage->InitMessage(FN1,FN2,FN3);

//!Open the file containing model-specific crop identifiers
	ifstream *cropCodesfile = new ifstream();
   char * cropfilename = "FASSETcodes";
   char cropfilefullname[200];
   sprintf(cropfilefullname,"%s\%s.txt",inputDir,cropfilename);
   cropCodesfile->open(cropfilefullname,fstream::in);
   if (!cropCodesfile)
	      theMessage->FatalError("Crop code file ", cropfilename, " not found");
   int i = 0;
   while (!cropCodesfile->eof())
   {
	   *cropCodesfile >> cropCodes[i][0] >> cropCodes[i][1];
//      cout << cropCodes[i][0] << " " << cropCodes[i][1] << endl;     //for debugging only
      i++;
   }
	cropCodesfile->close();
   delete cropCodesfile;
   
   //change to input directory
	chdir(inputDir);
   //! name of input file (will add '.txt' later
   char * atempname = "outfile";
   char fileName[200];
   sprintf(fileName,"%s\%s.txt",inputDir,atempname);
   ifstream *opFile = new ifstream();
   opFile->open(fileName,fstream::in);
   if (!opFile)
	      theMessage->FatalError("Crop operation file ", fileName, " not found");

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
		  notfinished=aFarm->processFieldOps(opFile,outputDir, &NCU);
        aFarm->initFielddat(aFarm->getOutputDirectoryName());
        bool pig=false;
        bool dairy=false;
        bool beef=false;
        aFarm->writeFarmdat(aFarm->getOutputDirectoryName(), pig, dairy, beef);
		  delete aFarm;
	   }
   }
//      cout << endl;
   //if (opFile->eof())
	 //  cout << "end of file";
   opFile->close();
   delete opFile;
   delete theMessage;
}

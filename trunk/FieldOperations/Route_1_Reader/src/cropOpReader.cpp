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
#include "fieldOp.h"
#include "fieldOpTill.h"
#include "fieldOpSow.h"
#include "fieldOpFert.h"
#include "fieldOpHarv.h"
#include "fieldOpGrass.h"
#include "fieldOpCut.h"
#include "unix_util.h"


//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;
//!If true, detailed output is sent to the screen
bool verbosity = false;

//!main function for Route 1 file reader
int main(int argc,char *argv[])
{
#ifdef __WINDOWS__
	   //char inputDir[] = "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\OutputFiles\\";
	char inputDir[] = "c:\\Home\\Projects\\NitroEurope\\C5\\Field_operations\\CropManagement\\test\\";
#else
   char inputDir[] = "/home/njh/workspace/InputFiles/Route_1_Reader/";
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
   int NCUcount=0;
   int NumberOfNoOps = 0;
   int NCU =0;
   int cropSeries =0;
   int oldCropSeries=-1;
   int oldYear=0;
   bool finished = false;  //only here because with the Borland compiler, is_open does not want to return false at end of file
#ifdef __BCplusplus__
   while ((opFile)&&(!finished))
#else
   while ((!opFile->eof())&&(!finished))
#endif
   {
	   *opFile >> NCU >> cropSeries;
      cout << "NCU " << NCU << " series " << cropSeries << " ";
      NCUcount++;
      int numOps =0;
      int year =0;
      *opFile >> year >> numOps;
      cout << year  << endl;
      //this checks if the end of file has been reached (should not be necessary but sometimes is, depending on compiler implementation of eof() function)
      if (year==0)
    	  finished=true;
      if (numOps==0)
    	  NumberOfNoOps++;
      if (cropSeries!=oldCropSeries)
      {
    	  oldCropSeries=cropSeries;
    	  oldYear=year;
      }
      else
      {
    	  if (year!=oldYear+1)
    	  {
    		  cout << "Error - NCU " << NCU << " crop series " << cropSeries << " year " << year << endl;
    	      theMessage->FatalError("Years are not in sequence");
    	  }
    	  else
    		  oldYear=year;
      }
      for (int i=0;i< numOps; i++)
      {
         int julianDay=0;
         int opCode=0;
         fieldOp * afieldOp;
         *opFile >> julianDay >> opCode;
         //! On basis of operation code (opCode), initialise an instance of the appropriate field operation class
         switch (opCode)
         {
         case 1: //tillage
		      afieldOp = new fieldOpTill();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         case 2://sowing
		      afieldOp = new fieldOpSow();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         case 3://fertilisation and manuring
		      afieldOp = new fieldOpFert();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         case 4://cutting
		      afieldOp = new fieldOpCut();
		      //afieldOp = new fieldOpGrass();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         case 5://start grazing
		      afieldOp = new fieldOpGrass(5);
		      afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
            break;
         case 6://stop grazing
		      afieldOp = new fieldOpGrass(6);
		      afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
            break;
         case 9://harvesting
		      afieldOp = new fieldOpHarv();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         case 10://atmospheric N deposition
		      afieldOp = new fieldOpFert();
		      afieldOp->ReadOpResults(opFile,year,julianDay);
            break;
         default:
         	char buf[2];
            itoa(opCode,buf,10);
            cout << "Operation number " << i << endl;
         	theMessage->FatalError("Incorrect or unprogrammed operation code ", buf);
            break;
         }
         //afieldOp->CheckContent();
         if (verbosity)
			cout << "Op no " << i << " Op name " << afieldOp->getname() << " Op code " << afieldOp->getopCode() << " Day " << julianDay << endl;
		delete afieldOp;
      }
//      cout << endl;
      getLineFile(opFile);
   }
   //if (opFile->eof())
	 //  cout << "end of file";
   opFile->close();
   delete opFile;
   delete theMessage;
   cout << "Finished after " << NCUcount << " NCUs" << " No of NCU without ops " << NumberOfNoOps << endl;
}

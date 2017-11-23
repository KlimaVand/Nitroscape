

// Class: modelControlClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "modelControlClass.h"
#include "NCUClass.h"
#include "climateNCU_MARS_ATEAM.05.Extract_Data.Func.V2.h"
#include "unix_util.h"
#include <stdlib.h>
#include "message.h"
#ifdef __ANSICPP__
	#include "nixcbits.h"
#endif

extern int getMARS(int hsmuID,char Cmars[256]);

modelControlClass::modelControlClass ( )
{
	theNCUs = new linkList<NCUClass>;
};

modelControlClass:: ~ modelControlClass ( )
{
	delete theNCUs;
};

//obsolete
void modelControlClass::Initialise( int startDay,  int startYear,  int endDay,  int endYear,  int maxNCUs, int baseYear, int range,
	int startNCU,  int endNCU, char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir, fstream* infile,
   bool bootstrap )
{
	bool keepGoing = true;
	int ncu=startNCU;
	int count = 0;
	while ((keepGoing))//&&(ncu<(endNCU+1))&& (count<=maxNCUs))
	{
		int retCode=1; // NCUClass::Initialise will set this to 0 if the NOCR code is found (no cropping)
		NCUClass * aNCUClass = new NCUClass();
		aNCUClass->Initialise(inputDir);
		retCode=aNCUClass->ReadData(&ncu, startDay,  startYear,  endDay,  endYear,  baseYear, range,
			metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, infile, bootstrap);

		char tempChar[10];
		string outString;
		if (retCode==1)
		{
//			if (verbosity)
         {
            cout << "Count " << count << " Reading " << ncu << endl;
            itoa(ncu,tempChar,10);
            outString = "Reading ";
            outString.append(tempChar);
            outString.append(" NCUI ");
            itoa(aNCUClass->getNCUI(),tempChar,10);
            outString.append(tempChar);
            theMessage->LogEvent(outString.c_str());
         }
         theNCUs->InsertLast(aNCUClass);
		}
	  else
	  {
		cout << "Skipping " << ncu << endl;
		itoa(ncu,tempChar,10);
		outString = "Skipping " + *tempChar;
		outString.append(" NCUI ");
		itoa(aNCUClass->getNCUI(),tempChar,10);
		outString.append(tempChar);
		theMessage->LogEvent(outString.c_str());
		delete aNCUClass;
	  }
	  if ((retCode==2)|| (count==maxNCUs))
		keepGoing=false;
	  else
	  {
		 count++;
//		 ncu++;
	  }
   }
};

void modelControlClass::calcCropManagement(char *inputDir, char *outputDir, char *outfileName)
{
   char inputFileName[256];
   strcpy(inputFileName,inputDir);
   strcat(inputFileName,"NCU_MARS.txt");

   char outputFileName[256];
   strcpy(outputFileName,outputDir);
   strcat(outputFileName,outfileName);
   fstream *outfile = new fstream();
	cout << "Output file name " << outputFileName << endl;
   string outString = "Output file name ";
   outString.append(outputFileName);
   theMessage->LogEvent(outString.c_str());
   outfile->open(outputFileName,fstream::out);

   int i=0;
	while (i<theNCUs->NumOfNodes())
   {
   	NCUClass *aNCUClass =theNCUs->ElementAtNumber(i);

   	cout << "Processing " << i << " NCUI " << aNCUClass->getNCUI();
		char tempChar[10];
      itoa(i,tempChar,10);
      outString = "Processing ";
      outString.append(tempChar);
      outString.append(" NCUI ");
      itoa(aNCUClass->getNCUI(),tempChar,10);
      outString.append(tempChar);
      theMessage->LogEvent(outString.c_str());
      aNCUClass->setMARS(getMARS(aNCUClass->getNCUI(),inputFileName));
      aNCUClass->calcCropManagement(inputDir, outputDir);
      aNCUClass->generateOps();
      aNCUClass->outputOps(outfile);
      i++;
      cout << endl;
   }
	outfile->close();
	delete outfile;
};

//obsolete
void modelControlClass::closeDown()
{};

void modelControlClass::processCropManagement( int startDay,  int startYear, int endDay,  int endYear,  int maxNCUs, int baseYear,
 int range,	int startNCU,  int endNCU, char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir,
 char *outputDir, fstream* infile, char *outfileName, bool bootstrap, bool verbosity)
{
   char inputFileName[256];
   strcpy(inputFileName,inputDir);
   strcat(inputFileName,"NCU_MARS.txt");

   char outputFileName[256];
   strcpy(outputFileName,outputDir);
   strcat(outputFileName,outfileName);
   fstream *outfile = new fstream();
	cout << "Output file name " << outputFileName << endl;
   string outString = "Output file name ";
   outString.append(outputFileName);
   theMessage->LogEvent(outString.c_str());
   outfile->open(outputFileName,fstream::out);

#ifdef __METOUT__
   char outputFileNameMet[256];
   strcpy(outputFileNameMet,outputDir);
   strcat(outputFileNameMet,"metdata.txt");
   fstream *outfileMet = new fstream();
   outfileMet->open(outputFileNameMet,fstream::out);
#endif
    char tempChar[10];
	bool keepGoing = true;
	int ncu=startNCU;
	int count = 0;
	while ((keepGoing)&& (count<=maxNCUs))
	{
      int retCode=1; // NCUClass::Initialise will set this to 0 if the NOCR code is found (no cropping)
      NCUClass * aNCUClass = new NCUClass();
		aNCUClass->Initialise(inputDir);
      retCode=aNCUClass->ReadData(&ncu, startDay,  startYear,  endDay,  endYear,  baseYear, range,
         metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, infile, bootstrap);

      if (retCode==1)
      {
      //			if (verbosity)
         {
            cout << "Count " << count << " Reading " << ncu << endl;
            if (verbosity)
            {
            	outString.clear();
				itoa(ncu,tempChar,10);
				outString = "Reading ";
				outString.append(tempChar);
				outString.append(" NCUI ");
				itoa(aNCUClass->getNCUI(),tempChar,10);
				outString.append(tempChar);
				theMessage->LogEvent(outString.c_str());
            }
         }
//         theNCUs->InsertLast(aNCUClass);
      }
	   else
	   {
         cout << "Skipping " << ncu << endl;
         itoa(ncu,tempChar,10);
     	 outString.clear();
         outString.append("Skipping ncu ");
         outString.append(tempChar);
         outString.append(" NCUI ");
         itoa(aNCUClass->getNCUI(),tempChar,10);
         outString.append(tempChar);
         theMessage->LogEvent(outString.c_str());
         delete aNCUClass;
	   }
	   if (retCode==2)
		 keepGoing=false;
	   else
	   {
         cout << "Processing " << count << " NCUI " << aNCUClass->getNCUI() << endl;
         if (verbosity)
         {
			 itoa(count,tempChar,10);
         	 outString.clear();
         	 outString = "Processing ";
			 outString.append(tempChar);
			 outString.append(" NCUI ");
			 itoa(aNCUClass->getNCUI(),tempChar,10);
			 outString.append(tempChar);
			 theMessage->LogEvent(outString.c_str());
         }

         aNCUClass->setMARS(getMARS(aNCUClass->getNCUI(),inputFileName));
         aNCUClass->calcCropManagement(inputDir, outputDir);
         aNCUClass->generateOps();
         aNCUClass->outputOps(outfile);
#ifdef __METOUT__
		 aNCUClass->writeAnnualMet(outfileMet);
#endif
         delete aNCUClass;
         count++;
         ncu++;
	  }
   }
	cout << "Finished after " << count << " NCUs" << endl;
	itoa(count,tempChar,10);
	outString.clear();
	outString = "Finished after  ";
	outString.append(tempChar);
	outString.append(" NCUs ");
	theMessage->LogEvent(outString.c_str());
	outfile->close();
	delete outfile;
#ifdef __METOUT__
	outfileMet->close();
	delete outfileMet;
#endif
}


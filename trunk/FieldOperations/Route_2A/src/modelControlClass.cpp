

// Class: modelControlClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "modelControlClass.h"
#include "NCUClass.h"
#include "climateNCU_MARS_ATEAM.05.Extract_Data.Func.V2.h"
#include "base\unix_util.h"
#include <stdlib.h>
#include "base\message.h"
#ifdef __ANSICPP__
	#include "base\nixcbits.h"
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
void modelControlClass::closeDown()
{};

void modelControlClass::processCropManagement( int startDay,  int startYear, int endDay,  int endYear,  int maxNCUs, int numFields, int baseYear,
 int range,	int startNCU,  int endNCU, string metdataInputDir, string tpvStem, string radStem, string ncu_mars, string inputDir,
 string outputDir, fstream* infile, string outfileName, bool bootstrap, bool verbosity, bool usezeroTillage, bool usecoverCropping)
{
   string inputFileName;
   inputFileName = inputDir;
   inputFileName+="NCU_MARS.txt";

   string outputFileName;
   outputFileName=outputDir;
   outputFileName+=outfileName;
   fstream *outfile = new fstream();
	cout << "Output file name " << outputFileName << endl;
   string outString = "Output file name ";
   outString.append(outputFileName);
   theMessage->LogEvent(outString.c_str());
   outfile->open(outputFileName.c_str(),fstream::out);
/*#ifdef __BCplusplus__
   if (!outfile)
#else
*/
   if (!outfile->is_open())
	   theMessage->FatalError("modelControlClass::processCropManagement could not open output file ", outputFileName.c_str());

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
      aNCUClass->Initialise(inputDir, numFields);
      retCode=aNCUClass->ReadData(&ncu, startDay,  startYear,  endDay,  endYear,  baseYear, range,
         metdataInputDir, tpvStem, radStem, ncu_mars, inputDir, infile, bootstrap);

      aNCUClass->setcoverCropping(usecoverCropping);
      aNCUClass->setzeroTillage(usezeroTillage);

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
         char *tempstring;
         strcpy(tempstring,inputFileName.c_str());
         aNCUClass->setMARS(getMARS(aNCUClass->getNCUI(),tempstring));
         aNCUClass->calcCropManagement(inputDir, outputDir,startYear);
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


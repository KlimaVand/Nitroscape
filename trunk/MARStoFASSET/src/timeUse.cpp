/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "timeUse.h"
#include "bstime.h"
#include "message.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
timeUse::timeUse(char *outputDir)
{
   typedef char string100[100];
	string100 outfileName;
   strcpy(outfileName,outputDir);
   for (int i=0; i<MaxNumberOftimeUse; i++)
   {
   	startTime[i] = clock_t(0);
    	usedTime[i]  = clock_t(0);
      calls[i] = 0;
   }
#ifdef __BCplusplus__
   strcat(outfileName,"timeUse.txt");

   f.open(outfileName,ios::out);
#else
   f.open(outfileName,ios::out);
#endif
   f.precision(4);
}


/****************************************************************************\
  Destructor
\****************************************************************************/
timeUse::~timeUse()
{
	f.close();
}

void timeUse::startProfiling(int profileNumber)
{
	if (useProfiling)
   {
      startTime[profileNumber] = clock();
      calls[profileNumber]++;
	}
}

void timeUse::endProfiling(int profileNumber)
{
	if (useProfiling)
   {
      usedTime[profileNumber] += clock()-startTime[profileNumber];
      lastUsedTime[profileNumber]=clock()-startTime[profileNumber];
	}
}

void timeUse::outputtimeUse()
{
	if (useProfiling)
   {
   f << "------------------------------------------------------------------" << endl;
   f << "                                " << theTime.GetYear() << endl;
   f << "------------------------------------------------------------------" << endl;

	for (int i=0; i<MaxNumberOftimeUse; i++)
   {
   	if (usedTime[i]>0.0)
      {
      	string outputString;
         switch (i)
         {
				case Mark1: outputString          = "Mark1    "; break;
				case Mark2: outputString          = "Mark2    "; break;
				case Mark3: outputString          = "Mark3    "; break;
				case Mark4: outputString          = "Mark4    "; break;
				case Mark5: outputString          = "Mark5    "; break;
            default: outputString                         = "Unknown profile: "; break;
         }
         char out[80];
#ifdef __ANSICPP__
         float timeUse = usedTime[i]/double(CLOCKS_PER_SEC);
#else
         float timeUse = usedTime[i]/CLK_TCK;
#endif
         if (calls[i]==0)
            timeUse=0.0;
         sprintf(out, "%-30s  calls: %7i  time: %7.1f ", (char*) outputString.c_str(),calls[i],timeUse );

         f << out << endl;
		}
      startTime[i] = clock_t(0);
      usedTime[i]  = clock_t(0);
      calls[i] = 0;
   }
   }
}


void timeUse::getLastUsedTime()
{
	if (useProfiling)
   {

	for (int i=0; i<MaxNumberOftimeUse; i++)
   {
     	string outputString;
   	if (lastUsedTime[i]>0.0)
      {
         switch (i)
         {
				case Mark1: outputString          = " Mark1 "; break;
				case Mark2: outputString          = " Mark2 "; break;
				case Mark3: outputString          = " Mark3 "; break;
				case Mark4: outputString          = " Mark4 "; break;
				case Mark5: outputString          = " Mark5 "; break;
            default: outputString                         = "Unknown profile: "; break;
         }
         char out[80];
#ifdef __ANSICPP__
         float timeUsed = lastUsedTime[i]/double(CLOCKS_PER_SEC);
#else
         float timeUsed = lastUsedTime[i]/CLK_TCK;
#endif
         cout << outputString << timeUsed;
		}
	}
   }
};



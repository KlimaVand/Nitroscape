/*
 * timeUsed.h
 *
 *  Created on: 27/01/2010
 *      Author: beer
 */

#ifndef TIMEUSED_H_
#define TIMEUSED_H_

#ifdef __ANSICPP__
	#include <ctime>
#else
	#include <time.h>
#endif
using namespace std;
enum {
   Mark1,
   Mark2,
   Mark3,
   Mark4,
   Mark5
};

class timeUse
{
   private:
   	enum {MaxNumberOftimeUse=10};

      fstream f;
      int calls[MaxNumberOftimeUse];
      clock_t startTime[MaxNumberOftimeUse];
      clock_t usedTime[MaxNumberOftimeUse];
      clock_t lastUsedTime[MaxNumberOftimeUse];
      bool useProfiling;

   public:
      timeUse(char *outputDir);
      ~timeUse();
      void startProfiling(int timeUseNumber);
      void endProfiling(int timeUseNumber);
      void outputtimeUse();
		void getLastUsedTime();
      void setUseProfiling(bool aVal){useProfiling=aVal;}
};

extern timeUse * timeConsumption;

#endif


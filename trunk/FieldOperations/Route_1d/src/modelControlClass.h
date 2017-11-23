// Class: modelControlClass                     //ANSI C++

#include "common.h"                 

#ifndef __MODELCONTROLCLASS_H  //Required for current class
  #define __MODELCONTROLCLASS_H

                              //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include "linklist.h"        //Required for include files

#include "bstime.h"        //Required for include files
                               
                               //Required for 1:M aggregation (part) classes
#ifndef __NCUCLASS_H
  #include "NCUClass.h"
#endif

class modelControlClass  : public base        
{                       
			
     bsTime* startTime; //Attribute data member
   		
     bsTime* endTime; //Attribute data member
   		
     fstream* outputDetailsFile; //Attribute data member
   		
     fstream* outputFile; //Attribute data member        

   linkList<NCUClass> *theNCUs; //1:M aggregation part data member 

private:

protected:

public:

   modelControlClass ();

	void Initialise(int startDay,  int startYear,  int endDay, int maxNCUs, int endYear,  int baseYear, int range,
		int startNCU,  int endNCU, char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir, fstream* infile,
         bool bootstrap);
	void calcCropManagement(char *inputDir, char *outputDir, char *outfileName);
   void closeDown ()    ;
	void processCropManagement( int startDay,  int startYear, int endDay,  int endYear,  int maxNCUs, int baseYear,
			 int range,	int startNCU,  int endNCU, char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir,
			 char *outputDir, fstream* infile, char *outfileName, bool bootstrap, bool verbosity);

   ~ modelControlClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  bsTime* getstartTime() const  { return startTime;}

                       //!Get accessor function for non-static attribute data member
  bsTime* getendTime() const  { return endTime;}

                       //!Get accessor function for non-static attribute data member
  fstream* getoutputDetailsFile() const  { return outputDetailsFile;}

                       //!Get accessor function for non-static attribute data member
  fstream* getoutputFile() const  { return outputFile;}
                       
                       //!Set accessor function for non-static attribute data member
  void setstartTime (bsTime* astartTime)  { startTime = astartTime;}

                       //!Set accessor function for non-static attribute data member
  void setendTime (bsTime* aendTime)  { endTime = aendTime;}

                       //!Set accessor function for non-static attribute data member
  void setoutputDetailsFile (fstream* aoutputDetailsFile)  { outputDetailsFile = aoutputDetailsFile;}

                       //!Set accessor function for non-static attribute data member
  void setoutputFile (fstream* aoutputFile)  { outputFile = aoutputFile;}
       
                       //!Get accessor function for 1:many aggregation object data member
  linkList <NCUClass>* gettheNCUs()  { return theNCUs;  }
                       
};

#endif

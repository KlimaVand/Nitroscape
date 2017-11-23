

// Class: NCUCropClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "NCUCropClass.h"
#include "message.h"
#include "fieldOp.h"
#include "fieldOpFert.h"
#include "fieldOpSow.h"
#include "fieldOpTill.h"

extern bool verbosity;

NCUCropClass:: ~ NCUCropClass ( )
{

};

void NCUCropClass::Initialise()
{
     UreaFertiliser = 0.0;
     NitrateFertiliser = 0.0;
     CattleSlurry= 0.0;;
     CattleSolid= 0.0;
     PigSlurry= 0.0;
     PigSolid= 0.0;
     PoultrySlurry= 0.0;
     PoultrySolid= 0.0;
     Grazing = 0.0;
     AtmosphericNdep=0.0;
     CropName ="";
     WinterCrop=false;

	  for (int i=0; i<numberFertTypes;i++)
     	FertDay[i]=0;
	  for (int i=0; i<5;i++)
     	SowingDay[i]=0;
	  for (int i=0; i<5;i++)
     	HarvestingDay[i]=0;
	  for (int i=0; i<5;i++)
     	PloughingDay[i]=0;
	  for (int i=0; i<5;i++)
     	HarrowingDay[i]=0;
	  for (int i=0; i<5;i++)
     	GrazingStartDay[i]=0;
	  for (int i=0; i<5;i++)
     	GrazingStopDay[i]=0;
	  for (int i=0; i<5;i++)
		  CuttingDay[i]=0;

	  for (int i=0; i<numberManureTypes;i++)
     	ManureDay[i]=0;

}

#ifdef __BCplusplus__	   
	bool NCUCropClass::ReadData(istrstream *instring, int row)
#else
	bool NCUCropClass::ReadData(stringstream *instring, int row)
#endif

{
	bool abort = false;
   double indata;
	string aString;
   if (row==0)
   {
      *instring >> aString;
      if (aString.length()==0)
         theMessage->FatalError("NCUCropClass: error in reading crop rotation data");
      else
      	if (verbosity)
#ifdef __BCplusplus__
      	cout << aString << " ";
#else
    	cout << aString << " " << flush;
#endif
 	}
   else
   {
	   *instring >>indata;
	    if (verbosity)
#ifdef __BCplusplus__
			cout << indata << " ";
#else
		cout << indata << " " << flush;
#endif
   }
   switch (row) {
   case 0:  if (aString == "NOCR")
               abort = true;
            if ((aString == "WWHE")||(aString == "RAPE"))
               setWinterCrop(true);
            setCropName(aString.c_str());
            break;
   case 1: setNitrateFertiliser(indata);
            break;
   case 2: setUreaFertiliser(indata);
            break;
   case 3: setCattleSlurry(indata);
            break;
   case 4: setCattleSolid(indata);
            break;
   case 5: setPigSlurry(indata);
            break;
   case 6: setPigSolid(indata);
            break;
   case 7: setPoultrySolid(indata);
            break;
   case 8: setGrazing(indata);
            break;
   case 9: setCattleSlurryC(indata);
            break;
   case 10: setCattleSolidC(indata);
            break;
   case 11: setPigSlurryC(indata);
            break;
   case 12: setPigSolidC(indata);
            break;
   case 13: setPoultrySolidC(indata);
            break;
   case 14: setGrazingC(indata);
            break;
   case 15: setAtmosphericNdep(indata);
   			break;
   default: theMessage->FatalError("NCUCropClass: error in reading crop rotation data");
   }
	return abort;
}



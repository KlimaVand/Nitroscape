

// Class: NCUCropClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "NCUCropClass.h"
#include "base\message.h"
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
     CattleSlurryC= 0.0;;
     CattleSolidC= 0.0;
     PigSlurryC= 0.0;
     PigSolidC= 0.0;
     PoultrySlurryC= 0.0;
     PoultrySolidC= 0.0;
     Grazing = 0.0;
     AtmosphericNdep=0.0;
     CropName ="";
     WinterCrop=false;

	  for (int i=0; i<numberFertTypes;i++)
		  for (int j=0; j<4;j++)
			  FertDay[i][j]=0;
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
		  for (int j=0; j<4;j++)
			  ManureDay[i][j]=0;
	  propFert[0]=0.2;
	  propFert[1]=0.8;

	  numLeadSpaces=0;
}

#ifdef __BCplusplus__	   
	bool NCUCropClass::ReadData(istrstream *instring, int row)
#else
	bool NCUCropClass::ReadData(stringstream *instring, int row, bool first)
#endif

{
		bool abort = false;
		double indata;
		string aString;
		char str[20]="";
		if (first)
		{
			numLeadSpaces=2;
		}
		else
			numLeadSpaces=3;
		if (row==0)
		{
/*			if (numLeadSpaces==0)
			{
				bool gotit=false;
				char tmpStr[2];
				char spaceCh[] = " ";
				while (!gotit)
				{
					instring->get(tmpStr,2);
					strcat(str,tmpStr);
					cout << tmpStr << endl;
					if (strcmp(tmpStr,spaceCh)==0)
					{
						numLeadSpaces++;
					}
					else
						gotit=true;
				}
				for (int i=0; i<3; i++)
				{
					instring->get(tmpStr,2);
					cout << tmpStr << endl;
					strcat(str,tmpStr);
				}
				aString.assign(str);
			}
			else*/
			{
				instring->get(str,numLeadSpaces+5);
				aString.assign(str);
			}
			if (aString.length()==0)
				theMessage->FatalError("NCUCropClass: error in reading crop rotation data");
			else
				//erase the leading spaces
				aString.erase(0,numLeadSpaces);
			if (verbosity)
#ifdef __BCplusplus__
				cout << aString << " ";
#else
			cout << aString << endl;
#endif
		}
		else
		{
			*instring >>indata;
			if (verbosity)
#ifdef __BCplusplus__
				cout << indata << " ";
#else
			cout << indata << " " << endl;
#endif
		}
		switch (row) {
		case 0:  if (aString == "NOCR")
			abort = true;
		if ((aString == "WWHE")||(aString == "RAPE"))
			setWinterCrop(true);
		if (!aString.length()==4)
			theMessage->FatalError("Crop name not 4 characters ",aString.c_str());
		setCropName(aString);
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

int NCUCropClass::getNumManureApps(int index)
{
	int retVal=0;
	for (int i=0; i<=4; i++)
		if (ManureDay[index][i]>0.0)
			retVal++;
	return retVal;
}
int NCUCropClass::getNumFertApps(int index)
{
	int retVal=0;
	for (int i=0; i<=4; i++)
		if (FertDay[index][i]>0.0)
			retVal++;
	return retVal;
}


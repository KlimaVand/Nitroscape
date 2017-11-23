
 
// Class: fieldOpGrass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpGrass.h"   
#include "message.h"

fieldOpGrass::fieldOpGrass (int anOpCode)
{
   switch (anOpCode)
   {
   case 5: name = "Start grazing";
		   break;
   case 6: name = "Stop grazing";
		   break;
   default:		theMessage->FatalError("Input operation code does not match grazing operation code");
   }
	opCode =anOpCode;

}

fieldOpGrass:: ~ fieldOpGrass ( )
{

};

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param Month month
\param Day day
\param LAI leaf area index after grazing
*/

void fieldOpGrass::DefineOp(char * aName, int aNumber, int Year, int Month, int Day, double LAI)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
   harvLAI=LAI;
   name = "Start grazing";
   opCode = 5;
}

void fieldOpGrass::DefineOp(char * aName, int aNumber, int Year, int JulianDay, double LAI)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
   harvLAI=LAI;
   name = "Start grazing";
   opCode = 5;
}
void fieldOpGrass::DefineOp(char * aName, int aNumber, int Year, int Month, int Day)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
   name = "Stop grazing";
   opCode = 6;
}

void fieldOpGrass::DefineOp(char * aName, int aNumber, int Year, int JulianDay)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
	name = "Stop grazing";
   opCode = 6;
}

void fieldOpGrass::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
	int numDumint;
	if (opCode==5)
	{
		*outfile << harvLAI << "\t" ;
		if (verbosePrint)
			cout << harvLAI  << "\t";
		numDumint=5;
	}
	else
		numDumint=6;

	for (int i=0; i<numDumint; i++)
	{
		*outfile << "-999" << "\t";
      if (verbosePrint)
			cout << "-999" << "\t";
	}
}
void fieldOpGrass::ReadOpResults(ifstream * infile, int theYear, int theJulianDay, int anOpCode)
{
   fieldOp::ReadOpResults(infile, theYear, theJulianDay, anOpCode);
   int numDumint;
   if (opCode==5)
	{
	   *infile >> harvLAI;
	   numDumint=5;
	}
   else
	   numDumint=6;
   int dumint;
   for (int i=0; i<numDumint; i++)
		*infile >> dumint;
   switch (anOpCode)
   {
   case 5: name = "Start grazing";
		   break;
   case 6: name = "Stop grazing";
		   break;
   default:		theMessage->FatalError("Input operation code does not match grazing operation code");
   }
}



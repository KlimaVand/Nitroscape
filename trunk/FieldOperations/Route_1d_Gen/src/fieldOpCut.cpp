
 
// Class: fieldOpCut

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpCut.h"

fieldOpCut:: ~ fieldOpCut ( )
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

void fieldOpCut::DefineOp(char * aName, int aNumber, int Year, int Month, int Day)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
}

void fieldOpCut::DefineOp(char * aName, int aNumber, int Year, int JulianDay)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
}

void fieldOpCut::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
	for (int i=0; i<6; i++)
	{
		*outfile << "-999" << "\t";
      if (verbosePrint)
			cout << "-999" << "\t";
	}
}
void fieldOpCut::ReadOpResults(ifstream * infile, int theYear, int theJulianDay)
{
   fieldOp::ReadOpResults(infile, theYear, theJulianDay);
   int dumint;
	for (int i=0; i<6; i++)
		*infile >> dumint;
   name = "Cutting";
}



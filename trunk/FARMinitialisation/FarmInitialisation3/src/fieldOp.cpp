

// Class: fieldOp

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOp.h"

fieldOp::fieldOp()
{
	Initialise();
}

fieldOp:: ~ fieldOp ( )
{
	delete theOptime;
};

void fieldOp::DefineOp(char * aName, int aopCode, int Year, int Month, int Day)
{
 name=aName;
 theOptime->SetTime(Day, Month, Year);
 opCode = aopCode;
}

void fieldOp::DefineOp(char * aName, int aopCode, int Year, int JulianDay)
{
 name=aName;
 theOptime->SetTime(Year,JulianDay);
 opCode = aopCode;
}
//! Initialise an instance
void fieldOp::Initialise()
{
 theOptime = new bsTime();
 name = "No operation" ;
}

void fieldOp::outputOps(fstream * outfile, int status, bool printYear)
{
	if (printYear)
	{
		*outfile << theOptime->GetYear() << "\t" ;
		if(verbosity)
		{
			cout << theOptime->GetYear() << "\t";
		}
	}
	*outfile << theOptime->GetJulianDay() << "\t" << opCode << "\t" << status;
	cout << theOptime->GetJulianDay() << "\t" << opCode << "\t" << status;
}
//! Tidy up
void fieldOp::closeDown()
{
   delete theOptime;
}

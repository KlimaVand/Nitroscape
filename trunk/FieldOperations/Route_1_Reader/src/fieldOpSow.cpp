
 
// Class: fieldOpSow

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpSow.h"

fieldOpSow:: ~ fieldOpSow ( )
{

};

void fieldOpSow::DefineOp(char * aName, int aNumber, int Year, int Month, int Day, string aplantName)
{
	fieldOp::DefineOp(aName, aNumber, Year, Month, Day);
   plantName=aplantName;
   amountSeed=GetamountSeed();
}
void fieldOpSow::DefineOp(char * aName, int aNumber, int Year, int JulianDay, string aplantName)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
   plantName=aplantName;
   amountSeed=GetamountSeed();
}

//! Tidy up
void fieldOpSow::closeDown()
{
   fieldOp::closeDown();
}
void fieldOpSow::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
	*outfile << plantName.c_str() << "\t"  << "5.0" << "\t" ;
	if (verbosePrint)
		cout << plantName.c_str() << "\t"  << "5.0" << "\t" ; // "5.0" is a default amount of seed (kg/ha)
	for (int i=0; i<4; i++)
	{
		*outfile << "-999" << "\t";
      if (verbosePrint)
			cout << "-999" << "\t";
	}
}

void fieldOpSow::outputFnnOps(ofstream *outfile, int number)
{
	char Num[3];
	sprintf(Num,"%d",number);
	*outfile << "SowCrop(" << Num << ")" << "\t" << plantName.c_str() << endl;
	*outfile << "SowDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	*outfile << "SowAmount(" << Num << ")" << "\t" << "5.0" << endl;
}

void fieldOpSow::ReadOpResults(ifstream * infile, int theYear, int theJulianDay)
{
   fieldOp::ReadOpResults(infile, theYear, theJulianDay);
   *infile >> plantName >> amountSeed;
   int dumint;
	for (int i=0; i<4; i++)
		*infile >> dumint;
   name = "Sowing";
}

float fieldOpSow::GetamountSeed()
{
	return 5.0;
}


 
// Class: fieldOpTill

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpTill.h"   

fieldOpTill:: ~ fieldOpTill ( )
{

};

void fieldOpTill::DefineOp(char * aName, int aNumber, int Year, int Month, int Day, float tillageDepth)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
   depth=tillageDepth;
}

void fieldOpTill::DefineOp(char * aName, int aNumber, int Year, int JulianDay, float tillageDepth)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
   depth=tillageDepth;
}


void fieldOpTill::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);

	*outfile << "\t" << depth;
/*	if(verbosity)
	{
		cout << "\t" << depth;
	}
	for (int i=0; i<5; i++)
	{
		*outfile << "\t" << "-999";
		cout << "\t" << "-999";
	}*/
}

void fieldOpTill::outputFnnOps(ofstream *outfile, int number)
{
	char Num[3];
	sprintf(Num,"%d",number);
	*outfile << "TillageDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	if(verbosity)
	{
		cout << "TillageDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	}
	if (depth>100.0)
	{
		*outfile << "TillageType(" << Num << ")" << "\t" << "PLOUGH" << endl;
		*outfile << "TillageIncorporation(" << Num << ")" << "\t" << 0.5 << endl;
	}
	else
	{
		*outfile << "TillageType(" << Num << ")" << "\t" << "HARROW" << endl;
		*outfile << "TillageIncorporation(" << Num << ")" << "\t" << 0.5 << endl;
	}
	*outfile << "TillageDepth(" << Num << ")" << "\t" << depth << endl;
}

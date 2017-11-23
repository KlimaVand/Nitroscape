
 
// Class: fieldOpTill

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpHarv.h"

fieldOpHarv:: ~ fieldOpHarv ( )
{

};

void fieldOpHarv::DefineOp(char * aName, int aNumber, int Year, int Month, int Day, bool CollectStraw)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
   collectStraw=CollectStraw;
}

void fieldOpHarv::DefineOp(char * aName, int aNumber, int Year, int JulianDay, bool CollectStraw)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
   collectStraw=CollectStraw;
}


void fieldOpHarv::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
/*	*outfile << "\t" << collectStraw;
	cout << "\t" << collectStraw;
	for (int i=0; i<5; i++)
	{
		*outfile << "\t" << "-999";
		cout << "\t" << "-999";
	}*/
}

void fieldOpHarv::outputFnnOps(ofstream *outfile, int number)
{
	char Num[3];
	sprintf(Num,"%d",number);
	*outfile << "HarvestDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	*outfile << "HarvestRemoveStraw(" << Num << ")" << "\t" << collectStraw << endl;
	if(verbosity)
	{
		cout << "HarvestDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
				gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
		cout << "HarvestRemoveStraw(" << Num << ")" << "\t" << collectStraw << endl;
	}
}


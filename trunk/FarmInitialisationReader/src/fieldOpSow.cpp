
 
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
}
void fieldOpSow::DefineOp(char * aName, int aNumber, int Year, int JulianDay, string aplantName)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
   plantName=aplantName;
}

//! Tidy up
void fieldOpSow::closeDown()
{
   fieldOp::closeDown();
}
void fieldOpSow::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
/*	if (outfile)
		*outfile << "\t" << plantName.c_str();*/
	cout << "\t" << plantName.c_str();
	*outfile << "\t" << plantName.c_str();
}

void fieldOpSow::outputFnnOps(ofstream *outfile, int number)
{
	char Num[3];
	sprintf(Num,"%d",number);
	*outfile << "SowCrop(" << Num << ")" << "\t" << plantName.c_str() << endl;
	*outfile << "SowDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	*outfile << "SowAmount(" << Num << ")" << "\t" << "5.0" << endl;
	if(verbosity)
	{
		cout << "SowCrop(" << Num << ")" << "\t" << plantName.c_str() << endl;
		cout << "SowDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
				gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
		cout << "SowAmount(" << Num << ")" << "\t" << "5.0" << endl;
	}
}

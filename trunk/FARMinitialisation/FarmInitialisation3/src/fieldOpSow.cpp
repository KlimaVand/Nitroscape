
 
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
	*outfile << "\t" << plantName.c_str() << "\t"  << "5.0";
	cout << "\t" << plantName.c_str() << "\t"  << "5.0"; // "5.0" is a default amount of seed (kg/ha)
	for (int i=0; i<4; i++)
	{
		*outfile << "\t" << "-999";
		cout << "\t" << "-999";
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
	if(verbosity)
	{
		cout << log_info << "SowCrop(" << Num << ")" << "\t" << plantName.c_str() << endl;
		cout << log_info <<"SowDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
				gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
		cout << log_info << "SowAmount(" << Num << ")" << "\t" << "5.0" << endl;
	}
}

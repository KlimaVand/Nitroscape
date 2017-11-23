

// Class: fieldOpFert

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpFert.h"

fieldOpFert:: ~ fieldOpFert ( )
{

};

void fieldOpFert::DefineOp(char * aName, int aNumber, int Year, int Month, int Day, int theType,
   		float theNH4N, float theNO3N, float theOrgN, float theC, int theappMeth)
{
	fieldOp::DefineOp(aName,  aNumber, Year, Month, Day);
	amountNH4N=theNH4N;
   amountNO3N=theNO3N;
   amountOrgN=theOrgN;
   amountC = theC;
   applicMethod=theappMeth;
   type=theType;
}

void fieldOpFert::DefineOp(char * aName, int aNumber, int Year, int JulianDay, int theType,
   		float theNH4N, float theNO3N, float theOrgN, float theC, int theappMeth)
{
	fieldOp::DefineOp(aName,  aNumber, Year, JulianDay);
	amountNH4N=theNH4N;
   amountNO3N=theNO3N;
   amountOrgN=theOrgN;
   amountC = theC;
   applicMethod=theappMeth;
   type=theType;
}

//! Tidy up
void fieldOpFert::closeDown()
{
   fieldOp::closeDown();
}

void fieldOpFert::outputOps(fstream * outfile, int status, bool printYear)
{
	fieldOp::outputOps(outfile, status, printYear);
	*outfile << "\t" << type << "\t" << amountNH4N << "\t" << amountNO3N << "\t" << amountOrgN << "\t" << amountC << "\t" << applicMethod;
	cout << "\t" << type << "\t" << amountNH4N << "\t" << amountNO3N << "\t" << amountOrgN << "\t" << amountC << "\t" << applicMethod;
}

void fieldOpFert::outputFnnOps(ofstream *outfile, int number)
{
	char Num[3];
	float totalN = amountNH4N + amountNO3N + amountOrgN;
	sprintf(Num,"%d",number);
	*outfile << "FertilizerDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
			gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
	*outfile << "FertilizerType(" << Num << ")" << "\t" << "SLURRY" << endl;
	*outfile << "FertilizerN(" << Num << ")" << "\t" << totalN << endl;
	*outfile << "NH4_fraction(" << Num << ")" << "\t" << 0.5 << endl;
	if(verbosity)
	{
		cout << log_info << "FertilizerDate(" << Num << ")" << "\t" << gettheOptime()->GetDay() << "/" <<
				gettheOptime()->GetMonth() << "/" << gettheOptime()->GetYear() << endl;
		cout << log_info << "FertilizerType(" << Num << ")" << "\t" << "SLURRY" << endl;
		cout << log_info << "FertilizerN(" << Num << ")" << "\t" << totalN << endl;
		cout << log_info << "NH4_fraction(" << Num << ")" << "\t" << 0.5 << endl;
	}
}


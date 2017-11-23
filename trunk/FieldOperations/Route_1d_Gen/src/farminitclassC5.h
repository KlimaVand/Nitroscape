#ifndef FARMINITCLASS_H_
#define FARMINITCLASS_H_

#include "common.h"
#include "linklist.h"
#include "cropSeriesClass.h"

using namespace std;

//!Generates initialisation files for an individual farm, using a variety of formats

class farmInitClassC5
{
	fstream fielddat;
	fstream *fieldsfnn;
	linkList<cropSeriesClass> *theFields;
	int NCU;
	char directoryName[300];

public:
	farmInitClassC5(int aNCU);
	virtual ~farmInitClassC5();
   char * getOutputDirectoryName(){return directoryName;};

	//!Generate the FASSET farm data file
	/*!
	 *  @param pig true if the farm is a pig farm
	 *  @param dairy true if the farm is a dairy farm
	 *  @param beef true if the farm as a beef farm
	 */
	void writeFarmdat(char *outputDirectory, bool pig, bool dairy, bool beef);
	//!Generate the FASSET field data file
	void initFielddat(char *outputDirectory);
	//!Writes the field operations to the FASSET field operation files
	/*!
	 * @param startYear first year for which operations should be generated
	 * @param endYear last year for which operations should be generated
	 */
	void writeFieldsfnn(char *outputDirectory);
	//!Initialise a particular FASSET field operations file
	/*!
	 * @param number the number to be appended the file (e.g. "1" for file f01)
	 */
	void initFieldsfnn(int number);
	//!Close the current FASSET field operations file
	void closeFieldsfnn();
	//!Writes the field operations using the NitroEurope C5 format
	void writeFieldOps();

   bool readFieldOps(ifstream *opFile);

   int getFirstYear();
   int getLastYear();
   bool processFieldOps(ifstream *opFile, char *outputDir, int *aNCU);



};

#endif /*FARMINITCLASS_H_*/

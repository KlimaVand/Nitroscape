#ifndef FARMINITCLASS_H_
#define FARMINITCLASS_H_

#include "common.h"
#include "xmlParser.h"
#include "linklist.h"
#include "cropSeriesClass.h"

using namespace std;

//!Generates initialisation files for an individual farm, using a variety of formats

class farmInitClass
{
	ofstream fielddat;
	ofstream *fieldsfnn;
	ofstream *cattlefnn;
	ofstream *pigsfnn;
	linkList<cropSeriesClass> *theFields;
	XMLNode *xNodeFarm;
public:
	farmInitClass();
	virtual ~farmInitClass();
	//!Initialise farmInitClass
	/*!
	 *  @param aNodeFarmpointer to the farm node
	 */
	void initFarm(XMLNode *aNodeFarm);
	//!Generate the FASSET farm data file
	/*!
	 *  @param pig true if the farm is a pig farm
	 *  @param dairy true if the farm is a dairy farm
	 *  @param beef true if the farm as a beef farm
	 */
	void writeFarmdat(bool pig, bool dairy, bool beef);
	//!Generate the FASSET field data file
	void initFielddat();
	//!Read the field operations for the required years
	/*!
	 * @param startYear first year for which operations should be generated
	 * endYear last year for which operations should be generated
	 */
	void readFieldOps(int startYear, int endYear);
	//!Writes the field operations to the FASSET field operation files
	/*!
	 * @param startYear first year for which operations should be generated
	 * @param endYear last year for which operations should be generated
	 */
	void writeFieldsfnn(int startYear, int endYear);
	//!Initialise a particular FASSET field operations file
	/*!
	 * @param number the number to be appended the file (e.g. "1" for file f01)
	 */
	void initFieldsfnn(int number);
	//!Close the current FASSET field operations file
	void closeFieldsfnn();
	//!Writes the field operations using the NitroEurope C5 format
	void writeFieldOps();

	//!Write cattle input file (FASSET format). Returns 1 if file cannot be created, 2 if category not found, otherwise returns 0
	/*!
	 * \param number the number to be appended the file (e.g. "1" for file f01)
	 * \param cattleArr array containing the numbers of different categories of cattle
	 * \param numCattleCats total number of categories of cattle
	 */
	int writeCattlefnn(int number, float cattleArr[], int numCattleCats);
	//!Write pig input file (FASSET format). Returns 1 if file cannot be created, 2 if category not found, otherwise returns 0
	/*!
	 * \param number the number to be appended the file (e.g. "1" for file f01)
	 * \param pigArr array containing the numbers of different categories of pigs
	 * \param numPigCats total number of categories of pigs
	 */
	int writePigsfnn(int number, float pigArr[], int numPigCats);
	//!Detect presence of livestock and writes livestock input files (FASSET format)
	/*!
	 * \param startYear first year for which input files should be created
	 * \param endYear last year for which input files should be created
	 * \param dairyPresent is set true if dairy cattle are present
	 * \param beefPresent is set true if beef cattle are present
	 * \param pigsPresent is set true if pigs are present
	*/
	int writeLivestockfnn(int startYear, int endYear, bool *dairyPresent, bool *beefPresent, bool *pigsPresent);

};

#endif /*FARMINITCLASS_H_*/

#ifndef FARMINITCLASS_H_
#define FARMINITCLASS_H_

#include "base/common.h"
#include "xmlParser.h"
#include "base/linklist.h"
#include "cropSeriesClass.h"
#include "animalHousingClass.h"
#include "manureStorageClass.h"
using namespace std;

enum {	numDairyCats = 4};
enum {numBeefCats = 8};
enum {numCattleCats = 12};
enum {numPigCats = 4};
enum {numPoultryCats = 3};

//!Generates initialisation files for an individual farm, using a variety of formats

class farmInitClass
{
	ofstream fielddat;
	ofstream *fieldsfnn;
	ofstream *cattlefnn;
	ofstream *pigsfnn;
	ofstream *runBuildings;
	linkList<cropSeriesClass> *theFields;
	linkList<animalHousingClass> *theHousing;
	linkList <manureStorageClass> *theManureStorage;
	int farmID;
	float cattleArr[numCattleCats];
	float pigArr[numPigCats];
	float poultryArr[numPoultryCats];
	string outputDirectoryName;

	XMLNode *xNodeFarm;
public:
	farmInitClass();
	virtual ~farmInitClass();
	//!Initialise farmInitClass
	/*!
	 *  @param aNodeFarmpointer to the farm node
	 */
	bool initFarm(XMLNode *aNodeFarm);
	//!Generate the FASSET farm data file
	/*!
	 *  @param pig true if the farm is a pig farm
	 *  @param dairy true if the farm is a dairy farm
	 *  @param beef true if the farm as a beef farm
	 *  @param poultry true if the farm has poultry
	 *  @param isWithoutFields true if the simulation should be run without fields (for debugging)
	 */
	bool writeFarmdat(bool pig, bool dairy, bool beef, bool poultry, bool isWithoutFields);
	//!Generate the FASSET field data file
	bool writeFielddat();
	//!Read the field operations for the required years
	/*!
	 * @param startYear first year for which operations should be generated
	 * endYear last year for which operations should be generated
	 */
	bool readFieldOps(int startYear, int endYear);
	//!Writes the field operations to the FASSET field operation files
	/*!
	 * @param startYear first year for which operations should be generated
	 * @param endYear last year for which operations should be generated
	 */
	bool writeFieldsfnn(int startYear, int endYear);
	//!Initialise a particular FASSET field operations file
	/*!
	 * @param number the number to be appended the file (e.g. "1" for file f01)
	 */
	void initFieldsfnn(int number);
	//!Close the current FASSET field operations file
	void closeFieldsfnn();
	//!Writes the field operations using the NitroEurope C5 format
	bool writeFieldOps();

	//!Write cattle input file (FASSET format). Returns 1 if file cannot be created, 2 if category not found, otherwise returns 0
	/*!
	 * \param cattleArr array containing the numbers of different categories of cattle
	 * \param numCattleCats total number of categories of cattle
	 */
	bool writeCattlefnn(float cattleArr[], int numCattleCats);

	//!Write pigs feed input file (FASSET format). Returns false if file cannot be created.
	bool writePigsfeedfnn();

	//!Write pig input file (FASSET format). Returns 1 if file cannot be created, 2 if category not found, otherwise returns 0
	/*!
	 * \param pigArr array containing the numbers of different categories of pigs
	 * \param numPigCats total number of categories of pigs
	 */
	bool writePigsfnn(float pigArr[], int numPigCats);
	//!Detect presence of livestock and writes livestock input files (FASSET format)
	/*!
	 * \param dairyPresent is set true if dairy cattle are present
	 * \param beefPresent is set true if beef cattle are present
	 * \param pigsPresent is set true if pigs are present
	*/
	bool readLivestock(bool *dairyPresent, bool *beefPresent, bool *pigsPresent, bool *poultryPresent);
	bool writeLivestockfnn(bool dairyPresent, bool beefPresent, bool pigsPresent, bool poultryPresent);
	bool initBuildings();
	bool checkBuildings();
	bool writeBuildings();
	string getoutputDirectoryName(){return outputDirectoryName;}
	void setoutputDirectoryName(string aName) {outputDirectoryName.assign(aName);}
	int getfarmID(){return farmID;}
};

#endif /*FARMINITCLASS_H_*/

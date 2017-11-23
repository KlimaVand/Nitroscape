// Class: NCUClass                     //ANSI C++

#include "base\common.h"
#include "base\linklist.h"
#include "xmlParser.h"
#include <vector>

#ifndef __CROPSERIESCLASS_H
  #define __CROPSERIESCLASS_H

                              //Required for 1:M aggregation (part) classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//!Stores and manages the field operations for an individual field or location
struct cropDetails
{
	int cropID;
	string cropName;
};

class cropSeriesClass
{

private:

	//!Unique identification number for the series
	int seriesNumber;
	//!Area of field
	double fieldArea;
	//! Is true if the data have been read and are complete
	bool isGood;
	//!List of identifiers of number and name for crops grown
	  vector<cropDetails> theCrops;
	//!List of pointers to the operations
   linkList<fieldOp> *theOperations;

protected:

public:

			//constructor
  cropSeriesClass (int theseriesNumber);

   ~ cropSeriesClass ( );  //Destructor - Delete any pointer data members that used new in constructors
   //!Returns a pointer to the list of operations
  linkList<fieldOp> * gettheOperations() {return theOperations;}
  //!Add the operations of this crop series
  /*!
   *  @param fieldNode pointer to the XML field node
   *   @param startYear first year for which operations should be generated
   *    @param endYear last year for which operations should be generated
   */
  bool addOperations(XMLNode *fieldNode,int startYear, int endYear);
  //!Recover the crop related field operations (sowing and harvesting)
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  bool getcrops(XMLNode *fieldNode, int startYear, int endYear);
  //!Recover the FASSET crop code corresponding to the crop identifier
  /*!
   *  @param croptypeid string containing the crop identification number
   */
  string *getCropID(string croptypeid);
  //!Recover the tillage operations
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  bool gettillage(XMLNode *fieldNode, int startYear, int endYear);
  //!Recover the fertiliser operations
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  bool getfertilisation(XMLNode *fieldNode, int startYear, int endYear);
  //!Recover the manure operations
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  bool getmanure(XMLNode *fieldNode, int startYear, int endYear);
  //!Sort the field operations in order of date
  void sortOps();
  //!Output the field operations using the NitroEurope C5 file format
   void OutputOps(fstream * outfile, int farmID);
   //!Return the year as an integer
   /*!
    *  @param datestring string containing the date in the format dd/mm/yyyy
    */
	int getYear(string datestring);
   //!Return the month as an integer
   /*!
	*  @param datestring string containing the date in the format dd/mm/yyyy
	*/
	int getMonth(string datestring);
   //!Return the Day as an integer
   /*!
	*  @param datestring string containing the date in the format dd/mm/yyyy
	*/
	int getDay(string datestring);
	//!Return the number of operations in a particular year
	/*!
	 *  @param year the year for which the number of operations is required
	 */
	int getNumOpsThisYear(int year);
	//!Output the field operations for a particular year, using the FASSET file format
	/*!
	 *  @param fieldsfnn pointer to the output file
	 *   @param year year for which the operations should be provided
	 */
	bool OutputOpsTofnn(ofstream *fieldsfnn, int year);
	void checkOps();
	void setfieldArea(double aVal){fieldArea=aVal;}
	//!
	double getfieldArea(){return fieldArea;}
	int getCropIDNumber(int index);
	string getCropName(int index);
	bool getisGood(){return isGood;}
	void setisGood(bool aVal){isGood=aVal;}
	int getseriesNumber(){return seriesNumber;}
};

#endif

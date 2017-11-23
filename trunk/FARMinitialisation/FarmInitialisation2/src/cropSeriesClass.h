// Class: NCUClass                     //ANSI C++

#include "common.h"
#include "linklist.h"
#include "xmlParser.h"

#ifndef __CROPSERIESCLASS_H
  #define __CROPSERIESCLASS_H

                              //Required for 1:M aggregation (part) classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//!Stores and manages the field operations for an individual field or location

class cropSeriesClass
{

private:

	//!Unique identification number for the series
	int seriesNumber;
	//!List of pointers to the operations
   linkList<fieldOp> *theOperations; //1:M aggregation part data member

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
  void addOperations(XMLNode *fieldNode,int startYear, int endYear);
  //!Recover the crop related field operations (sowing and harvesting)
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  void getcrops(XMLNode *fieldNode, int startYear, int endYear);
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
  void gettillage(XMLNode *fieldNode, int startYear, int endYear);
  //!Recover the fertiliser operations
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  void getfertilisation(XMLNode *fieldNode, int startYear, int endYear);
  //!Recover the manure operations
  /*!
   *  @param fieldNode pointer to the XML field node
   *  @param startYear first year for which operations should be generated
   *  @param endYear last year for which operations should be generated
   */
  void getmanure(XMLNode *fieldNode, int startYear, int endYear);
  //!Sort the field operations in order of date
  void sortOps();
  //!Output the field operations using the NitroEurope C5 file format
   void OutputOps(fstream * outfile, int NCU);
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
};

#endif

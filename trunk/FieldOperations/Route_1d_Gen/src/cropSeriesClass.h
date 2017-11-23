// Class: NCUClass                     //ANSI C++

#include "common.h"
#include "linklist.h"

#ifndef __CROPSERIESCLASS_H
  #define __CROPSERIESCLASS_H

                             //Required for 1:M aggregation (part) classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

class cropSeriesClass
{

private:

	int seriesNumber;
   linkList<fieldOp> *theOperations; //1:M aggregation part data member

protected:

public:

			//constructor
  cropSeriesClass (int theseriesNumber);

   ~ cropSeriesClass ( );  //Destructor - Delete any pointer data members that used new in constructors
  linkList<fieldOp> * gettheOperations() {return theOperations;}
	void sortOps();
   void OutputOps(fstream * outfile, int NCU);
   int GetNumOutputOpsInYear(int targetYear);
	bool OutputOpsTofnn(fstream *fieldsfnn, int year);
	bool ReadFieldOps(ifstream *opFile);
	//! Get year of first field operation
	int getFirstYear();
	//! Get year of last field operation
	int getLastYear();
};

#endif

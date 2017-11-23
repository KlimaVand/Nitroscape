// Class: fieldOp                     //ANSI C++

#ifndef __FIELDOPHARV_H  //Required for current class
  #define __FIELDOPHARV_H

#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//#include <bsTime.h>        //Required for include files

//! Describes a harvesting operation

class fieldOpHarv: public fieldOp
{

private:

protected:

     //! True if straw should be collected
     bool collectStraw;

public:

			//!Default constructor
  fieldOpHarv (){opCode = 9;};

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param Month month
\param Day day
\param CollectStraw True if straw should be collected
*/
   void DefineOp (char * aName, int aNumber, int Year, int Month, int Day, bool CollectStraw);

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param JulianDay Julian day on which operation occurs
\param CollectStraw True if straw should be collected
*/
	void DefineOp(char * aName, int aNumber, int Year, int JulianDay, bool CollectStraw);


//! Write details of the timing of the operation to a file
/*!
\param outfile pointer to the output file
\param status integer indicating the success or failure of the operation to be performed as desired
\param printYear boolean which if true, prevents the year from being output
*/
 virtual void outputOps (fstream * outfile, int status, bool printYear)    ;

//!Destructor
  virtual  ~ fieldOpHarv ( );  //Destructor - Delete any pointer data members that used new in constructors

  virtual void outputFnnOps(ofstream *outfile, int number);

};

#endif

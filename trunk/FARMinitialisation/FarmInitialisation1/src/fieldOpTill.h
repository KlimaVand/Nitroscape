// Class: fieldOpTill                     //ANSI C++

#include "linklist.h"

#ifndef __FIELDOPTILL_H  //Required for current class
  #define __FIELDOPTILL_H

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//! Describes a tillage operation

class fieldOpTill  : public fieldOp
{

  //! Depth to which the soil should be tilled (millimetres)
     float depth; //Attribute data member

private:

protected:

public:

			//!Default constructor
  fieldOpTill () {opCode = 1;};

//! Defines the parameters of the tillage operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param Month month
\param Day day
\param tillageDepth depth of the tillage operation
*/
   void DefineOp(char * aName, int aNumber, int Year, int Month, int Day, float tillageDepth);

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param JulianDay Julian day on which operation occurs
\param tillageDepth depth of the tillage operation
*/
	void DefineOp(char * aName, int aNumber, int Year, int JulianDay, float tillageDepth);

   ~ fieldOpTill ( );  //Destructor - Delete any pointer data members that used new in constructors

                       //!Get accessor function for non-static attribute data member
  float getdepth() const  { return depth;}

                       //!Set accessor function for non-static attribute data member
  void setdepth (float adepth)  { depth = adepth;}

  //! Write details of the timing of the operation to a file
  /*!
  \param outfile pointer to the output file
  \param status integer indicating the success or failure of the operation to be performed as desired
  \param printYear boolean which if true, prevents the year from being output
  */
   virtual void outputOps (fstream * outfile, int status, bool printYear)    ;
   virtual void outputFnnOps(ofstream *outfile, int number);

};

#endif

// Class: fieldOpSow                     //ANSI C++

#include "base\linklist.h"

#ifndef __FIELDOPSOW_H  //Required for current class
  #define __FIELDOPSOW_H

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//! Specialised field operation for sowing. See fieldOp for details.

class fieldOpSow  : public fieldOp
{

  //! Name of the crop to be sown
     string plantName; //Attribute data member

private:

protected:

public:

			//Default constructor
  fieldOpSow () {opCode = 2;};

//! Defines the parameters of the sowing operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param Month month
\param Day day
\param aplantName name of the crop to be sown
*/
   void DefineOp (char * aName, int aNumber, int Year, int Month, int Day, string aplantName)    ;

/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param JulianDay Julian day no
\param aplantName name of the crop to be sown
*/
   void DefineOp(char * aName, int aNumber, int Year, int JulianDay, string aplantName);

//! Tidy up
   void closeDown ()    ;

   ~ fieldOpSow ( );  //Destructor - Delete any pointer data members that used new in constructors

                       //!Get accessor function for non-static attribute data member
  string getplantName() const  { return plantName;}

                       //!Set accessor function for non-static attribute data member
  void setplantName (string aplantName)  { plantName = aplantName;}

  //! Write details of the timing of the operation to a file
  /*!
  \param outfile pointer to the output file
  \param status integer indicating the success or failure of the operation to be performed as desired
  \param printYear boolean which if true, prevents the year from being output
  */
   virtual  void outputOps (fstream * outfile, int status, bool printYear)    ;
   virtual void outputFnnOps(ofstream *outfile, int number);
};

#endif

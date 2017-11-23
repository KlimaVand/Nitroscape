// Class: fieldOp                     //ANSI C++

#ifndef __FIELDOP_H  //Required for current class
  #define __FIELDOP_H

#include "bstime.h"        //Required for include files
#include "common.h"

//! Describes a generalised or specific field operation

class fieldOp
{

private:

protected:

  //! Date on which the operation can be performed
     bsTime * theOptime; //Attribute data member
   //! Name of the operation
     string name; //Attribute data member
   //! Identity number of the operation
     int opCode; //Attribute data member

public:

			//!Default constructor
  fieldOp ();

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param Month month
\param Day day
*/
 virtual   void DefineOp (char * aName, int aNumber, int Year, int Month, int Day);

//! Defines the parameters of the operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param Year year
\param JulianDay Julian day on which operation occurs
*/
 virtual	void DefineOp(char * aName, int aNumber, int Year, int JulianDay);

//! Initialise an instance
 virtual   void Initialise ()    ;

//! Write details of the timing of the operation to a file
/*!
\param outfile pointer to the output file
\param status integer indicating the success or failure of the operation to be performed as desired
\param printYear boolean which if true, prevents the year from being output
*/
 virtual  void outputOps (fstream * outfile, int status, bool printYear)    ;

//! Tidy up
 virtual   void closeDown ()    ;
//!Destructor
  virtual  ~ fieldOp ( );  //Destructor - Delete any pointer data members that used new in constructors

                       //!Get accessor function for non-static attribute data member
  bsTime * gettheOptime() const  { return theOptime;}

                       //!Get accessor function for non-static attribute data member
  string getname() const  { return name;}

                       //!Get accessor function for non-static attribute data member
  int getopCode() const  { return opCode;}

                       //!Set accessor function for non-static attribute data member
  void settheOptime (bsTime * atheOptime)  { theOptime = atheOptime;}

                       //!Set accessor function for non-static attribute data member
  void setname (string aname)  { name = aname;}

                       //!Set accessor function for non-static attribute data member
  void setopCode (int aopCode)  { opCode = aopCode;}

  virtual void outputFnnOps(ofstream *outfile, int number){};

};

#endif

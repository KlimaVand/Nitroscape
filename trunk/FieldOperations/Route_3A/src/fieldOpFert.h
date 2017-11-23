// Class: fieldOpFert                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOPFERT_H  //Required for current class
  #define __FIELDOPFERT_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif
                               
#include <bsTime.h>        //Required for include files
                               
//! Specialised field operation for the application of fertiliser

class fieldOpFert  : public fieldOp        
{                       
			
  //! Amount of fertiliser or manure nitrogen tobe applied (kilogram hectare). Optional parameter		
     double amountN; //Attribute data member
   //! Type of manure to be applied. Optional parameter.		
     string * type; //Attribute data member        

private:

protected:

public:

			//Default constructor
  fieldOpFert () {};

//! Defines the parameters of a fertiliser operation
/*!
\param aName name of the operation
\param aNumber identity of the operation
\param startYear year of starting date
\param startMonth month of starting date
\param startDay day of starting date
\param endYear year of end date
\param  endMonth month of end date
\param endDay day of end date 
\param aTsum temperature sum for operation
\param aworkabilityConstraint set to true if workability limit is to be applied
\param theamountN the amount of nitrogen in the fertiliser or manure applied (kilograms per hectare). Note: optional parameter
\param theType the type of fertiliser or manure applied. Note: optional parameter
*/					
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, string * theType, double theamountN)    ;

//! Tidy up					
   void closeDown ()    ;
 
   ~ fieldOpFert ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getamountN() const  { return amountN;}

                       //!Get accessor function for non-static attribute data member
  string * gettype() const  { return type;}
                       
                       //!Set accessor function for non-static attribute data member
  void setamountN (double aamountN)  { amountN = aamountN;}

                       //!Set accessor function for non-static attribute data member
  void settype (string * atype)  { type = atype;}
       
};
			
#endif

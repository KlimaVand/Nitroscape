// Class: fieldOpSow                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOPSOW_H  //Required for current class
  #define __FIELDOPSOW_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif
                               
#include <bsTime.h>        //Required for include files
                               
//! Specialised field operation for sowing. See fieldOp for details.

class fieldOpSow  : public fieldOp        
{                       
			
  //! Name of the crop to be sown		
     string * plantName; //Attribute data member        

private:

protected:

public:

			//Default constructor
  fieldOpSow () {};

//! Defines the parameters of the sowing operation
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
\param aplantName name of the crop to be sown
*/					
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, string * aplantName)    ;

//! Tidy up					
   void closeDown ()    ;
 
   ~ fieldOpSow ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  string * getplantName() const  { return plantName;}
                       
                       //!Set accessor function for non-static attribute data member
  void setplantName (string * aplantName)  { plantName = aplantName;}
       
};
			
#endif

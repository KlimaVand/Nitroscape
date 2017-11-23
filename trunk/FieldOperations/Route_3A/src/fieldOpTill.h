// Class: fieldOpTill                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOPTILL_H  //Required for current class
  #define __FIELDOPTILL_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif
                               
#include <bsTime.h>        //Required for include files
                               
//! Describes a tillage operation

class fieldOpTill  : public fieldOp        
{                       
			
  //! Depth to which the soil should be tilled (millimetres)		
     double depth; //Attribute data member        

private:

protected:

public:

			//!Default constructor
  fieldOpTill () {};

//! Defines the parameters of the tillage operation
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
\param tillageDepth depth of the tillage operation
*/					
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, double tillageDepth)    ;
 
   ~ fieldOpTill ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getdepth() const  { return depth;}
                       
                       //!Set accessor function for non-static attribute data member
  void setdepth (double adepth)  { depth = adepth;}
       
};
			
#endif

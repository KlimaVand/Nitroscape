// Class: fieldOpIrr                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOPIRR_H  //Required for current class
  #define __FIELDOPIRR_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif
                               
#include <bsTime.h>        //Required for include files
                               
//! Describes an irrigation operation

class fieldOpIrr  : public fieldOp
{

  //! Threshold soil moisture deficit (proportion of field capacity)
     double thresholdSMD; //Attribute data member

private:

protected:

public:

			//Default constructor
  fieldOpIrr () {};

//! Defines the parameters of the operation
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
\param athresholdSMD the threshold soil moisture deficit which triggers irrigation (millimetres)
*/
					
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, double athresholdSMD)    ;
 
   ~ fieldOpIrr ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getthresholdSMD() const  { return thresholdSMD;}
                       
                       //!Set accessor function for non-static attribute data member
  void setthresholdSMD (double athresholdSMD)  { thresholdSMD = athresholdSMD;}
       
};
			
#endif

// Class: fieldOpGrass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOPGRASS_H  //Required for current class
  #define __FIELDOPGRASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif
                               
#include <bsTime.h>        //Required for include files
                               
//! Describes a grazing operation

class fieldOpGrass  : public fieldOp        
{                       
			
  //! Leaf area index of the crop after harvesting		
     double harvLAI; //Attribute data member        

private:

protected:

public:

			//Default constructor
  fieldOpGrass () {};

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
\param LAI leaf area index after grazing
*/
					
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, double LAI)    ;
 
   ~ fieldOpGrass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getharvLAI() const  { return harvLAI;}
                       
                       //!Set accessor function for non-static attribute data member
  void setharvLAI (double aharvLAI)  { harvLAI = aharvLAI;}
       
};
			
#endif

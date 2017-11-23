
 
// Class: fieldOpGrass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpGrass.h"   
#include "soilDataClass.h" 				

fieldOpGrass:: ~ fieldOpGrass ( )
{

};

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

void fieldOpGrass::DefineOp(char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, double LAI)  
{
	fieldOp::DefineOp(aName,  aNumber, startYear, startMonth,startDay, endYear, endMonth,endDay, aTsum, aworkabilityConstraint);
   harvLAI=LAI;
}

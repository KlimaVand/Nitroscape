
 
// Class: fieldOpSow

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpSow.h"   
#include "soilDataClass.h" 				

fieldOpSow:: ~ fieldOpSow ( )
{

};

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
void fieldOpSow::DefineOp(char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, string * aplantName)  
{
	fieldOp::DefineOp(aName, aNumber, startYear, startMonth,startDay, endYear, endMonth,endDay, aTsum, aworkabilityConstraint);
   plantName=aplantName;
}
//! Tidy up
void fieldOpSow::closeDown()  
{
   fieldOp::closeDown();
   delete plantName;
}

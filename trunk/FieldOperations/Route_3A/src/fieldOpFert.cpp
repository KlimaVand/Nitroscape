
 
// Class: fieldOpFert

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOpFert.h"   
#include "trafworkDataClass.h" 				

fieldOpFert:: ~ fieldOpFert ( )
{

};

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
void fieldOpFert::DefineOp(char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint, string * theType, double theamountN)  
{
	fieldOp::DefineOp(aName, aNumber, startYear, startMonth,startDay, endYear, endMonth,endDay, aTsum, aworkabilityConstraint);
   amountN=theamountN;
   type=theType;
}
//! Tidy up
void fieldOpFert::closeDown()  
{
   fieldOp::closeDown();
   delete type;
}

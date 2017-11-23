

// Class: fieldOp

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOp.h"   
#include "soilDataClass.h" 				

fieldOp:: ~ fieldOp ( )
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
*/
void fieldOp::DefineOp(char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint)  
{
 name=aName;
 opStart->SetTime(startDay, startMonth, startYear);
 opEnd->SetTime(endDay, endMonth, endYear);
 Tsum=aTsum;
 workabilityConstraint=aworkabilityConstraint;
 number = aNumber;
}
//!Check to see if an operation can be performed
/*!
\param currentTime the current date
\param currentTsum the current temperature sum
*/
int fieldOp::doOpNow(bsTime * currentTime, double currentTsum)  
{
   if (*currentTime>=*opStart)
   {
	   if (*currentTime>=*opEnd)
      {
      	return 3;
      }
      else
        if (currentTsum >= Tsum)
        {
           if (*currentTime==*opStart)
           	return 1;
           else
           	return 2;
        }
   }
   return 0;
}
//! Initialise an instance
void fieldOp::Initialise()  
{
 opStart  = new bsTime();
 opEnd  = new bsTime();
 name = "No operation" ;
}
//! Write details of the timing of the operation to a file
/*! 
\param outfile pointer to the output file
\param theOptime the date on which the operation is performed
\param status integer indicating the success or failure of the operation to be performed as desired
*/
void fieldOp::outputOps(fstream * outfile, bsTime * theOptime, int status)  
{
	*outfile << theOptime->GetYear() << "\t" << theOptime->GetJulianDay() << "\t" << number << "\t" << status << "\t" << *theOptime << "\t" << name;
}
//! Tidy up
void fieldOp::closeDown()  
{
delete opStart;
delete opEnd;}

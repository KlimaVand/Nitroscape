// Class: fieldOp                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDOP_H  //Required for current class
  #define __FIELDOP_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

#include <bsTime.h>        //Required for include files
                               
//! Describes a generalised or specific field operation

class fieldOp      
{                       
			
  //! Earliest date on which the operation can be performed		
     bsTime * opStart; //Attribute data member
   //! The date on or before which the operation must be performed		
     bsTime * opEnd; //Attribute data member
   //! Name of the operation		
     string name; //Attribute data member
   //! Temperature sum must be achieved before the operation is performed		
     double Tsum; //Attribute data member
   //! True if the operation should be constrained by the workability of the soil, otherwise the trafficability constraint will apply		
     bool workabilityConstraint; //Attribute data member
   //! Identity number of the operation		
     int number; //Attribute data member        

private:

protected:

public:

			//!Default constructor
  fieldOp () {};

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
   void DefineOp (char * aName, int aNumber, int startYear, int startMonth, int startDay, int endYear, int endMonth, int endDay, double aTsum, bool aworkabilityConstraint)    ;

//!Check to see if an operation can be performed
/*! 
\param currentTime the current date
\param currentTsum the current temperature sum
*/					
   int doOpNow (bsTime * currentTime, double currentTsum)    ;

//! Initialise an instance					
 virtual   void Initialise ()    ;

//! Write details of the timing of the operation to a file
/*! 
\param outfile pointer to the output file
\param theOptime the date on which the operation is performed
\param status integer indicating the success or failure of the operation to be performed as desired
*/					
   void outputOps (fstream * outfile, bsTime * theOptime, int status)    ;

//! Tidy up					
 virtual   void closeDown ()    ;
//!Destructor 
  virtual  ~ fieldOp ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  bsTime * getopStart() const  { return opStart;}

                       //!Get accessor function for non-static attribute data member
  bsTime * getopEnd() const  { return opEnd;}

                       //!Get accessor function for non-static attribute data member
  string getname() const  { return name;}

                       //!Get accessor function for non-static attribute data member
  double getTsum() const  { return Tsum;}

                       //!Get accessor function for non-static attribute data member
  bool getworkabilityConstraint() const  { return workabilityConstraint;}

                       //!Get accessor function for non-static attribute data member
  int getnumber() const  { return number;}
                       
                       //!Set accessor function for non-static attribute data member
  void setopStart (bsTime * aopStart)  { opStart = aopStart;}

                       //!Set accessor function for non-static attribute data member
  void setopEnd (bsTime * aopEnd)  { opEnd = aopEnd;}

                       //!Set accessor function for non-static attribute data member
  void setname (string aname)  { name = aname;}

                       //!Set accessor function for non-static attribute data member
  void setTsum (double aTsum)  { Tsum = aTsum;}

                       //!Set accessor function for non-static attribute data member
  void setworkabilityConstraint (bool aworkabilityConstraint)  { workabilityConstraint = aworkabilityConstraint;}

                       //!Set accessor function for non-static attribute data member
  void setnumber (int anumber)  { number = anumber;}
       
};
			
#endif

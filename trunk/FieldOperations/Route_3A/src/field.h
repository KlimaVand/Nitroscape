// Class: field                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELD_H  //Required for current class
  #define __FIELD_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

#include <climateClass.h>        //Required for include files

#include <cropManageClass.h>        //Required for include files

#include <fieldOpTill.h>        //Required for include files

#include <fieldOpGrass.h>        //Required for include files

#include <fieldOpSow.h>        //Required for include files

#include <fieldOpFert.h>        //Required for include files

#include <nixcbits.h>        //Required for include files

#include <trafworkDataClass.h>        //Required for include files

#include <fieldOpIrr.h>        //Required for include files
                               
                               //Required for 1:1 association classes
#ifndef __TRAFWORKDATACLASS_H
  #include "trafworkDataClass.h"
#endif
                                                               
                               //Required for 1:1 aggregation (part) classes
#ifndef __ECOSYSTEM_H
  #include "ecosystem.h"
#endif
                                                               
                               //Required for 1:M aggregation (part) classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif

//! Simulates an individual field

class field: public base
{                       

  //! Is set to true if the crop should be grazed		
     bool graze; //Attribute data member
   //! Index of the next field operation to be performed		
     int currentOpNo; //Attribute data member
   //! Pointer to the output file
     fstream * outfile; //Attribute data member
   //! Index of the field		
     int fieldNo; //Attribute data member
   //! Is set to true if the field is to be irrigated		
     bool irrigate; //Attribute data member        

  //! The ecosystem model
  ecosystem theEcosystem; //1:1 aggregation part data member        

  //! Pointer to the trafficability and workability data
  trafworkDataClass* theTrafWorkData; //1:1 association object data member      
			
  //! Pointer to the list of field operations
   linkList<fieldOp> *theOperations; //1:M aggregation part data member

private:

protected:

public:

			//!Constructor
/*!
\param  aName Not used
\param  aIndex field number
\param  aOwner pointer to the location class that owns this field
*/
  field (const char * aName, const int aIndex, const base * aOwner);

//!Perform daily routines
/*!
\param potEvap potential evapotranspiration (millimetres per day)
\param precip precipitation (millimetres per day)
\param Tmean mean daily air temperature (Celsius)
\param currentTsum current temperature sum (degree days)
*/
   void dailyRoutine (double potEvap, double precip, double Tmean, double currentTsum)    ;

//!Initialise an instance of field
/*!
\param afieldNo field identification number
\param theSoilType soil type to be used
\param thecropManagement pointer to a list of crop management practices
\param trafworkData pointer to the trafficability and workability parameters for this field
\param anoutputFile pointer to the output file
*/
   void Initialise (int afieldNo, int theSoilType, linkList<cropManageClass> * thecropManagement, trafworkDataClass * trafworkData, fstream * anoutputFile)    ;

//! Outputs field variables to file
/*!
\param file pointer to output file
\param header set to true if the variable names should be printed
*/
   void outputDetails (fstream * file, bool header)    ;

//! Loads the field operation list
/*!
\param thePlan pointer to the list of field operations
*/
   void loadOps (cropManageClass * thePlan)    ;

//! Checks whether one or more field operations can be performed. If so, the timing of the operation is logged to the output file.
   fieldOp * doOp (double Tsum)    ;

//! Sorts the field operations in order of starting date
   void sortOps ()    ;

//! Tidy up
   void closeDown ()    ;
//!Destructor
   ~ field ( );  //Destructor - Delete any pointer data members that used new in constructors

                       //!Get accessor function for non-static attribute data member
  bool getgraze() const  { return graze;}

                       //!Get accessor function for non-static attribute data member
  int getcurrentOpNo() const  { return currentOpNo;}

                       //!Get accessor function for non-static attribute data member
  fstream * getoutfile() const  { return outfile;}

                       //!Get accessor function for non-static attribute data member
  int getfieldNo() const  { return fieldNo;}

                       //!Get accessor function for non-static attribute data member
  bool getirrigate() const  { return irrigate;}
                       
                       //!Set accessor function for non-static attribute data member
  void setgraze (bool agraze)  { graze = agraze;}

                       //!Set accessor function for non-static attribute data member
  void setcurrentOpNo (int acurrentOpNo)  { currentOpNo = acurrentOpNo;}

                       //!Set accessor function for non-static attribute data member
  void setoutfile (fstream * aoutfile)  { outfile = aoutfile;}

                       //!Set accessor function for non-static attribute data member
  void setfieldNo (int afieldNo)  { fieldNo = afieldNo;}

                       //!Set accessor function for non-static attribute data member
  void setirrigate (bool airrigate)  { irrigate = airrigate;}

                       //!Get accessor function for 1:1 aggregation part data member
  ecosystem& gettheEcosystem()   { return theEcosystem;}
                       
                       //!Set accessor function for 1:1 aggregation part data member
  void settheEcosystem (ecosystem& atheEcosystem)  { theEcosystem = atheEcosystem;  }

                       //!Get accessor function for 1:many aggregation object data member
  linkList <fieldOp>* gettheOperations()  { return theOperations;  }
                       
                       //!Get accessor function for 1:1 association object data member
  trafworkDataClass* gettheTrafWorkData() const  { return theTrafWorkData;  }
                       
                       //!Set accessor function for 1:1 association object data member
  void settheTrafWorkData (trafworkDataClass* atheTrafWorkData)  { theTrafWorkData = atheTrafWorkData;  }

};
			
#endif

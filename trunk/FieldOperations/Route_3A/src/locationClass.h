// Class: locationClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __LOCATIONCLASS_H  //Required for current class
  #define __LOCATIONCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include <trafworkCharClass.h>        //Required for include files
                               
                               //Required for 1:1 association classes
#ifndef __CLIMATECLASS_H
  #include "climateClass.h"
#endif
                                                               
                               //Required for 1:M aggregation (part) classes
#ifndef __FIELD_H
  #include "field.h"
#endif

                               //Required for 1:M aggregation (part) classes
#ifndef __CROPMANAGECLASS_H
  #include "cropManageClass.h"
#endif

//! Contains all sub models for a single location

class locationClass  : public base
{

  //! NCU identifier
     int NCU; //Attribute data member
  //! Name of climate file
     string climateFileName; //Attribute data member
   //! Name of file containing the characteristics of crops
     string cropCharFileName; //Attribute data member
   //! Name a file containing the field operations for the crops
     string cropManageFileName; //Attribute data member
   //! Soil type
     int soilType; //Attribute data member
   //! Temperature sum (degree days)
     double Tsum; //Attribute data member
   //! Pointer to the output file
     fstream * outfile; //Attribute data member

     //!Pointer to the climate data
  climateClass* aClimate; //1:1 association object data member
	//!Pointer to a list of field classes
   linkList<field> *theFields; //1:M aggregation part data member
  //!Pointer to a list of crop management plans
   linkList<cropManageClass> *theCropOps; //1:M aggregation part data member

private:

protected:

public:

			//Default constructor
  locationClass () {};

//! Initialises the model for a given location
/*!
\param anNCU NCU identifier for this location
\param aclimateFileName name of a climate file
\param outputFile pointer to an output file
\param aTrafworkdb pointer to a class instance containing the workability and trafficability parameters for all soil types
*/
   void Initialise (int anNCU, char * aclimateFileName, fstream * outputFile, trafworkCharClass * aTrafworkdb)    ;

//! Perform the daily functions for this location
   void dailyRoutine ()    ;

//! Tidy up
   void closedown ()    ;

//!Output details of the model at this location
/*!
\param afile a pointer to the output file
\param header set to true if only variable names should be printed to file
*/
   void outputDetails (fstream * afile, bool header)    ;
 //!Destructor
   ~ locationClass ( );

                       //!Get accessor function for non-static attribute data member
  string getclimateFileName() const  { return climateFileName;}

                       //!Get accessor function for non-static attribute data member
  string getcropCharFileName() const  { return cropCharFileName;}

                       //!Get accessor function for non-static attribute data member
  string getcropManageFileName() const  { return cropManageFileName;}

                       //!Get accessor function for non-static attribute data member
  int getsoilType() const  { return soilType;}

                       //!Get accessor function for non-static attribute data member
  double getTsum() const  { return Tsum;}

  int GetNCU() {return NCU;}

                       //!Set accessor function for non-static attribute data member
  void setclimateFileName (string aclimateFileName)  { climateFileName = aclimateFileName;}

                       //!Set accessor function for non-static attribute data member
  void setcropCharFileName (string acropCharFileName)  { cropCharFileName = acropCharFileName;}

                       //!Set accessor function for non-static attribute data member
  void setcropManageFileName (string acropManageFileName)  { cropManageFileName = acropManageFileName;}

                       //!Set accessor function for non-static attribute data member
  void setsoilType (int asoilType)  { soilType = asoilType;}

                       //!Set accessor function for non-static attribute data member
  void setTsum (double aTsum)  { Tsum = aTsum;}
       
                       //!Get accessor function for 1:many aggregation object data member
  linkList <field>* gettheFields()  { return theFields;  }

                       //!Get accessor function for 1:many aggregation object data member
  linkList <cropManageClass>* gettheCropOps()  { return theCropOps;  }
                       
                       //!Get accessor function for 1:1 association object data member
  climateClass* getaClimate() const  { return aClimate;  }
                       
                       //!Set accessor function for 1:1 association object data member
  void setaClimate (climateClass* aaClimate)  { aClimate = aaClimate;  }

};
			
#endif

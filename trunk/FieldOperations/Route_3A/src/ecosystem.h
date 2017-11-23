// Class: ecosystem                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __ECOSYSTEM_H  //Required for current class
  #define __ECOSYSTEM_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include <cropCharClass.h>        //Required for include files

#include <climateClass.h>        //Required for include files

#include <cropManageClass.h>        //Required for include files

#include <fieldOpTill.h>        //Required for include files

#include <fieldOpGrass.h>        //Required for include files

#include <fieldOpSow.h>        //Required for include files

#include <nixcbits.h>        //Required for include files

#include <soilCharClass.h>        //Required for include files
                               
                               //Required for 1:1 aggregation (part) classes
#ifndef __SIMPLEPLANT_H
  #include "simplePlant.h"
#endif

                               //Required for 1:1 aggregation (part) classes
#ifndef __SIMPLESOIL_H
  #include "simpleSoil.h"
#endif
                                                               
                               //Required for 1:M aggregation (part) classes
#ifndef __CROPCHARCLASS_H
  #include "cropCharClass.h"
#endif

//! Describes a single ecosystem

class ecosystem  : public base        
{                       
			
  //! Instance of the crop model						
  simplePlant thePlants; //1:1 aggregation part data member
   //! Instance of the soil model						
  simpleSoil theSoil; //1:1 aggregation part data member        

  //! List of pointers to the crop parameters
   linkList<cropCharClass> *theCropChars; //1:M aggregation part data member 
 /*!finds the crop parameters for a given crop
 \param cropName The name of crop for which parameters are required
 */
   cropCharClass * FindCropChars (const string cropName)    ;
 
private:

protected:

public:

			//!Default constructor
  ecosystem () {};

//!Simulate daily processes of the ecosystem
/*! 
\param potEvap potential evapotranspiration (millimetres)
\param precip precipitation (millimetres)
\param Tmean mean air temperature (Celsius)
\param autumn set to true if the plant model should enter autumn mode
*/					
   void dailyRoutine (double potEvap, double precip, double Tmean, bool autumn)    ;

//!Initialise the ecosystem model
/*! 
\param soilType number of the soil to be used by the ecosystem model
*/					
   void Initialise (int soilType)    ;

//!Output state variables to file
/*! 
\param file pointer to output file
\param header set to true if name of variables should be printed
*/					
   void outputDetails (fstream * file, bool header)    ;

//!Harvest the current crop
/*! 
\param residualLAI leaf area index remaining after harvesting
*/					
   void harvest (double residualLAI)    ;

//!Harvest the current crop by cutting
/*!
\param residualLAI leaf area index of the crop after cutting
*/
					
   void cut (double residualLAI)    ;

//!Sow a given crop
/*! 
\param cropName name of the crop to be sown
\param autumn sets to true if the crop should enter autumn mode
*/					
   void sow (string cropName, bool autumn)    ;

//!Till the soil
/*! 
\param depth depth to which the soil should be tilled
*/					
   void tillage (double depth)    ;

//!Add fertiliser. 
/*! Note:Type and amount of fertiliser are not currently used. Included here for future use.
\param aType type of fertiliser to be applied
\param anAmount the amount of fertiliser to be applied
*/					
   void fertilise (string aType, double anAmount)    ;

//! Return the name of the current crop					
   string getCropName ()    ;

//!Returns the current soil moisture deficit (in millimetres)
/*! 
\param depth soil depth to which the soil moisture deficit should be calculated
*/					
   double getSMD (double depth)    ;

//! Tidy up ecosystem model					
   void closeDown ()    ;

//! Return the soil depth (millimetres)					
   double getSoilDepth ()    ;

//! Return the plant-available water to maximum rooting depth at field capacity (millimetres)					
   double getFieldCapacity ()    ;

/*! Tell the soil whether irrigation should occur or not
\param toIrrigate is set to true if irrigation should occur
*/					
   void irrigate (bool toIrrigate)    ;
 
   ~ ecosystem ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for 1:1 aggregation part data member
  simplePlant& getthePlants()   { return thePlants;}

                       //!Get accessor function for 1:1 aggregation part data member
  simpleSoil& gettheSoil()   { return theSoil;}
                       
                       //!Set accessor function for 1:1 aggregation part data member
  void setthePlants (simplePlant& athePlants)  { thePlants = athePlants;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void settheSoil (simpleSoil& atheSoil)  { theSoil = atheSoil;  }

                       //!Get accessor function for 1:many aggregation object data member
  linkList <cropCharClass>* gettheCropChars()  { return theCropChars;  }
                       
};
			
#endif

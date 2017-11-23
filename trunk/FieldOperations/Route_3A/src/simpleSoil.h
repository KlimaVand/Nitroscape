// Class: simpleSoil                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __SIMPLESOIL_H  //Required for current class
  #define __SIMPLESOIL_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

#include <soilDataClass.h>        //Required for include files

#include <message.h>        //Required for include files
                               
                               //Required for 1:1 association classes
#ifndef __SOILDATACLASS_H
  #include "soilDataClass.h"
#endif

                               //Required for 1:1 association classes
#ifndef __SIMPLEPLANT_H
  #include "simplePlant.h"
#endif
                                                               
                               //Required for 1:1 aggregation (part) classes
#ifndef __POOL_H
  #include "pool.h"
#endif
                               //Required for 1:M aggregation (part) classes
#ifndef __LAYERCLASS_H
  #include "layerClass.h"
#endif

//! Describes the soil

class simpleSoil      
{                       
			
  //! Cumulative amount of water input into the system		
     double waterIn; //Attribute data member
   //! Cumulative amount of water leaving the system		
     double waterOut; //Attribute data member
   //! Plant-available water of whole soil at field capacity (millimetres)		
     double soilFieldCapacity; //Attribute data member
   //! Is set to true if the soil should be irrigated		
     bool irrigate; //Attribute data member        

   //! Pool containing snow						
  pool theSnow; //1:1 aggregation part data member
   //! Rooting pool
  pool theRooting; //1:1 aggregation part data member
  //! Pool of water beneath the rooting pool
  pool theSubZone; //1:1 aggregation part data member
   //! Plant canopy interception pool
  pool canopyInterception; //1:1 aggregation part data member
  //!Pool that describes the water that can be lost in the soil by evaporation
  pool theEvaporation; //1:1 aggregation part data member
  /*!Pool that describes the water available in the upper part of the rooting zone. Only used when precipitation
  or irrigation water falls onto the soil at a time when the soil moisture in the rooting zone is below the breakpoint
  for reduced transpiration.
  */
  pool upperRooting; //1:1 aggregation part data member

   //! Pointer to the soil property data
  soilDataClass* theSoilProperties; //1:1 association object data member
  //!Pointer to an instance of the class describing the crop
  simplePlant* thePlants; //1:1 association object data member

   //! Pointer to a list of instances containing the physical characteristics of each soil layer
   linkList<layerClass> *theLayers; //1:M aggregation part data member

private:

protected:

public:

			//!Default constructor
  simpleSoil () {};

//!Initialise an instance of this class
/*! 
\param thesoilData pointer to the instance of a class containing the soil parameters
*/					
   void Initialise (soilDataClass * thesoilData)    ;

//! Execute the daily functions
/*!
\param potEvap potential evapotranspiration (millimetres per day)
\param precipIn precipitation (millimetres per day)
\param Tmean mean air temperature (Celsius)
*/					
   void dailyRoutine (double potEvap, double precipIn, double Tmean)    ;

//!Calculate the capacity of a soil water pool
/*! 
\param startDepth depth below the soil surface of the upper boundary of the pool (millimetres)
\param endDepth depth below the soil surface of the lower boundary of the pool (millimetres)
*/					
   double calcPoolVolume (double startDepth, double endDepth)    ;

//!Adjust the variables and parameters of the soil pool in response to a tillage operation
/*! 
\param depth depth to which the soil is tilled (millimetres)
*/					
   void tillage (double depth)    ;

//! Update the soil pools to account for changes in the crop rooting depth					
   void updatePools ()    ;

//! Reports an error if the soil water budget cannot be closed					
   void checkBudget ()    ;

//!Outputs the soil variables to a file
/*! 
\param afile pointer to the output file
\param header set to true if only the variable names should be printed
*/					
   void outputDetails (fstream * afile, bool header)    ;

//!Returns the soil moisture deficit to a given depth
/*! 
\param depth depth below the soil surface for which the soil moisture deficit should be calculated
*/					
   double getSMD (double depth)    ;

//! Tidy up his class					
   void closeDown ()    ;

//! Return the depth of the soil					
   double getSoilDepth ()    ;
 
   ~ simpleSoil ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getwaterIn() const  { return waterIn;}

                       //!Get accessor function for non-static attribute data member
  double getwaterOut() const  { return waterOut;}

                       //!Get accessor function for non-static attribute data member
  double getsoilFieldCapacity() const  { return soilFieldCapacity;}

                       //!Get accessor function for non-static attribute data member
  bool getirrigate() const  { return irrigate;}
                       
                       //!Set accessor function for non-static attribute data member
  void setwaterIn (double awaterIn)  { waterIn = awaterIn;}

                       //!Set accessor function for non-static attribute data member
  void setwaterOut (double awaterOut)  { waterOut = awaterOut;}

                       //!Set accessor function for non-static attribute data member
  void setsoilFieldCapacity (double asoilFieldCapacity)  { soilFieldCapacity = asoilFieldCapacity;}

                       //!Set accessor function for non-static attribute data member
  void setirrigate (bool airrigate)  { irrigate = airrigate;}
       
                       //!Get accessor function for 1:1 aggregation part data member
  pool& gettheSnow()   { return theSnow;}

                       //!Get accessor function for 1:1 aggregation part data member
  pool& gettheRooting()   { return theRooting;}

                       //!Get accessor function for 1:1 aggregation part data member
  pool& gettheSubZone()   { return theSubZone;}

                       //!Get accessor function for 1:1 aggregation part data member
  pool& getcanopyInterception()   { return canopyInterception;}

                       //!Get accessor function for 1:1 aggregation part data member
  pool& gettheEvaporation()   { return theEvaporation;}

                       //!Get accessor function for 1:1 aggregation part data member
  pool& getupperRooting()   { return upperRooting;}
                       
                       //!Set accessor function for 1:1 aggregation part data member
  void settheSnow (pool& atheSnow)  { theSnow = atheSnow;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void settheRooting (pool& atheRooting)  { theRooting = atheRooting;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void settheSubZone (pool& atheSubZone)  { theSubZone = atheSubZone;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void setcanopyInterception (pool& acanopyInterception)  { canopyInterception = acanopyInterception;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void settheEvaporation (pool& atheEvaporation)  { theEvaporation = atheEvaporation;  }

                       //!Set accessor function for 1:1 aggregation part data member
  void setupperRooting (pool& aupperRooting)  { upperRooting = aupperRooting;  }

                       //!Get accessor function for 1:many aggregation object data member
  linkList <layerClass>* gettheLayers()  { return theLayers;  }
                       
                       //!Get accessor function for 1:1 association object data member
  soilDataClass* gettheSoilProperties() const  { return theSoilProperties;  }

                       //!Get accessor function for 1:1 association object data member
  simplePlant* getthePlants() const  { return thePlants;  }
                       
                       //!Set accessor function for 1:1 association object data member
  void settheSoilProperties (soilDataClass* atheSoilProperties)  { theSoilProperties = atheSoilProperties;  }

                       //!Set accessor function for 1:1 association object data member
  void setthePlants (simplePlant* athePlants)  { thePlants = athePlants;  }

};
			
#endif

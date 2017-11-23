// Class: pool                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __POOL_H  //Required for current class
  #define __POOL_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

//! Describes a pool that contains water

class pool      
{                       
			
  //! Current evaporation from this pool (millimetres)		
     double evaporation; //Attribute data member
   //! Current rate of transpiration from this pool (millimetres)		
     double transpiration; //Attribute data member
   //! Current input of water via capillary transport to this pool (millimetres)		
     double capillary; //Attribute data member
   //! Current loss of water from the pool via drainage (millimetres)		
     double drainage; //Attribute data member
   //! Capacity of the pool to hold water (millimetres)		
     double maxVolume; //Attribute data member
   /*! Rate at which water in excess of the pool capacity drains to the next pool (millimetres per day).
Only valid for soil pools.
*/		
     double drainageConst; //Attribute data member
   //! Name of the pool		
     char * name; //Attribute data member        

private:

protected:

//! Current amount of water in the pool (millimetres)
  double volume;  

public:

			//Default constructor
  pool () {};

//!Update the status variables of the pool
/*! 
\param waterInAbove water entering the pool from above (millimetres)
\param waterInCapillary water entering the pool by capillary transport (millimetres)
\param theevaporation water leaving the pool by evaporation (millimetres)
\param thetranspiration water leaving the pool by transpiration (millimetres)
*/					
   double update (double waterInAbove, double waterInCapillary, double theevaporation, double thetranspiration)    ;

//!Initialise a pool
/*! 
\param thevolume the initial volume of water in the pool (millimetres)
\param themaxVolume the initial capacity of the pool (millimetres)
\param thedrainageConst the initial drainage constant (millimetres per day)
*/					
   void Initialise (double thevolume, double themaxVolume, double thedrainageConst)    ;

//!Simulates the dynamics of snow melting and subtracts evaporation from the snow from the potential evaporation.
/*! 
\param potEvap pointer to the potential evaporation
\param precip precipitation as snow (millimetres of water)
\param Tmean mean daily temperature (Celsius)
*/					
   double dailySnow (double * potEvap, double precip, double Tmean)    ;

//!Outputs state variables to file
/*!
\param afile pointer to output file
\param header set to true in order to print variable names only
*/					
   void outputDetails (fstream * afile, bool header)    ;

//! Return the deficit between the capacity of the pool and the current volume					
   double getDeficit ()    ;
//!Destructor 
   ~ pool ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getvolume() const  { return volume;}

                       //!Get accessor function for non-static attribute data member
  double getevaporation() const  { return evaporation;}

                       //!Get accessor function for non-static attribute data member
  double gettranspiration() const  { return transpiration;}

                       //!Get accessor function for non-static attribute data member
  double getcapillary() const  { return capillary;}

                       //!Get accessor function for non-static attribute data member
  double getdrainage() const  { return drainage;}

                       //!Get accessor function for non-static attribute data member
  double getmaxVolume() const  { return maxVolume;}

                       //!Get accessor function for non-static attribute data member
  double getdrainageConst() const  { return drainageConst;}

                       //!Get accessor function for non-static attribute data member
  char * getname() const  { return name;}
                       
                       //!Set accessor function for non-static attribute data member
  void setvolume (double avolume)  { volume = avolume;}

                       //!Set accessor function for non-static attribute data member
  void setevaporation (double aevaporation)  { evaporation = aevaporation;}

                       //!Set accessor function for non-static attribute data member
  void settranspiration (double atranspiration)  { transpiration = atranspiration;}

                       //!Set accessor function for non-static attribute data member
  void setcapillary (double acapillary)  { capillary = acapillary;}

                       //!Set accessor function for non-static attribute data member
  void setdrainage (double adrainage)  { drainage = adrainage;}

                       //!Set accessor function for non-static attribute data member
  void setmaxVolume (double amaxVolume)  { maxVolume = amaxVolume;}

                       //!Set accessor function for non-static attribute data member
  void setdrainageConst (double adrainageConst)  { drainageConst = adrainageConst;}

                       //!Set accessor function for non-static attribute data member
  void setname (char * aname)  { name = aname;}
       
};
			
#endif

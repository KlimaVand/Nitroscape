
 
// Class: pool

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "pool.h"   
#include "fieldOp.h" 				

pool:: ~ pool ( )
{

};

//!Update the status variables of the pool
/*! 
\param waterInAbove water entering the pool from above (millimetres)
\param waterInCapillary water entering the pool by capillary transport (millimetres)
\param theevaporation water leaving the pool by evaporation (millimetres)
\param thetranspiration water leaving the pool by transpiration (millimetres)
*/
double pool::update(double waterInAbove, double waterInCapillary, double theevaporation, double thetranspiration)  
{
	drainage = 0.0;
	evaporation = theevaporation;
	transpiration = thetranspiration;
	capillary = waterInCapillary;
	volume += waterInAbove + waterInCapillary - theevaporation + thetranspiration;
   if (volume > maxVolume)
   {
   	drainage = drainageConst * (volume - maxVolume);
   	volume -= drainage;
   }
   return drainage;
}
//!Initialise a pool
/*! 
\param thevolume the initial volume of water in the pool (millimetres)
\param themaxVolume the initial capacity of the pool (millimetres)
\param thedrainageConst the initial drainage constant (millimetres per day)
*/
void pool::Initialise(double thevolume, double themaxVolume, double thedrainageConst)  
{
 volume = thevolume;
 maxVolume = themaxVolume;
 drainageConst = thedrainageConst;
 evaporation = 0.0;
 transpiration = 0.0;
 capillary = 0.0;
}
//!Simulates the dynamics of snow melting and subtracts evaporation from the snow from the potential evaporation.
/*! 
\param potEvap pointer to the potential evaporation
\param precip precipitation as snow (millimetres of water)
\param Tmean mean daily temperature (Celsius)
*/
double pool::dailySnow(double * potEvap, double precip, double Tmean)  
{
   drainage = precip;
   double melt = 0.0;
   double newSnow = 0.0;
   if (*potEvap > volume)  //limit evaporation if greater than volume
   {
   	evaporation = volume;
      *potEvap -= evaporation;   //adjust pot evap to reflect loss from pool
      volume = 0.0;
   }
   else
   {
   	evaporation = *potEvap;
   	volume-=evaporation;
      *potEvap = 0.0;
   }
	if (Tmean > 0.0)    //melt some snow if there is any
	{
      if (volume>0.0)
      {
         melt = 2.0 * Tmean;
         if (melt > volume)
            melt = volume;
      }
	   drainage = melt + precip;   //precipitation drops through pool, add meltwater if necessary
   }
   else
   {
   	newSnow = precip;
      drainage = 0.0;
   }
   volume+=newSnow - melt;  //do water balance
   return drainage;
}
//!Outputs state variables to file
/*!
\param afile pointer to output file
\param header set to true in order to print variable names only
*/
void pool::outputDetails(fstream * afile, bool header)  
{
	if (header)
    *afile << "name" << "\t" << "volume" << "\t" << "evaporation" << "\t" << "transpiration" << "\t" <<
      "capillary" << "\t" << "drainage" << "\t" << "maxVolume" << "\t" <<  "drainageConst" << "\t";
   else
    *afile << name << "\t" << volume << "\t" << evaporation << "\t" << transpiration << "\t" <<
      capillary << "\t" << drainage << "\t" << maxVolume << "\t" <<  drainageConst << "\t";
}
//! Return the deficit between the capacity of the pool and the current volume
double pool::getDeficit()  
{
	double aVal;
   if ((maxVolume - volume)>0.0)
   	aVal = (maxVolume - volume);
   else
   	aVal = 0.0;
	return aVal;
}

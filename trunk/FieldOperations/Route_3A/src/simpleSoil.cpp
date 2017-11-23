
 
// Class: simpleSoil

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "simpleSoil.h"   
#include "cropManageClass.h" 				

simpleSoil:: ~ simpleSoil ( )
{
	delete theLayers;
};

//!Initialise an instance of this class
/*! 
\param thesoilData pointer to the instance of a class containing the soil parameters
*/
void simpleSoil::Initialise(soilDataClass * thesoilData)  
{
	waterIn = 0.0;
   waterOut = 0.0;
   irrigate= false;
   soilFieldCapacity = 0.0;
   theLayers = new linkList<layerClass>;
	theSoilProperties = new soilDataClass(*thesoilData);;
   //load soil info from soil database
   int num = theSoilProperties->gettheLayers()->NumOfNodes();
   for(int index=0;index<num;index++)
   {
	   layerClass *alayerClass = new layerClass(* theSoilProperties->GetLayerDetails(index));
      soilFieldCapacity +=alayerClass->getfieldCapacity() * alayerClass->getthickness();
   	theLayers->InsertLast(alayerClass);
   }
   layerClass *alayerClass;
   theSnow.Initialise(0.0,100000.0,1.0);
   theSnow.setname("snow");
   alayerClass = gettheLayers()->ElementAtNumber(0);
   theEvaporation.Initialise(10.0, 10.0, 1.0);
   theEvaporation.setname("evaporation");
   //Load the lowest soil layer
   alayerClass = gettheLayers()->ElementAtNumber(num-1);
   double soilDepth = alayerClass->getz_lower();
   double poolDepth;
   alayerClass = gettheLayers()->ElementAtNumber(0);
   double evapDepth = 10.0/(1000.0 * alayerClass->getfieldCapacity());   //evaporation pool has always a capacity of10 millimetres
   //depth of rooting poolis always greater or equal to the depth of evaporation pool
   if (thePlants->getactive())
   {
   	poolDepth = thePlants->getrootDepth();
      if (poolDepth<evapDepth)
      	poolDepth = evapDepth;
   }
   else
 		poolDepth = evapDepth;
//calculate the drainage constant for the rooting pool
   double pooldrainageConst = theSoilProperties->getdrainageConst() +  (1 - theSoilProperties->getdrainageConst())
   										* (soilDepth - poolDepth)/soilDepth;
   //initialise the rooting pool
   theRooting.Initialise(10.0,10.0,pooldrainageConst);
   theRooting.setname("rooting");
   double availWater = 0.0;
   //calculate the thickness of the different layers
   double thickness;
   for(int index=0;index<num;index++)
   {
	   alayerClass = gettheLayers()->ElementAtNumber(index);
      thickness = alayerClass->getthickness();
   	availWater+=thickness * alayerClass->getfieldCapacity();
   }
   waterIn+=availWater;

   //note initial value of poolDepth is depth of rooting pool
   poolDepth = soilDepth - poolDepth;
   //calculate the drainage constant for the subroot zone
   pooldrainageConst = theSoilProperties->getdrainageConst() +  (1 - theSoilProperties->getdrainageConst())
   										* poolDepth/soilDepth;
   theSubZone.Initialise(availWater-10.0,availWater-10.0, pooldrainageConst);
   theSubZone.setname("subzone");
   //set the upper rooting to zero
   upperRooting.Initialise(0.0,0.0, 0.0);
   upperRooting.setname("upperRooting");
   //initialise the canopy interception pool.
   if (thePlants->getactive())
   	canopyInterception.Initialise(0.0,0.5*thePlants->getLtot(), 1.0);
   else
   	canopyInterception.Initialise(0.0,0.0, 1.0);
   canopyInterception.setname("interception");
}
//! Execute the daily functions
/*!
\param potEvap potential evapotranspiration (millimetres per day)
\param precipIn precipitation (millimetres per day)
\param Tmean mean air temperature (Celsius)
*/
void simpleSoil::dailyRoutine(double potEvap, double precipIn, double Tmean)  
{
//	linkList<double>

	waterIn+=precipIn;
   //update pool status, since rooting depth may have changed
   updatePools();
	double drainage = 0.0;
   //handle snow
   double snowEvap = potEvap;
	drainage=theSnow.dailySnow(&potEvap, precipIn, Tmean);
   snowEvap-=potEvap;   //subtract evaporation from snow from the potential evapotranspiration
   waterOut+=snowEvap;
   ///irrigation
   double irrigationWater = 0.0;
   if (irrigate)
   {
	   irrigationWater = getSMD(getSoilDepth());
      waterIn+=irrigationWater;
      drainage+=irrigationWater;
   }
   double croppotEvap;
   double croppotEvapGreen;
   double soilpotEvap;
   //set interception pool to zeroif no plants present
   if (!thePlants)
   {
		canopyInterception.update(0.0,0.0,0.0,0.0);
   	soilpotEvap = potEvap;
   	croppotEvap = 0.0;
   }
   else
   {
   //partition potevap between crop and soil surface
   	double propToSoil = exp(-thePlants->getattenuationCoeff() * thePlants->getLtot());
   	croppotEvap = potEvap * (1-propToSoil);  //potential evapotranspiration from crop
   	soilpotEvap = potEvap * propToSoil;      //potential evaporation from soil
      //water available for evaporation from canopy includes any drainage from snow melt or irrigation
	   double availWater = canopyInterception.getvolume() + drainage;
      if (availWater < croppotEvap)
      {
         canopyInterception.update(drainage,0.0,availWater, 0.0);
         croppotEvap-=availWater;
         waterOut+=availWater;
         drainage = 0.0;
      }
      else
      {
         drainage = canopyInterception.update(drainage,0.0,croppotEvap, 0.0);
         waterOut+=croppotEvap;
         croppotEvap=0.0;
      }
   }
	if (thePlants->getLtot()>0.0)   //calculate potential transpiration from green leaf
   	croppotEvapGreen = croppotEvap * thePlants->getLg()/thePlants->getLtot();
   else
   	croppotEvapGreen = 0.0;
   double canopyDrainage = drainage;
   //deal with soil surface layer
   double surfaceAvailWater = theEvaporation.getvolume() + drainage;   //water available for evaporation
   double rootAvailWater = theRooting.getvolume() + drainage;          //water available for evaporation or transpiration
   double evap = 0.0;
   double capillary = 0.0;
   if (surfaceAvailWater > soilpotEvap)  //if there is enough water in the surface to enable evaporation at the potential rate
   	evap = soilpotEvap;
	else  //insufficient water available in surface to enable evaporation at the potential rate
   {
   	if (rootAvailWater > 0.0)  //there is water in the rooting pool
      {
         capillary = 0.15 * (soilpotEvap - surfaceAvailWater);   //calculate the evaporation supported by capillary flow
         evap = capillary + surfaceAvailWater;
      }
	   else
         evap = surfaceAvailWater;    //no capillary flow
   }
   waterOut+=evap;

   if (capillary > 0.0)  //if there has been capillary flow
   {
	   theEvaporation.update(drainage,capillary,evap, 0.0);    //update the evaporation pool
   	if (evap <=(theRooting.getvolume()+drainage))
      {
	      drainage=theRooting.update(drainage,0.0,evap,0.0);
      	drainage=theSubZone.update(drainage,0.0,0.0,0.0);
      }
      else
      {
         double fromRooting = theRooting.getvolume();    //capillary flow provided from the rooting zone
         double fromSubZone = evap - (fromRooting + drainage); //capillary flow provided from below the rooting zone
         drainage=theRooting.update(drainage,fromSubZone,evap,0.0);  //update rooting zone
         drainage=theSubZone.update(drainage,fromSubZone,fromSubZone,0.0);  //update sub rooting zone
         waterIn+=fromSubZone;
      }
   }
   else   //update all the soil pools
   {
	   theEvaporation.update(drainage,0.0,evap, 0.0);
      drainage=theRooting.update(drainage,0.0,evap,0.0);
      drainage=theSubZone.update(drainage,0.0,0.0,0.0);
   }
   waterOut+=drainage;

   //now calculate transpiration
   double transpire = 0.0;
   if (thePlants)
   {
      if (theRooting.getvolume()>=thePlants->getbreakpoint() * theRooting.getmaxVolume())    //if the water available in the rooting zone is sufficient to allow transpiration to perceive that the maximum rate
      {
      	//zero the up a routine pool
	   	upperRooting.setvolume(0.0);
	   	upperRooting.setmaxVolume(0.0);
      }
      else  //calculate contents of the upper rooting pool
      {
	      upperRooting.setmaxVolume(min(theRooting.getmaxVolume(),(upperRooting.getmaxVolume() + canopyDrainage - evap)));
         if (upperRooting.getmaxVolume()<0.0)
         	upperRooting.setmaxVolume(0.0);
   	   upperRooting.setvolume(upperRooting.getvolume() + canopyDrainage - evap);
         if (upperRooting.getvolume()<0.0)
	   	   upperRooting.setvolume(0.0);
      }
      if ((upperRooting.getvolume()>=thePlants->getbreakpoint() * upperRooting.getmaxVolume()) &&
      		(upperRooting.getvolume()>=croppotEvapGreen))     //if there is sufficient water in the upper rooting pool to enable transpiration to proceed at the central rate
      {     //take no action
      }
      else  //set up a rooting pool to 0
      {
	   	upperRooting.setvolume(0.0);
	   	upperRooting.setmaxVolume(0.0);
      }
		if ((upperRooting.getvolume()>0.0)||(theRooting.getvolume()>thePlants->getbreakpoint() * theRooting.getmaxVolume()))
      	//then transpiration can precede at the potential rate
      	transpire = croppotEvapGreen;
      else    //transpiration will be at a rate lower than the potential
      	if ((upperRooting.getvolume()==0.0)&&(theRooting.getvolume()>0.0)
         		&&(theRooting.getvolume()<(thePlants->getbreakpoint() * theRooting.getmaxVolume())))
               //transpiration rate is reduced to a fraction of the potential
               transpire = (theRooting.getvolume()/(thePlants->getbreakpoint() * theRooting.getmaxVolume())) * croppotEvapGreen;
         else  //there will be no transpiration
         	transpire = 0.0;

      if (upperRooting.getvolume()>transpire)   //update up a rooting pool
         upperRooting.setvolume(upperRooting.getvolume()-transpire);
      else
         upperRooting.setvolume(0.0);

      double waterFromEvaporationPool = 0.0;
      if (theRooting.getvolume()>0.0)
      	//calculate the contribution to transpiration from the evaporation will
      	waterFromEvaporationPool = transpire * theEvaporation.getvolume()/theRooting.getvolume();
      theEvaporation.setvolume(theEvaporation.getvolume()-waterFromEvaporationPool);     //update evaporation pool
      theRooting.setvolume(theRooting.getvolume()-transpire);     //update rooting pool
		theRooting.settranspiration(transpire);
	   waterOut+=transpire;
      if (theEvaporation.getvolume()<0.000000001)  //set evaporation pool to 0 (limited precision means that calculations can lead to very small negative values)
	      theEvaporation.setvolume(0.0);
	}

   checkBudget();    //check for water budget closure
}
//!Calculate the capacity of a soil water pool
/*! 
\param startDepth depth below the soil surface of the upper boundary of the pool (millimetres)
\param endDepth depth below the soil surface of the lower boundary of the pool (millimetres)
*/
double simpleSoil::calcPoolVolume(double startDepth, double endDepth)  
{
	double ret_val = 0.0;
   int num = gettheLayers()->NumOfNodes();
   double z_upper=0.0;
   for(int index=0;index<num;index++)
   {
	   layerClass *alayerClass = gettheLayers()->ElementAtNumber(index);
   	double z_lower = alayerClass->getz_lower();
      if ((startDepth <= z_upper)&&(endDepth >= z_lower))  //whole layer is included
			ret_val+= (z_lower - z_upper) * alayerClass->getfieldCapacity();
      if ((endDepth > z_upper)&&(endDepth < z_lower))  //upper part of layer included only
			ret_val+= (endDepth - z_upper) * alayerClass->getfieldCapacity();
      if ((startDepth > z_upper)&&(startDepth < z_lower))  //lower part of layer included only
			ret_val+= (z_lower - startDepth) * alayerClass->getfieldCapacity();
      z_upper=alayerClass->getz_lower();
   }
  return ret_val;
}
//!Adjust the variables and parameters of the soil pool in response to a tillage operation
/*! 
\param depth depth to which the soil is tilled (millimetres)
*/
void simpleSoil::tillage(double depth)  
{

//checkBudget();
   layerClass *alayerClass = gettheLayers()->ElementAtNumber(0);
	double z_lower = alayerClass->getz_lower();
   double depthRootingPool = theRooting.getmaxVolume()/alayerClass->getfieldCapacity();  //depth of rooting pool may be > rooting depth (if == to evaporation pool)
   double newMoistVal;
   if (depth > z_lower)
   	theMessage->FatalError("tillage depth is greater than the depth of the uppermost soil layer");
   if (depth > depthRootingPool)  //then tillage will affect all of the rooting pool and some of the sub zone pool
   {
      z_lower = gettheLayers()->ElementAtNumber(gettheLayers()->NumOfNodes()-1)->getz_lower();
      double depthSubZonePool = z_lower - depthRootingPool;
      double oldRootZoneVol = theRooting.getvolume();
      double oldRootZoneMaxVol = theRooting.getmaxVolume();
      double oldSubZoneVol = theSubZone.getvolume();
      double oldSubZoneMaxVol = theSubZone.getmaxVolume();
      //new moisture value for the rooting pool is calculated as the weighted average of the rooting and sub rooting pools
   	double tempVol= oldRootZoneVol + (depth - depthRootingPool) * oldSubZoneVol/depthSubZonePool;
      double tempMaxVol = oldRootZoneMaxVol + (depth - depthRootingPool) * oldSubZoneMaxVol/depthSubZonePool;
      double newMoistVal =tempVol/tempMaxVol;
		theRooting.setmaxVolume(10.0);
   	double newRootingVol = newMoistVal * theRooting.getmaxVolume();
      theRooting.setvolume(newRootingVol);
      double newEvapVol = newMoistVal * theEvaporation.getmaxVolume();
      theEvaporation.setvolume(newEvapVol);
      //now calculate new moisture value for the sub zone
      theSubZone.setmaxVolume(oldRootZoneMaxVol + oldSubZoneVol - theRooting.getmaxVolume());
   	theSubZone.setvolume(oldSubZoneVol + oldRootZoneVol - theRooting.getvolume());
   }
   else  //there will be no change in the rooting pool but a change in the evaporation pool only
   {
   	newMoistVal =theRooting.getvolume()/theRooting.getmaxVolume();
      double newEvapVol = newMoistVal * theEvaporation.getmaxVolume();
      theEvaporation.setvolume(newEvapVol);
   }
//   checkBudget();
}
//! Update the soil pools to account for changes in the crop rooting depth
void simpleSoil::updatePools()  
{
// update pools
	double rootingDepth = 0.0;
   double newrootCapacity;
   if (thePlants)
   {
   	rootingDepth =thePlants->getrootDepth();
      if (rootingDepth>theSoilProperties->getmaxRootingDepth()) //rooting depth cannot exceed the maximum rooting depth of the soil
      {
			rootingDepth=theSoilProperties->getmaxRootingDepth();
         thePlants->setrootDepth(rootingDepth);
      }
      newrootCapacity=calcPoolVolume(0,rootingDepth); //recalculate the capacity of the rooting pool
      if (newrootCapacity<10.0)
      	newrootCapacity=10.0;
      canopyInterception.setmaxVolume(0.5 * thePlants->getLtot()); //update the capacity of the canopy interception pool
   }
   else
	  	newrootCapacity=theEvaporation.getmaxVolume();
   double oldrootCapacity = theRooting.getmaxVolume();
	theRooting.setmaxVolume(newrootCapacity);
   double oldrootVolume = theRooting.getvolume();
   double oldSubZonecapacity = theSubZone.getmaxVolume();
   double oldSubZonevolume = theSubZone.getvolume();
   if (newrootCapacity<=oldrootCapacity)   //if the rooting depth is decreasing
   {
   	theRooting.setvolume(oldrootVolume + (newrootCapacity - oldrootCapacity)* oldrootVolume/oldrootCapacity);
      if (theRooting.getvolume()<10.0)
	      theRooting.setvolume(10.0);
   }
   else  //the rooting pool will inherit water from the sub zone pool
   	theRooting.setvolume(oldrootVolume + (newrootCapacity - oldrootCapacity)* oldSubZonevolume/oldSubZonecapacity);

   layerClass * alayerClass;
   double poolDepth;
   alayerClass = gettheLayers()->ElementAtNumber(0);
   double evapDepth = 10.0/(1000.0 * alayerClass->getfieldCapacity());
   alayerClass = gettheLayers()->ElementAtNumber(gettheLayers()->NumOfNodes()-1);
   if (thePlants)
   {
   	poolDepth = thePlants->getrootDepth();
      if (poolDepth<evapDepth)
      	poolDepth = evapDepth;
   }
   else
 		poolDepth = evapDepth;
   double soilDepth = alayerClass->getz_lower();
   //recalculate the rooting pool drainage constant
   double pooldrainageConst = theSoilProperties->getdrainageConst() +  (1 - theSoilProperties->getdrainageConst())
   										* (soilDepth - poolDepth)/soilDepth;
   theRooting.setdrainageConst(pooldrainageConst);
   //note initial value of poolDepth is depth of rooting pool
   poolDepth = soilDepth - poolDepth;
   //recalculate the sub zone drainage constant
   pooldrainageConst = theSoilProperties->getdrainageConst() +  (1 - theSoilProperties->getdrainageConst())
   										* poolDepth/soilDepth;
	theSubZone.setdrainageConst(pooldrainageConst);

   //recalculate volume and capacity of the sub zone
	theSubZone.setvolume(oldSubZonevolume - (theRooting.getvolume() - oldrootVolume));
   double fieldCap = 0.0;
   double thickness;
   for(int index=0;index<gettheLayers()->NumOfNodes();index++)
   {
	   layerClass *alayerClass = gettheLayers()->ElementAtNumber(index);
      thickness = alayerClass->getthickness();
   	fieldCap+=thickness * alayerClass->getfieldCapacity();
   }
   theSubZone.setmaxVolume(fieldCap - theRooting.getmaxVolume());
   double newupperRootingCapacity = min(theRooting.getmaxVolume(),upperRooting.getvolume());
   if (newupperRootingCapacity>0.0)
	   upperRooting.setvolume(upperRooting.getvolume() - (upperRooting.getmaxVolume() - newupperRootingCapacity)
   		*upperRooting.getvolume()/upperRooting.getmaxVolume());
   else
   	upperRooting.setvolume(0.0);
   upperRooting.setmaxVolume(newupperRootingCapacity);
}
//! Reports an error if the soil water budget cannot be closed
void simpleSoil::checkBudget()  
{
	double waterInSystem = 0.0;
   waterInSystem+=canopyInterception.getvolume();
   waterInSystem+=theRooting.getvolume();
   waterInSystem+=theSubZone.getvolume();
  	double error = waterInSystem + waterOut - waterIn;
   if (error > 0.0000001)
   {
   	cout << " error " <<  error;
   }
}
//!Outputs the soil variables to a file
/*! 
\param afile pointer to the output file
\param header set to true if only the variable names should be printed
*/
void simpleSoil::outputDetails(fstream * afile, bool header)  
{
 theSnow.outputDetails(afile, header);
 canopyInterception.outputDetails(afile, header);
 theEvaporation.outputDetails(afile, header);
 theRooting.outputDetails(afile, header);
 theSubZone.outputDetails(afile, header);
 upperRooting.outputDetails(afile, header);
}
//!Returns the soil moisture deficit to a given depth
/*! 
\param depth depth below the soil surface for which the soil moisture deficit should be calculated
*/
double simpleSoil::getSMD(double depth)  
{
	double ret_val=0.0;
   layerClass * alayerClass = gettheLayers()->ElementAtNumber(0);
   double evapDepth = 10.0/(1000.0 * alayerClass->getfieldCapacity());   //evaporation pool has always a capacity of10 millimetres
   if (depth <=evapDepth)   //retrieve the deficit in the evaporation pool
   	ret_val = theEvaporation.getDeficit();
   else
   {
      double rootingDepth = 0.0;
      if (thePlants)
      	rootingDepth = thePlants->getrootDepth();
      if (rootingDepth > depth)   //only a rooting pool is involved
         ret_val =  theRooting.getDeficit();
      else      //both the rooting and the sub zone pools are involved
         ret_val =  (rootingDepth * theRooting.getDeficit() + (depth - rootingDepth) * theSubZone.getDeficit())/depth;
   }
	return ret_val;
}
//! Tidy up his class
void simpleSoil::closeDown()  
{
	theLayers->Reset();
   delete theSoilProperties;
}
//! Return the depth of the soil
double simpleSoil::getSoilDepth()  
{
	layerClass * alayerClass = gettheLayers()->ElementAtNumber(gettheLayers()->NumOfNodes()-1);
   return alayerClass->getz_lower();
}

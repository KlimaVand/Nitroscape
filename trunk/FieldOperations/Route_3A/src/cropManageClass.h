// Class: cropManageClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __CROPMANAGECLASS_H  //Required for current class
  #define __CROPMANAGECLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include <linkList.h>        //Required for include files

#include <fieldOrderPeriod.h>        //Required for include files
                               
/*! This class reads the crop management details for a single field for all years of the simulation.
*/

class cropManageClass  : public base        
{                       
			
  //! Name of the crop		
     string cropName; //Attribute data member
   //! List of tillage operations		
     linkList<fieldOrderPeriod> * TillagePeriods; //Attribute data member
   //! List of sowing operations		
     linkList<fieldOrderPeriod> * SowPeriods; //Attribute data member
   //! List of fertiliser or manure application operations		
     linkList<fieldOrderPeriod> * FertilizerPeriods; //Attribute data member
   //!List of harvesting operations		
     linkList<fieldOrderPeriod> * HarvestPeriods; //Attribute data member
   //! List of cutting operations		
     linkList<fieldOrderPeriod> * CuttingPeriods; //Attribute data member
   //! List of occasions when grazing commences		
     linkList<fieldOrderPeriod> * GrazingStartPeriods; //Attribute data member
   //! Listed occasions when grazing ceases		
     linkList<fieldOrderPeriod> * GrazingStopPeriods; //Attribute data member
   //! List of tillage depths corresponding to the tillage operations		
     linkList<double> * TillageDepth; //Attribute data member
   //! List of leaf area indices post-grazing, corresponding to the grazing periods		
     linkList<double> * GrazingLAI; //Attribute data member
   //! List of types of fertiliser or manure, corresponding to the fertiliser periods		
     linkList<string> * FertilizerType; //Attribute data member
   //! List of amounts of fertiliser or manure, corresponding to the fertiliser or manure application periods		
     linkList<double> * FertilizerAmount; //Attribute data member
   //! List of periods for starting irrigation		
     linkList<fieldOrderPeriod> * IrrigationStartPeriods; //Attribute data member
   //! List of periods for ending irrigation		
     linkList<fieldOrderPeriod> * IrrigationStopPeriods; //Attribute data member
   //! Threshold soil moisture deficit for irrigation (proportion of field capacity)		
     linkList<double> * IrrigationThreshold; //Attribute data member        

private:

protected:

public:

			//Default constructor
  cropManageClass () {};

//! Read the list of field operations for a single field					
   void ReadOperations ()    ;

//!Creates the arrays used to store the details of the field operations and then reads and stores the data.
/*! 
\param afile pointer to a file containing data
\param anIndex number of the field to which the operations should be applied
*/					
   void Initialise (fstream * afile, int anIndex)    ;

//! Tidy up lists of operations					
   void CloseDown ()    ;
 
   ~ cropManageClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  string getcropName() const  { return cropName;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getTillagePeriods() const  { return TillagePeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getSowPeriods() const  { return SowPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getFertilizerPeriods() const  { return FertilizerPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getHarvestPeriods() const  { return HarvestPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getCuttingPeriods() const  { return CuttingPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getGrazingStartPeriods() const  { return GrazingStartPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getGrazingStopPeriods() const  { return GrazingStopPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<double> * getTillageDepth() const  { return TillageDepth;}

                       //!Get accessor function for non-static attribute data member
  linkList<double> * getGrazingLAI() const  { return GrazingLAI;}

                       //!Get accessor function for non-static attribute data member
  linkList<string> * getFertilizerType() const  { return FertilizerType;}

                       //!Get accessor function for non-static attribute data member
  linkList<double> * getFertilizerAmount() const  { return FertilizerAmount;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getIrrigationStartPeriods() const  { return IrrigationStartPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<fieldOrderPeriod> * getIrrigationStopPeriods() const  { return IrrigationStopPeriods;}

                       //!Get accessor function for non-static attribute data member
  linkList<double> * getIrrigationThreshold() const  { return IrrigationThreshold;}
                       
                       //!Set accessor function for non-static attribute data member
  void setcropName (string acropName)  { cropName = acropName;}

                       //!Set accessor function for non-static attribute data member
  void setTillagePeriods (linkList<fieldOrderPeriod> * aTillagePeriods)  { TillagePeriods = aTillagePeriods;}

                       //!Set accessor function for non-static attribute data member
  void setSowPeriods (linkList<fieldOrderPeriod> * aSowPeriods)  { SowPeriods = aSowPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setFertilizerPeriods (linkList<fieldOrderPeriod> * aFertilizerPeriods)  { FertilizerPeriods = aFertilizerPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setHarvestPeriods (linkList<fieldOrderPeriod> * aHarvestPeriods)  { HarvestPeriods = aHarvestPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setCuttingPeriods (linkList<fieldOrderPeriod> * aCuttingPeriods)  { CuttingPeriods = aCuttingPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setGrazingStartPeriods (linkList<fieldOrderPeriod> * aGrazingStartPeriods)  { GrazingStartPeriods = aGrazingStartPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setGrazingStopPeriods (linkList<fieldOrderPeriod> * aGrazingStopPeriods)  { GrazingStopPeriods = aGrazingStopPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setTillageDepth (linkList<double> * aTillageDepth)  { TillageDepth = aTillageDepth;}

                       //!Set accessor function for non-static attribute data member
  void setGrazingLAI (linkList<double> * aGrazingLAI)  { GrazingLAI = aGrazingLAI;}

                       //!Set accessor function for non-static attribute data member
  void setFertilizerType (linkList<string> * aFertilizerType)  { FertilizerType = aFertilizerType;}

                       //!Set accessor function for non-static attribute data member
  void setFertilizerAmount (linkList<double> * aFertilizerAmount)  { FertilizerAmount = aFertilizerAmount;}

                       //!Set accessor function for non-static attribute data member
  void setIrrigationStartPeriods (linkList<fieldOrderPeriod> * aIrrigationStartPeriods)  { IrrigationStartPeriods = aIrrigationStartPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setIrrigationStopPeriods (linkList<fieldOrderPeriod> * aIrrigationStopPeriods)  { IrrigationStopPeriods = aIrrigationStopPeriods;}

                       //!Set accessor function for non-static attribute data member
  void setIrrigationThreshold (linkList<double> * aIrrigationThreshold)  { IrrigationThreshold = aIrrigationThreshold;}
       
};
			
#endif

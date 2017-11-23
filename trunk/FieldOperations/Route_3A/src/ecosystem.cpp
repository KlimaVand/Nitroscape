
 
// Class: ecosystem

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "ecosystem.h"   
#include "soilDataClass.h" 				

ecosystem:: ~ ecosystem ( )
{
	delete theCropChars;
};

//!Simulate daily processes of the ecosystem
/*! 
\param potEvap potential evapotranspiration (millimetres)
\param precip precipitation (millimetres)
\param Tmean mean air temperature (Celsius)
\param autumn set to true if the plant model should enter autumn mode
*/
void ecosystem::dailyRoutine(double potEvap, double precip, double Tmean, bool autumn)  
{	theSoil.dailyRoutine(potEvap, precip, Tmean);
   thePlants.dailyRoutine(Tmean,autumn);
}
//!Initialise the ecosystem model
/*! 
\param soilType number of the soil to be used by the ecosystem model
*/
void ecosystem::Initialise(int soilType)  
{
   thePlants.Initialise(false);
   theSoil.setthePlants(&thePlants);
   //read the characteristics for all soil types
	soilCharClass * theSoilChars = new soilCharClass();
	theSoilChars->Initialise("SoilProps.txt");
   theSoil.Initialise(theSoilChars->GetSoilData(soilType));
   delete theSoilChars;

   theCropChars = new linkList<cropCharClass>;
   //read crop characteristics for all crops
   char * acropCharFileName = "cropChars.txt";
   OpenInputFile(acropCharFileName);
   int first, num;
	GetSectionNumbers("Crop",&first,&num);
   for(int index=first;index<(first+num);index++)
   {
   	cropCharClass * acropCharClass = new cropCharClass();
      acropCharClass->Initialise(file, index);
   	theCropChars->InsertLast(acropCharClass);
   }
   CloseInputFile();
}
//!Output state variables to file
/*! 
\param file pointer to output file
\param header set to true if name of variables should be printed
*/
void ecosystem::outputDetails(fstream * file, bool header)  
{
	if (header)
		*file << "crop" << "\t";
   else
		*file << *thePlants.getName() << "\t";
	theSoil.outputDetails(file, header);
  	thePlants.outputDetails(file, header);
}
//!Returns the crop parameters for a given crop
/*! 
\param cropName name of the crop
*/
cropCharClass * ecosystem::FindCropChars(const string cropName)  
{
   cropCharClass * acropCharClass = NULL;
   int count = 0;
   while ((count<theCropChars->NumOfNodes()) && (acropCharClass==NULL))
   {
   	if (theCropChars->ElementAtNumber(count)->getName()==cropName)
	   	acropCharClass = theCropChars->ElementAtNumber(count);
      count++;
   }
   if (!acropCharClass)
   	theMessage->FatalError("Crop not found");
   else
   	return acropCharClass;
}
//!Harvest the current crop
/*! 
\param residualLAI leaf area index remaining after harvesting
*/
void ecosystem::harvest(double residualLAI)  
{
	thePlants.harvest(residualLAI);
   theSoil.updatePools();
}
//!Harvest the current crop by cutting
/*!
\param residualLAI leaf area index of the crop after cutting
*/

void ecosystem::cut(double residualLAI)  
{
	thePlants.cut(residualLAI);
}
//!Sow a given crop
/*! 
\param cropName name of the crop to be sown
\param autumn sets to true if the crop should enter autumn mode
*/
void ecosystem::sow(string cropName, bool autumn)  
{
	cropCharClass * acropCharClass = FindCropChars(cropName);
   thePlants.loadCropChars(acropCharClass);
   thePlants.setuseAutumn(autumn);
}
//!Till the soil
/*! 
\param depth depth to which the soil should be tilled
*/
void ecosystem::tillage(double depth)  
{
	theSoil.tillage(depth);
	thePlants.kill();
}
//!Add fertiliser. 
/*! Note:Type and amount of fertiliser are not currently used. Included here for future use.
\param aType type of fertiliser to be applied
\param anAmount the amount of fertiliser to be applied
*/
void ecosystem::fertilise(string aType, double anAmount)  
{}
//! Return the name of the current crop
string ecosystem::getCropName()  
{
	return *thePlants.getName();
}
//!Returns the current soil moisture deficit (in millimetres)
/*! 
\param depth soil depth to which the soil moisture deficit should be calculated
*/
double ecosystem::getSMD(double depth)  
{
	return theSoil.getSMD(depth);
}
//! Tidy up ecosystem model
void ecosystem::closeDown()  
{
   theCropChars->Reset();
   theSoil.closeDown();
   thePlants.closeDown();
}
//! Return the soil depth (millimetres)
double ecosystem::getSoilDepth()  
{
 return theSoil.getSoilDepth();
}
//! Return the plant-available water to maximum rooting depth at field capacity (millimetres)
double ecosystem::getFieldCapacity()  
{
 return theSoil.getsoilFieldCapacity();
}
/*! Tell the soil whether irrigation should occur or not
\param toIrrigate is set to true if irrigation should occur
*/
void ecosystem::irrigate(bool toIrrigate)  
{	theSoil.setirrigate(toIrrigate);
}

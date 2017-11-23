

// Class: locationClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "locationClass.h"   
#include "trafworkCharClass.h" 				

locationClass:: ~ locationClass ( )
{
	delete theFields;
	delete theCropOps;
};

//! Initialises the model for a given location
/*!
\param aclimateFileName name of a climate file
\param outputFile pointer to an output file
\param aTrafworkdb pointer to a class instance containing the workability and trafficability parameters for all soil types
*/
void locationClass::Initialise(int anNCU, char * aclimateFileName, fstream * outputFile, trafworkCharClass * aTrafworkdb)
{
   theFields = new linkList<field>;
   theCropOps = new linkList<cropManageClass>;
   NCU = anNCU;
   outfile =  outputFile;
   int climateFileType = 6;
   aClimate = new climateClass();
   aClimate->InitClimate("",aclimateFileName,climateFileType);
   int first, num;
   //read crop management details
   int fieldCount=0;
   char * rootString = "field";
   char * tailString = "txt";
   char * cropManagementFileName = "field0.txt";
   int theSoilType=0;
   while (OpenInputFile(cropManagementFileName))
   {
   	GetParameter("SoilType",&theSoilType);
      GetSectionNumbers("Crop",&first,&num);
      for(int index=first;index<(first+num);index++)
      {
         cropManageClass * acropManageClass = new cropManageClass();
         acropManageClass->Initialise(file, index);
         theCropOps->InsertLast(acropManageClass);
      }
      CloseInputFile();
      fieldCount++;
      strcpy(cropManagementFileName,rootString);
		sprintf(cropManagementFileName,"(%d)",fieldCount);
      strcat(cropManagementFileName,tailString);
   }
   delete file;
   file=NULL;
   //initialise all fields
   for(int index=0;index<fieldCount;index++)
   {
   	field * afield = new field("",index,this);
      afield->Initialise(index,theSoilType,theCropOps,aTrafworkdb->GetSoilData(theSoilType),outputFile);
   	theFields->InsertLast(afield);
   }
   Tsum=0.0;
}
//! Perform the daily functions for this location
void locationClass::dailyRoutine()  
{
   aClimate->Update();
	double Tmean = aClimate->GetMeanTemperature();
   if ((theTime.GetDay()==1) && (theTime.GetMonth()==1))
   	Tsum=0.0;
   if (Tmean > 0.0)
		Tsum+=Tmean ;
   int num = theFields->NumOfNodes();
   for(int index=0;index<num;index++)
   {
   	field * afield = theFields->ElementAtNumber(index);
   	afield->dailyRoutine(aClimate->GetEPot(),aClimate->Getprecip(),Tmean,Tsum);
   }
}
//! Tidy up
void locationClass::closedown()
{
   for(int index=0;index<theCropOps->NumOfNodes();index++)
   {
   	cropManageClass * acropManageClass;
      acropManageClass = theCropOps->ElementAtNumber(index);
   	acropManageClass->CloseDown();
   }
   for(int index=0;index<theFields->NumOfNodes();index++)
   {
   	field * afield = theFields->ElementAtNumber(index);
   	afield->closeDown();
   }
   theCropOps->Reset();
   delete aClimate;
}
//!Output details of the model at this location
/*! 
\param afile a pointer to the output file
\param header set to true if only variable names should be printed to file
*/
void locationClass::outputDetails(fstream * afile, bool header)  
{
   if (header)
   	*afile << "epot" << "\t" << "precip" << "\t" << "Tsum" << "\t"  << "Tmean" << "\t";
   else
   	*afile << aClimate->GetEPot() << "\t" << aClimate->Getprecip()<< "\t" << aClimate->GetdegreeDays()
      		<< "\t" << aClimate->GetMeanTemperature()<< "\t";
   int num = theFields->NumOfNodes();
   for(int index=0;index<num;index++)
   {
   	field * afield = theFields->ElementAtNumber(index);
   	afield->outputDetails(afile, header);
   }
}

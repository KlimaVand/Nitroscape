/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../Nitroscape/MMSClass.h"
#include "../Nitroscape/harvInfoClass.h"
#include "../base/timeuse.h"
#include "../data/globalData.h"
#include "../data/outputData.h"
#include <string.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>




void MMSClass::InitialiseInterface(eventControl *aEventControl)
{

   int numFields = aEventControl->getCropRotation()->GetFields()->NumOfNodes();
   for (int j=0; j<numFields;j++)
   {
      field *aField = aEventControl->getCropRotation()->GetFields()->ElementAtNumber(j);
      aField->UpdatePatch(farmNo);

   }
   CloseInputFile();
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].location=myBuildings->GetStable(i)->Getlocation();
   	houseEmission[i].name=myBuildings->GetStable(i)->GetAlias();
   }
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes(); i++)
   {
   //I have a feeling that the following cast is a bit naughty
   	storeEmission[i].location=((building *) myBuildings->GetManurestore(i))->Getlocation();
   	storeEmission[i].name=myBuildings->GetManurestore(i)->GetAlias();
   }
}

void MMSClass::zeroPatchOutputVariables()
{

	//for(int i=0;i<fieldList.size();i++)
	{
	//	fieldList.at(i)->zeroOutputVariables();
	}
}

void MMSClass::zeroFarmOutputVariables()
{
   for (int i=0; i<theBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].NH3Emission=0.0;
   	houseEmission[i].N2OEmission=0.0;
   }
}

double MMSClass::GetMMSHouseAmmonia(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].NH3Emission;
	return retVal;
}

double MMSClass::GetMMSHouseAmmonia()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].NH3Emission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSHouseN2O(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].N2OEmission;
	return retVal;
}

double MMSClass::GetMMSHouseN2O()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].N2OEmission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSHouseNO(int houseNo)
{
	double retVal=0.0;
   retVal=houseEmission[houseNo].NOEmission;
	return retVal;
}

double MMSClass::GetMMSHouseNO()
{
	double retVal=0.0;
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes();i++)
   {
	   retVal+=houseEmission[i].NOEmission;
	}
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreAmmonia(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].NH3Emission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].NH3Emission;
	return ret_val*1000.0;
}

double MMSClass::GetMMSStoreN2O(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].N2OEmission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreN2O()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].N2OEmission;
	return ret_val*1000.0;
}

double MMSClass::GetMMSStoreNO(int storeNo)
{
	double retVal=0.0;
   retVal=storeEmission[storeNo].NOEmission;
	return retVal*1000.0;
}

double MMSClass::GetMMSStoreNO()
{
	double ret_val=0.0;
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes();i++)
      ret_val +=storeEmission[i].NOEmission;
	return ret_val*1000.0;
}

/****************************************************************************\
\****************************************************************************/
//void MMSClass::initialise_MMS(string inputDirectory, string climateDirectory, string climateFileName, int climateFileType, string outputDirectory)
void MMSClass::initialise_MMS(int farmNo,string orgPath)
{
	std::string
	   ClimateDirectory,
	   ClimateFileName,
	   baseOutputDir;
   //set global to local, otherwise outputData, farmData and environData will get data from the wrong place
   theGlobalData = new globalData();

   fileAccess hd;
   hd.changeDir(orgPath);
   theClimate=NULL;
   base aBase;
   aBase.OpenInputFile("system.dat");
   string InputDirectoryS, OutputDirectoryS;
   aBase.SetCritical();
   aBase.FindSection("run",farmNo);
   aBase.GetParameter("InputDirectory",&InputDirectoryS);
   aBase.GetParameter("OutputDirectory",&OutputDirectoryS);
   aBase.GetParameter("ClimateDirectory",&ClimateDirectory);
   aBase.GetParameter("ClimateFileName",&ClimateFileName);
   aBase.UnsetCritical();
   cout<<OutputDirectoryS<<endl;
   cout<<OutputDirectoryS<<endl;
   theGlobalData->setOutputDirectory(OutputDirectoryS);
   theGlobalData->setIndputDir(InputDirectoryS);
   theGlobalData->setFarmNr(farmNo);
   strcpy(myInputDirectory,theGlobalData->getInputDirectory().c_str());
   strcpy(myOutputDirectory,theGlobalData->getOutputDirectory().c_str());
   myOutputData = new outputData();
     myFarmData = new farmData();
     myEnvironData = new environData();
     theOutputData=new outputData();
   for (int i=0; i<20; i++)
   {
		houseEmission[i].NH3Emission=0.0;
		storeEmission[i].NH3Emission=0.0;
		houseEmission[i].N2OEmission=0.0;
		storeEmission[i].N2OEmission=0.0;
		houseEmission[i].NOEmission=0.0;
		storeEmission[i].NOEmission=0.0;
   }


   std::string EconomicIndicatorDate="1/1";
   std::string EnvironmentalIndicatorDate="1/1";
   std::string WaterFlowMethod="";

   int ClimateFileType = 0;
   aBase.GetParameter("ClimateFileType",&ClimateFileType);
   int EpotCalculationMethod = 0;

   int VolatModel=0;
   int singleField=-1;
   int fertiliserUnit=0;
   int StartYear,StopYear;
   aBase.SetCritical();
   aBase.GetParameter("StartYear",&StartYear);
   aBase.GetParameter("StopYear",&StopYear);
   aBase.UnsetCritical();
	myTime = new bsTime(1, 8,StartYear);
	stopTime = new bsTime(1,8,StopYear);
   simYear =0;
   //  bool SellPlantProducts=false;
   bool
   UseReductionParameters=false, 		 // Default determined by theControlParameters
   MeasuredSoilTemperature=false,       // Default determined by theControlParameters
   WriteSoilTemperature=false,
   SimpleTemperatureModel=false,        // Default determined by theControlParameters
   // WriteDetails,                  // Default determined by theControlParameters
   GrassWeed=false,                     // Default determined by theControlParameters
   Organic=false, 					    	 // Default determined by theControlParameters
   ForcedOperations=false,              // Default determined by theControlParameters
   ForcedHarvest=false,
   fieldsFlexSize=false,                // When true, fields are allowed to shrimp or expand
   uniSizeFields=false;                 // When true, all fields are 1 ha big


   myControlParameters = new controlParameters;

   aBase.GetParameter("EnvironmentalIndicator",&EnvironmentalIndicatorDate);
   aBase.GetParameter("EpotCalculationMethod",&EpotCalculationMethod);
   aBase.GetParameter("VolatModel",&VolatModel);
   myControlParameters->SetvolatModel(VolatModel);
//   int wfm=mySystemData->getWaterFlowMethodInt();
//   myControlParameters->SetWaterFlowMethod(wfm);
   if (aBase.GetParameter("SimpleTemperatureModel",&SimpleTemperatureModel))
                  theControlParameters->SetSimpleTemperatureModel(SimpleTemperatureModel);
   if(aBase.GetParameter("AllFieldsOneSize",&uniSizeFields))
                  theControlParameters->SetFieldsUniSize(uniSizeFields);
   if(aBase.GetParameter("FieldsFlexSize",&fieldsFlexSize)) // Allow fields to expand or shrimp
                  theControlParameters->SetFieldsFlexSize(fieldsFlexSize);
   singleField=false;
   if(aBase.GetParameter("SingleField",&singleField))
               {
                  myControlParameters->SetSingleField(singleField);
                  theOutputData->DetailsData.setPlantDetails(true);
                  theOutputData->DetailsData.setSoilDetails(true);
               }
   if(aBase.GetParameter("FertiliserUnit",&fertiliserUnit))
                  myControlParameters->SetFertiliserUnit(fertiliserUnit);
   if (aBase.GetParameter("MeasuredSoilTemperature",&MeasuredSoilTemperature))
             myControlParameters->SetMeasuredSoilTemperature(MeasuredSoilTemperature);


   if (aBase.GetParameter("WriteSoilTemperature",&WriteSoilTemperature))             // Write soil temperature to file for later use in the above procedure (for speed reasons)
                  myControlParameters->SetWriteSoilTemperature(WriteSoilTemperature);
   if (aBase.GetParameter("ForcedOperations",&ForcedOperations))
                  myControlParameters->SetForcedOperations(ForcedOperations);

   ForcedHarvest =ForcedOperations;
   myControlParameters->SetForcedHarvest(ForcedHarvest);
   double HarvestWaste=0.05;

   if(aBase.GetParameter("HarvestWaste",&HarvestWaste))
   {
    myControlParameters->SetHarvestWaste(HarvestWaste); // 0.05 - 0.10 may be a sensible range
   }


   double sl=0.45;
               aBase.GetParameter("StrawLeft",&sl);
               myControlParameters->SetStrawLeft(sl);
               if (aBase.GetParameter("UseReductionParameters",&UseReductionParameters))
                  myControlParameters->SetUseReductionParameters(UseReductionParameters);
               if (aBase.GetParameter("GrassWeed",&GrassWeed))
                  myControlParameters->SetGrassWeed(GrassWeed);
               if (aBase.GetParameter("OrganicFarm",&Organic))
                  myControlParameters->SetOrganicFarm(Organic);





   char FN1[100],FN2[100], FN3[100], FN4[100], FN5[100];
   //               chdir(OutputDirectory);
   strcpy(FN1,InputDirectoryS.c_str());
   strcat(FN1,"warnings.txt");
   strcpy(FN2,InputDirectoryS.c_str());
   strcat(FN2,"logfile.txt");
   strcpy(FN3,InputDirectoryS.c_str());
   strcat(FN3,"debug.txt");      //debug file name
   strcpy(FN4,InputDirectoryS.c_str());
   strcat(FN4,"fieldfile1.txt");
   strcpy(FN5,InputDirectoryS.c_str());
   strcat(FN5,"fieldfile2.txt");

   char string80[80];
    strcpy(string80, InputDirectoryS.c_str());


   myProducts                  = new products;
   myOutput                    = new IndicatorOutput;
   myMessage                   = new message;
   timeConsumption              = new timeUse;
   myLegislation               = new legislate;
   myTechnics                  = new technics("technics",0); // BMP
   myClimate                   = new climate(ClimateFileType,EpotCalculationMethod);
   myBuildings        	        = new buildings;
   myManure_manager            = new manureManager();
   myEventControl = new eventControl();
   SetMMSGlobals();

   myMessage->InitMessage(FN1,FN2,FN3,FN4,FN5);
/*   cout << "Input directory    " << inputDirectory << endl;
   cout << "Output directory   " << outputDirectory << endl;
   cout << "Climate directory  " << climateDirectory << endl;
   cout << "Climate file       " << climateFileName << endl;*/

   if (chdir(myOutputDirectory)!=0)
#ifdef __ANSICPP__
	#ifdef BUILDER
	  mkdir(myOutputDirectory);
	#endif
	#ifdef UNIX
	  mkdir(myOutputDirectory,'-p');
	#endif
#ifdef CYGWIN
	  mkdir(myOutputDirectory,'-p');
#endif
#ifdef __BCplusplus__
	  mkdir(myOutputDirectory);
	#endif                                     				// make output directory if not present
#else
//	#ifdef BUILDER
//	  mkdir(myOutputDirectory,'-p');
/*	#else
	  cout << "Error - directory not found " << endl;
	  cout << errno << endl;
	  cout << "Press any key to exit " << endl;
	  char dum;
	  cin >> dum;
	  exit(99);
	#endif*/
#endif
   if (chdir(myInputDirectory)!=0)
		myMessage->FatalError("Event:: input directory ",myInputDirectory," not found!");

	myEventControl->Initialize(stopTime,(char*)&myInputDirectory,(char*)ClimateDirectory.c_str(),(char*)ClimateFileName.c_str(),(char*)&myOutputDirectory);
	InitialiseInterface(myEventControl);
}

void MMSClass::DoMMSDaily()
{
	SetMMSGlobals();
   chdir(myInputDirectory);
	zeroPatchOutputVariables();
   if ((theTime.GetDay()==30)&&(theTime.GetMonth()==4))
   	cout << "got to 1.5" << endl;
   if ((theTime.GetDay()==1)&&(theTime.GetMonth()==8))
   {
      char fileExtension[4];
      fileExtension[0] = '\0';
      if (simYear<100)
         sprintf(fileExtension,"f%02d",simYear);
      else
         sprintf(fileExtension,"f%03d",simYear);
      myEventControl->ReceivePlan(fileExtension,myInputDirectory);
      simYear++;
   }
   myEventControl->DailyUpdate();
   for (int i=0; i<myBuildings->GettheStables()->NumOfNodes(); i++)
   {
   	houseEmission[i].NH3Emission=myBuildings->GetStable(i)->GetNH3Emission();
   	houseEmission[i].N2OEmission=myBuildings->GetStable(i)->GetN2OEmission();
   	houseEmission[i].NOEmission=myBuildings->GetStable(i)->GetNOEmission();
   }
   for (int i=0; i<myBuildings->GettheManurestores()->NumOfNodes(); i++)
   {
   	storeEmission[i].NH3Emission=myBuildings->GetManurestore(i)->GetNH3Emission();
   	storeEmission[i].N2OEmission=myBuildings->GetManurestore(i)->GetN2OEmission();
   	storeEmission[i].NOEmission=myBuildings->GetManurestore(i)->GetNOEmission();
   }
   if ((myTime->GetDay()==31) && (myTime->GetMonth()==7))
      myEventControl->GiveIndicator(environmentalIndicator,simYear);

   myTime->AddOneDay();
}

void MMSClass::Destroy_MMS()
{
   delete myProducts;
   delete myOutput;
   delete myMessage;
   delete myLegislation;
   delete myTechnics;
   delete myClimate;
   delete myBuildings;
   delete myManure_manager;
   delete myEventControl;
   delete myOutputData;
   delete myTime;
   delete stopTime;
}

void MMSClass::SetMMSGlobals()
{
   theProducts = myProducts;
	theMessage	= myMessage;
//	thetimeConsumption* mytimeConsumption;
	theOutput	= myOutput;
	theLegislation = myLegislation;
	theTechnics	= myTechnics;
	theBuildings	= myBuildings;
	theClimate = myClimate;
	theControlParameters = myControlParameters;
   theOutput = myOutput;
	theManure_manager	= myManure_manager;
   theOutputData = myOutputData;
   theEnvironData = myEnvironData;
   theFarmData = myFarmData;
   theTime = *myTime;
}

void MMSClass::SetEcoStatus(int patchNo, double ReadyForHarvestOrLater,
      double IrrigationDemand, linkList <harvInfoClass> *theHarvestProducts)

{

	myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface()->SetEcoStatus(ReadyForHarvestOrLater,IrrigationDemand,
	   theHarvestProducts);
}

bool MMSClass::GetMMSTillage(int patchNo, double *theType, double *theDepth, double *theFractionIncorporated)
{
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();

	*theType=apatchInterfaceClass->GettheType()+1.0;
   *theDepth=apatchInterfaceClass->GettheDepth();
   *theFractionIncorporated = apatchInterfaceClass->GettheFractionIncorporated();
   apatchInterfaceClass->SettheType(-1);
   if (*theType>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSAddOrganic(int patchNo, double *theType, double *theamount, double *theDepth, double *theTotalN, double *theNH4N,
		double *theNO3N, double *thepercentDM, double *theC)
{
	*theType=0.0;  *theamount=0.0; *theDepth=0.0; *theTotalN=0.0; *theNH4N=0.0; *theNO3N=0.0;*theC=0.0;
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();
   if (apatchInterfaceClass->GetdecompAmount()>0.0)
   {
		*theamount = apatchInterfaceClass->GetdecompAmount();
	   *theDepth=0.0; //apatchInterfaceClass->GettheDepth();
		*theNH4N = apatchInterfaceClass->GetdecompNH4_N();
		*theNO3N = apatchInterfaceClass->GetdecompNO3_N();
		*theTotalN = *theNH4N + *theNO3N + apatchInterfaceClass->GetdecompOrg_N();
		*theC = apatchInterfaceClass->GetdecompC();
		*thepercentDM = apatchInterfaceClass->GetdecompPercentDM();
      *theType = apatchInterfaceClass->GetdecompType();
      apatchInterfaceClass->SetdecompAmount(0.0);
   }
   if (*theamount>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoFertilisation(int patchNo, double *theNH4N, double *theNO3N)
{
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();
	*theNO3N = apatchInterfaceClass->GettheNitrate();
   *theNH4N = apatchInterfaceClass->GettheAmmonium();
   if ((*theNO3N>0.0)||(*theNH4N>0.0))
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoSowing(int patchNo, double *theSeedCode, double *theSeedDM, double *theSeedN)
{
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();
	*theSeedCode = (double) apatchInterfaceClass->GetthecropID();
   *theSeedDM=apatchInterfaceClass->GetamountSeedDM();
   *theSeedN = apatchInterfaceClass->GetseedN();
   apatchInterfaceClass->SetamountSeedDM(0.0);
   if (*theSeedDM>0.0)
	   return true;
   else
	   return false;
}

bool MMSClass::GetMMSDoHarvesting(int patchNo, double *theharvestCode, double *thepropGrainHarvested, double *thepropStrawHarvested)
{
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();
	*theharvestCode = (double) apatchInterfaceClass->GetharvestCode();
   *thepropGrainHarvested=apatchInterfaceClass->GetpropGrainHarvested();
   *thepropStrawHarvested = apatchInterfaceClass->GetpropStrawHarvested();
   apatchInterfaceClass->SetharvestCode(0);
   if (*theharvestCode>0.0)
	   return true;
   else
	   return false;
}

double MMSClass::GetMMSPatchAmmonia(int patchNo)
{
	double ret_val=0.0;
	patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(patchNo)->getPatchInterface();
   ret_val = apatchInterfaceClass->GetNH3Emission();
	return ret_val;
}

double MMSClass::GetMMSSpreadAmmonia()
{
	double ret_val=0.0;
   for (int i=0; i<myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->getPatchList()->NumOfNodes();i++)
   {
      patchInterfaceClass * apatchInterfaceClass  = myEventControl->getCropRotation()->GetFields()->ElementAtNumber(farmNo)->GetPatch(i)->getPatchInterface();
      ret_val += apatchInterfaceClass->GetNH3Emission();
	}
	return ret_val;
}
bool MMSClass::GetfinishedRun()
{
   if (*myTime == *stopTime)
      return true;
   else
      return false;
};


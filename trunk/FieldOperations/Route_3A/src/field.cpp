
 
// Class: field

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "field.h"   
#include "layerClass.h"
#include "locationclass.h"


field::field(const char * aName, const int aIndex, const base * aOwner):
	base(aName, aIndex, aOwner)
{
}
field:: ~ field ( )
{
	delete theOperations;
};

//!Perform daily routines
/*!
\param potEvap potential evapotranspiration (millimetres per day)
\param precip precipitation (millimetres per day)
\param Tmean mean daily air temperature (Celsius)
\param currentTsum current temperature sum (degree days)
*/
void field::dailyRoutine(double potEvap, double precip, double Tmean, double currentTsum)  
{
   if (currentOpNo<theOperations->NumOfNodes())
   {
      bool valid;
      fieldOp * afieldOp = doOp(currentTsum);
      if (afieldOp)
         valid = true;      //an operation can be performed today
      else
      	valid = false;     //no operation today
      while (valid)
      {
      	if (afieldOp->getname()=="Sow")
         {
         	string cropName = *((fieldOpSow *) afieldOp)->getplantName();
				if (theTime.GetMonth()>8)
   	         theEcosystem.sow(cropName, true);
            else
	            theEcosystem.sow(cropName, false);
         }
      	if (afieldOp->getname()=="Tillage")
         {
            double depth;
            depth = ((fieldOpTill *) afieldOp)->getdepth();
         	theEcosystem.tillage(depth);
         }
      	if (afieldOp->getname()=="Fertilise")
         {
            double amountN;
            amountN = ((fieldOpFert *) afieldOp)->getamountN();
            string type;
            type = *((fieldOpFert *) afieldOp)->gettype();
         	theEcosystem.fertilise(type, amountN);
         }
         if (afieldOp->getname()=="IrrigationStart")
         	irrigate= true;
         if (afieldOp->getname()=="IrrigationStop")
         {
         	irrigate= false;
            theEcosystem.irrigate(false);
         }
         if (irrigate)
         {
            double threshold;
            threshold = ((fieldOpIrr *) afieldOp)->getthresholdSMD();
            //get the soil moisture deficit for the appropriate depth
            double depth = theEcosystem.getSoilDepth();
            double SMD = theEcosystem.getSMD(depth);
            double fieldCapacity= theEcosystem.getFieldCapacity();
            double propSMD=SMD/fieldCapacity;
            if (propSMD >= threshold)
            	theEcosystem.irrigate(true);
            else
	            theEcosystem.irrigate(false);
         }
      	if (afieldOp->getname()=="Harvesting")
         	theEcosystem.harvest(0.5);       //assume that the residual leaf area index is 0.5
      	if (afieldOp->getname()=="Cutting")
         	theEcosystem.cut(0.5);             //assume that the residual leaf area index is 0.5
      	if (afieldOp->getname()=="GrazingStart")
         	graze=true;
      	if (afieldOp->getname()=="GrazingStop")
         	graze=false;
         currentOpNo++;
         afieldOp = doOp(currentTsum);
         if (afieldOp)
            valid = true;
         else
            valid = false;
      }
   }
	if (graze)     //harvest the crop every day
		theEcosystem.cut(0.5);  //assume that the residual leaf area index is 0.5
	if ((theTime.GetMonth()>2)&&(theTime.GetMonth()< 10))
	  theEcosystem.dailyRoutine(potEvap, precip,Tmean,false);
   else
	  theEcosystem.dailyRoutine(potEvap, precip,Tmean,true);
}
//!Initialise an instance of field
/*! 
\param afieldNo field identification number
\param theSoilType soil type to be used
\param thecropManagement pointer to a list of crop management practices
\param trafworkData pointer to the trafficability and workability parameters for this field
\param anoutputFile pointer to the output file
*/
void field::Initialise(int afieldNo, int theSoilType, linkList<cropManageClass> * thecropManagement, trafworkDataClass * trafworkData, fstream * anoutputFile)  
{
   theOperations = new linkList<fieldOp>;
   theTrafWorkData = new trafworkDataClass(*trafworkData);       //load the trafficability and workability data class
   outfile = anoutputFile;
   fieldNo = afieldNo;
   theEcosystem.Initialise(theSoilType);     //initialise the ecosystem model
   graze = false;
   irrigate = false;
   for (int i=0;i<thecropManagement->NumOfNodes();i++) //load all the field operation specifications
   	loadOps(thecropManagement->ElementAtNumber(i));
   sortOps();                              //sort the operations in order of starting date
   currentOpNo = 0;
}
//! Outputs field variables to file
/*! 
\param file pointer to output file
\param header set to true if the variable names should be printed
*/
void field::outputDetails(fstream * file, bool header)  
{
	theEcosystem.outputDetails(file, header);
   if (header)
	   *file << "surfSMD" << "\t";
	else
   	*file << theEcosystem.getSMD(50.0) << "\t";

}
//! Loads the field operation list
/*!
\param thePlan pointer to the list of field operations
*/
void field::loadOps(cropManageClass * thePlan)  
{
	//create the tillage operations
	for (int i=0;i<thePlan->getTillagePeriods()->NumOfNodes();i++)
	{ // tillage
		fieldOpTill* FO=new fieldOpTill();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getTillagePeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      double * theDepth = new double();
      *theDepth= *thePlan->getTillageDepth()->ElementAtNumber(i);
      FO->DefineOp("Tillage",1, aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),aPeriod->getTSum(),
                     true, *theDepth);
      theOperations->InsertLast(FO);
      delete theDepth;
   }
   //create the sowing operations
	for (int i=0;i<thePlan->getSowPeriods()->NumOfNodes();i++)
   {
		fieldOpSow* FO=new fieldOpSow();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getSowPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      string * plantName= new string(thePlan->getcropName());
      FO->DefineOp("Sow",2,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),aPeriod->getTSum(),
                     false, plantName);
      theOperations->InsertLast(FO);
   }
   //create the fertilisation and manuring operations
	for (int i=0;i<thePlan->getFertilizerPeriods()->NumOfNodes();i++)
   {
		fieldOpFert* FO=new fieldOpFert();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getFertilizerPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      string * atype;
      if (thePlan->getFertilizerType()->ElementAtNumber(i))
	      atype = thePlan->getFertilizerType()->ElementAtNumber(i);
      else
      	atype = new string("No type");
      double * amountN = thePlan->getFertilizerAmount()->ElementAtNumber(i);
      FO->DefineOp("Fertilise",3,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),aPeriod->getTSum(),
                     false,atype,*amountN);
      theOperations->InsertLast(FO);
   }
   //create the cutting operations
	for (int i=0;i<thePlan->getCuttingPeriods()->NumOfNodes();i++)
   {
		fieldOp* FO=new fieldOp();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getCuttingPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      FO->DefineOp("Cutting",4,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),aPeriod->getTSum(),
                     false);
      theOperations->InsertLast(FO);
   }
   // create the start of grazing operations
	for (int i=0;i<thePlan->getGrazingStartPeriods()->NumOfNodes();i++)
   {
		fieldOpGrass* FO=new fieldOpGrass();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getGrazingStartPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      double * theLAI = new double();
      *theLAI= *thePlan->getGrazingLAI()->ElementAtNumber(i);
      FO->DefineOp("GrazingStart",5,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),0.0,
                     false,*theLAI);
      theOperations->InsertLast(FO);
      delete theLAI;
   }
   //create the end of grazing operations
	for (int i=0;i<thePlan->getGrazingStopPeriods()->NumOfNodes();i++)
   {
		fieldOp* FO=new fieldOp();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getGrazingStopPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      FO->DefineOp("GrazingStop",6,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),0.0,
                     false);
      theOperations->InsertLast(FO);
   }
	for (int i=0;i<thePlan->getIrrigationStartPeriods()->NumOfNodes();i++)
   {
		fieldOpIrr* FO=new fieldOpIrr();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getIrrigationStartPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      double * thethresholdSMD= new double();
      *thethresholdSMD= *thePlan->getIrrigationThreshold()->ElementAtNumber(i);
      FO->DefineOp("IrrigationStart",7,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),0.0,
                     false,*thethresholdSMD);
      theOperations->InsertLast(FO);
      delete thethresholdSMD;
   }
   //create the end of grazing operations
	for (int i=0;i<thePlan->getIrrigationStopPeriods()->NumOfNodes();i++)
   {
		fieldOp* FO=new fieldOp();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getGrazingStopPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      FO->DefineOp("IrrigationStop",8,aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),0.0,
                     false);
      theOperations->InsertLast(FO);
   }
   //create the harvesting operations
	for (int i=0;i<thePlan->getHarvestPeriods()->NumOfNodes();i++)
   {
		fieldOp* FO=new fieldOp();
      FO->Initialise();
      fieldOrderPeriod * aPeriod = thePlan->getHarvestPeriods()->ElementAtNumber(i);
      string * sdate=aPeriod->getStartDate();
      string * edate=aPeriod->getEndDate();
      FO->DefineOp("Harvesting",9, aPeriod->GetYear(sdate),aPeriod->GetMonth(sdate),aPeriod->GetDay(sdate),
      					aPeriod->GetYear(edate),aPeriod->GetMonth(edate),aPeriod->GetDay(edate),aPeriod->getTSum(),
                     false);
      theOperations->InsertLast(FO);
   }
}
//! Checks whether one or more field operations can be performed. If so, the timing of the operation is logged to the output file.
fieldOp * field::doOp(double Tsum)
{
   if (currentOpNo<theOperations->NumOfNodes())
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(currentOpNo);
      int Optest = afieldOp->doOpNow(&theTime, Tsum);
      switch (Optest)
      {
      	// if Optest is 1, the criteria for the operation have been fulfilled
         case 1:
         case 2: double maxMoistureTraffic;
               double maxMoistureWork;
               double maxMoisture;
               //load the trafficability and workability limits appropriate to the crop type
               if (theEcosystem.getCropName()=="Grass")
               {
                     maxMoistureTraffic = theTrafWorkData->gettrafficability_grass();
                     maxMoistureWork = theTrafWorkData->getworkability_grass();
               }
               else
               {
                     maxMoistureTraffic = theTrafWorkData->gettrafficability_arable();
                     maxMoistureWork = theTrafWorkData->getworkability_arable();
               }
               double SMD;
               if (afieldOp->getname()=="Tillage")
               {
                  double depth;
                  depth = ((fieldOpTill *) afieldOp)->getdepth();
                  //get the soil moisture deficit for the appropriate depth
                  SMD = theEcosystem.getSMD(depth);
                  maxMoistureWork*=depth;
               }
               else
               {
               	double depth = 50.0;
                  SMD = theEcosystem.getSMD(depth);
                  maxMoistureTraffic *=depth;
               }
               if (afieldOp->getworkabilityConstraint())  //choose whether the workability or trafficability limits should be applied
                  maxMoisture = maxMoistureWork;
               else
                  maxMoisture = maxMoistureTraffic;
               //assume that if irrigation is required, soil moisture does not limit trafficability
               if (afieldOp->getname()=="Irrigation")
               	maxMoisture=0.0;

               if (SMD>=maxMoisture)
               {
			         *outfile << ((locationClass *) GetOwner())->GetNCU() << "\t" << fieldNo << "\t" ;
						afieldOp->outputOps(outfile,&theTime, Optest);
                  *outfile << "\t" << afieldOp->getTsum() << "\t" << Tsum << endl;
                  return afieldOp;
               }
               else
               {
                  return NULL;
               }
         //if Optest is 3, the operation has been forced by reaching the end date
         case 3:
               {
			         *outfile << ((locationClass *) GetOwner())->GetNCU() << "\t" << fieldNo << "\t" ;
                  if (afieldOp->getTsum() >= Tsum)
							afieldOp->outputOps(outfile,&theTime, 3); //operation could not be completed because Tsum not achieved
                  else
							afieldOp->outputOps(outfile,&theTime, 4); //operation could not be completed because of soil moisture limitation
                  *outfile << "\t" << afieldOp->getTsum() << "\t" << Tsum << endl;
                  return afieldOp;
               }
         default: return NULL;
         }
      }
   else return NULL;
}
//! Sorts the field operations in order of starting date
void field::sortOps()
{
	temp_double_array  doubleTime;
	temp_ptr_array ptrlist;
	int Hi=0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
		doubleTime[Hi]=(double) afieldOp->getopStart()->GetDateValue();
   	ptrlist[i]=(void*) theOperations->ElementAtNumber(i);
		Hi++;
   }

   Hi--;
   // Below is the "Combsort" sorting algorithm, which is nearly as fast as
   // Quicksort, and does not require extra space on heap and stack, i.e. more
   // suited for this purpose than the implemented Quicksort (bmp).
	Comsort(doubleTime, ptrlist, Hi);
   theOperations->ForgetAll();
   for (int counter=0;counter<=Hi;counter++)
   {
		theOperations->InsertLast((fieldOp *) ptrlist[counter]);
   }
}
//! Tidy up
void field::closeDown()  
{
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
		afieldOp->closeDown();
   }
	theOperations->Reset();
   theEcosystem.closeDown();
   delete theTrafWorkData;
}


 
// Class: cropManageClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "cropManageClass.h"   
#include "soilDataClass.h" 				

cropManageClass:: ~ cropManageClass ( )
{

};

//! Read the list of field operations for a single field
void cropManageClass::ReadOperations()  
{
 int periodcount;
 int first;
 SetCritical();
 string * ch = new string();
 GetParameter("CROP",ch);
 cropName = *ch;
 delete ch;
 UnsetCritical();
 //most crops will require only a subset of the field operations to be defined
 GetVectorInfo("TillageStartDate",&first,&periodcount);
 file->seekg(0);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Tillage",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    TillagePeriods->InsertLast(aPeriod);
    double * a = new double(0.0);
    GetParameter("TillageDepth",a,i);
    TillageDepth->InsertLast(a);
 }

 GetVectorInfo("SowStartDate",&first,&periodcount);
 for (int i=first;i<(first+periodcount);i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Sow",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    SowPeriods->InsertLast(aPeriod);
 }
 GetVectorInfo("FertilizerStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)  //number of fertiliser or manure applications
 {
    fieldOrderPeriod *aPeriod = new fieldOrderPeriod("Fertilizer",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    FertilizerPeriods->InsertLast(aPeriod);
    double * a = new double(0.0);
    GetParameter("FertilizerN",a,i);
    FertilizerAmount->InsertLast(a);
    string * astring = new string();
    if (GetParameter("FertilizerType",astring,i))
	    FertilizerType->InsertLast(astring);
 }  //end of fertiliser

 GetVectorInfo("HarvestStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Harvest",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    HarvestPeriods->InsertLast(aPeriod);
 }
 GetVectorInfo("CuttingStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Cutting",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    CuttingPeriods->InsertLast(aPeriod);
 }
 GetVectorInfo("GrazingStartStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("GrazingStart",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    GrazingStartPeriods->InsertLast(aPeriod);
    double * a = new double(0.0);
    GetParameter("GrazingLAI",a,i);
    GrazingLAI->InsertLast(a);
 }
 GetVectorInfo("GrazingStopStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("GrazingStop",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    GrazingStopPeriods->InsertLast(aPeriod);
 }
 GetVectorInfo("IrrigationStartStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("IrrigationStart",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    IrrigationStartPeriods->InsertLast(aPeriod);
    double * a = new double(0.0);
    GetParameter("IrrigationThreshold",a,i);
    IrrigationThreshold->InsertLast(a);
 }
 GetVectorInfo("IrrigationStopStartDate",&first,&periodcount);
 for (int i=0;i<periodcount;i++)
 {
    fieldOrderPeriod * aPeriod = new fieldOrderPeriod("IrrigationStop",i,this);
    aPeriod->Setcur_pos(Getcur_pos());
    aPeriod->ReadPeriod(file);
    IrrigationStopPeriods->InsertLast(aPeriod);
 }
}
//!Creates the arrays used to store the details of the field operations and then reads and stores the data.
/*! 
\param afile pointer to a file containing data
\param anIndex number of the field to which the operations should be applied
*/
void cropManageClass::Initialise(fstream * afile, int anIndex)  
{
	Setfile(afile);
   Index = anIndex;
   TillagePeriods = new linkList<fieldOrderPeriod>;
   SowPeriods = new linkList<fieldOrderPeriod>;
   FertilizerPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   HarvestPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   CuttingPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   GrazingStartPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   GrazingStopPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   GrazingLAI = new linkList<double>;
   IrrigationStartPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   IrrigationStopPeriods = new linkList<fieldOrderPeriod>; //Attribute data member
   IrrigationThreshold = new linkList<double>;
   TillageDepth = new linkList<double>; //Attribute data member
   FertilizerAmount = new linkList<double>;
   FertilizerType = new linkList<string>;
   SetCritical();
   FindSection("Crop",Index);
   UnsetCritical();
   ReadOperations();
	Setfile(0);
}
//! Tidy up lists of operations
void cropManageClass::CloseDown()  
{
   delete TillagePeriods;
   delete SowPeriods; //Attribute data member
   delete FertilizerPeriods; //Attribute data member
   delete HarvestPeriods; //Attribute data member
   delete CuttingPeriods; //Attribute data member
   delete GrazingStartPeriods; //Attribute data member
   delete GrazingStopPeriods; //Attribute data member
   delete GrazingLAI;
   delete TillageDepth; //Attribute data member
   delete FertilizerAmount;
   delete FertilizerType;
   delete IrrigationStartPeriods;
   delete IrrigationStopPeriods;
   delete IrrigationThreshold;
}

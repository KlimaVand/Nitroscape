#include "../base/common.h"
#include "../base/IndicatorOutput.h"
#include "../building/PigHousing.h"
#include "../livestock/swineStock.h"
#include "../base/controlParameters.h"

PigHousing::PigHousing(char* aname,const int aIndex,const base* aOwner)
   	:stable(aname,aIndex,aOwner)
{
   Livestock = new animalStock(aname,aIndex,aOwner);
}

PigHousing::~PigHousing()
{
}
PigHousing::PigHousing(const PigHousing& aStable):stable(aStable)
{
	sections=new linkList<animalSection>();
	for (int i=0;i<aStable.sections->NumOfNodes();i++)
	   {
		animalSection *p = new pigstysection(*(pigstysection *)(aStable.sections->ElementAtNumber(i)));
		sections->InsertLast(p);
		p->GetContains();
	   }
	//standartSections=new linkList<animalSection>();
	//for (int i=0;i<aStable.standartSections->NumOfNodes();i++)
	   {
		//animalSection *p = new pigstysection(*(aStable.standartSections->ElementAtNumber(i)));
		//standartSections->InsertLast(p);
	   }
}
/**
 * Update the stable
 */
void PigHousing::DailyUpdate()
{
   stable::DailyUpdate();
   //double hours = GetLabour();
   #ifdef STALDDEBUG
// 	cout << " pigstable::DailyUpdate: " << Nbudget << endl;
	#endif

   //electricity is commented out until I understand how it is calculated
/*	theOutput->AddIndicator(economicIndicator,"07.02 Labour, stable","hours",hours);
	theOutput->AddIndicator(economicIndicator,"07.03 Labour, maintance and management","hours",0.20*hours);  // 5% for building maintance + 15% for management
	theOutput->AddIndicator(economicIndicator,"07.04 Total labour","hours",hours*1.20);
                                                                                       */
}

void PigHousing::ReadParameters(fstream * file)
{
   Setfile(file);
   Setcur_pos(0);
   FindSection("PigHousing",Index);  // find the correct place in the input file
   stable::ReadParameters(file);
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
//   FindSection("Pigstable",Index);  // find the correct place in the input file


   // get info on sections
   Setcur_pos(0);
	namestr=(string) "PigHousing"+Indexstr+"."+"Section";
   int first,num;
   GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
 //  double cumulativePartition=0.0
	for(int inx=first;inx<(first+num);inx++)
   {

    pigstysection *aSection=new pigstysection((char *)namestr.c_str(),inx,this);

    //aSection->SetScalable(true);
    // aSection->ReadParameters(file,false);
    // for(int i=0;i<standartSections->NumOfNodes();i++)
       {
    	//   	if(standartSections->ElementAtNumber(i)->GetContains().compare(aSection->GetContains())==0)
       	{

       		//pigstysection *Section=new pigstysection(*standartSections->ElementAtNumber(i));

       	//	Section->ReadParameters(file,true);

       		//sections->InsertLast(Section);
       	}
       }

    bool gotit=false;
    int i=0;
    while ((gotit==false)&&(i<sections->NumOfNodes()))
    {
    	animalSection *testSection= sections->ElementAtNumber(i);

    	if (testSection->GetContains()==aSection->GetContains())
    	{
    		//if (testSection->GetManureType()==aSection->GetManureType())
    		{
    			gotit=true;
    			/*
    			 * this code should read all parameters in the actual housing and overwrite the defaults. Does not yet because no == operator available
    			 *
    			 char newName[50];
    			strcpy(newName, aSection->GetName());
    			testSection->SetName(newName);
    			testSection->ReadParameters(file);
    			testSection->CheckFloorIntegrity();*/
    		}
    	}
		i++;
    }
   }
   // see if the flooring sends manure to an inhouse manure store
 for(int i=0;i<inhouseStores->NumOfNodes();i++)
 {
   manurestore * aStore=inhouseStores->ElementAtNumber(i);
 	for(int i=0;i<sections->NumOfNodes();i++)
   {
  	animalSection * aSection=sections->ElementAtNumber(i);
   aSection->LinkManureStores(aStore);
	}
 }
 if (theControlParameters->GetvolatModel()>0)
 		checkArea();
 Setfile(NULL);
}



/**************************************
Minimum ventilation in cubic metres per sec
**************************************/
double PigHousing::CalcMinimumVentilationRate()
{
	return GetNrOfAnimals() * 6.0/3600.0;
};

/**************************************
Maximum ventilation in cubic metres per sec
**************************************/
double PigHousing::CalcMaximumVentilationRate()
{
	return GetNrOfAnimals() * 125.0/3600.0;
};

void PigHousing::ReadDefaultParameters()
{
	SetCritical();
	if (OpenInputFile("StandardBuildings.dat"))
	{
		FindSection("PigHousing(0)");
		//stable::ReadDefaultParameters(file);
		GetParameter("meanHeight",&meanHeight);
		GetParameter("planArea",&planArea);
		GetParameter("thermalTransWall",&thermalTransWall);
		GetParameter("thermalTransRoof",&thermalTransRoof);
		GetParameter("minVentilation",&minVentilation);
		GetParameter("maxVentilation",&maxVentilation);
		if ((minVentilation>maxVentilation)||(maxVentilation<=0.0))
			theMessage->FatalError("Stable:: minimum ventilation rate > maximum or maximum <= 0");
		GetParameter("targetTemperature",&targetTemperature);
		GetParameter("absorbCoeff",&absorbCoeff);
		GetParameter("emissivity",&emissivity);
		GetParameter("externSurfResis",&externSurfResis);
		UnsetCritical();
		// get info on sections
		Setcur_pos(0);
		string namestr;
		namestr=(string) "PigHousing(0)"+".Section";
		int first,num;
		GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of sections associated with this instance
		//  double cumulativePartition=0.0;
		for(int inx=first;inx<(first+num);inx++)
		{
			pigstysection *aSection=new pigstysection((char *)namestr.c_str(),inx,this);
			aSection->SetScalable(true);
			//aSection->ReadDefaultParameters(file);
//			standartSections->InsertLast(aSection);
			sections->InsertLast(aSection);
		}
		// Construct in-house manure stores
		Setcur_pos(0);
		namestr=(string) "PigHousing(0)"+".inhouse";
		GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of inhouse stores associated with this instance
		FindSection((char*) namestr.c_str(),0);  // find the correct place in the input file
		//   string instanceName;
		for(int inx=first;inx<(first+num);inx++)
		{
			instore *manureInst=new instore((char *)namestr.c_str(),inx,this);
			manureInst->SetScalable(true);
			//manureInst->ReadDefaultParameters(file);
			inhouseStores->InsertLast(manureInst);
		}
		calcWallArea();
		CloseInputFile();
	}

}

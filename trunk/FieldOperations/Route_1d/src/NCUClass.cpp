

// Class: NCUClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "NCUClass.h"
#include "climateNCU_MARS_ATEAM.05.Extract_Data.Func.V2.h"
#include "message.h"
#include "operationData.h"
#include "fieldOp.h"
#include "fieldOpTill.h"
#include "fieldOpSow.h"
#include "fieldOpFert.h"
#include "fieldOpHarv.h"
#include "fieldOpGrass.h"
#include "fieldOpCut.h"
#include "nixcbits.h"
#ifdef __ANSICPP__
	#include <cmath>
	#include <istream>
	#include <iostream>
	#include <cstring>
#else
	#include <fstream.h>
	#include <iostream.h>

#endif
#include <vector>
extern int getNCU_ID(int,char[256]);
extern int getNCUfromHSMU(int,char[256]);
extern bool verbosity;
extern int routeNumber;

NCUClass::NCUClass ( )
{
 NCU=0;
 NCUI=0;
};

NCUClass:: ~ NCUClass ( )
{
	delete theYearPlans;
	delete theSeriesOperations;
};

void NCUClass::Initialise(char *inputDir)
{
//load list of replacement crops to be used for those crops not present in CGMS data
	loadReplacements(inputDir);

   theYearPlans= new linkList<cropRotationClass>;
   theSeriesOperations = new linkList<cropSeriesClass>;
}


int NCUClass::ReadData(int *aNCU, int startDay,  int startYear,  int endDay,  int endYear,  int baseYear, int range,
		char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir, fstream * rotationFile,
      bool bootstrap)
{

	int ret_val=0;
	string aString;
   for (int year=startYear;year<(endYear+2); year++)
   {
      cropRotationClass * acropRotationClass = new cropRotationClass();
      acropRotationClass->setYear(year);
      theYearPlans->InsertLast(acropRotationClass);
   }

#ifdef __BCplusplus__
	char dummy[5000];
#endif
	char str10[10];
    if (!bootstrap)   //if choice of NCUs is an input
    {
      NCU = *aNCU;
      NCUI = getNCU_ID(NCU,ncu_mars);
    }
    string testString;

#ifdef __BCplusplus__
//    rotationFile->getline(dummy,5000,'\n');
//    testString=dummy;
#else
//	getline(*rotationFile, testString);
#endif
 		*rotationFile >> NCUI;
  		if (NCUI==0)
       	 return 2;

    if (bootstrap)//need to get NCU and NCUI from input file
    {
      NCU = getNCUfromHSMU(NCUI,ncu_mars);
      *aNCU = NCU;   //to inform calling class of NCU
      if (NCU==3491)
    	  cout << "here";
    }

   if (verbosity)
	   cout << " NCUI " << NCUI << endl;

   int maxCrops = 6; //8 = max number of crop sequences
   int cropnum=0; //crop number

   //loop through crop series
   int startCol = startYear - baseYear + 2;
   while (cropnum<maxCrops)
   {
	   ReadCropSequence(cropnum, startCol, rotationFile);
      cropnum++;
   }

   cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(0);
   if (acropRotationClass->gettheCrops()->NumOfNodes()==0)
   	ret_val = 2;
   else
   {
   	if (routeNumber>1)	//met data not needed for Route 1
      {
      	//get met data
         for (int i=0; i<50;i++)
         for (int j=0;j<366;j++)
         for (int k=0; k<6; k++)
            metdata[i][j][k]=0.0;
         ret_val = getMetData(NCU, startDay, startYear, endDay, endYear, metdataInputDir,
               tpvStem, radStem, ncu_mars, baseYear, range, metdata);
      }
      else
      	ret_val = 1;
   }
   return ret_val;

/* code to be used when searching for a particular NCU
   int testNCUID=0;

   //find first line with the required NCUID
   while (testNCUID<NCUI)
   {
#ifdef __BCplusplus__
		if (testNCUID>0)
			rotationFile->getline(dummy,5000,'\n');
	   istrstream inputString(dummy,strlen(dummy));
#else
	   stringstream inputString(stringstream::in | stringstream::out);
		if (testNCUID>0)
			getline(*rotationFile, testString);
	   inputString << testString;
	   //cout << inputString.str() << endl;
#endif
      inputString >> str10;
      testNCUID = atoi(str10);
      if (testNCUID==0)
	      theMessage->FatalError("NCUClass: crop rotation for this NCU not found in in crop rotation file");
   }
*/
}

void NCUClass::ReadCropSequence(int cropnum, int startCol, fstream * rotationFile)
{

#ifdef __BCplusplus__
	char dummy[5000];
#else
	string testString;
#endif
	char str10[10];

	  bool abort=false;
      int row=0;
      int maxRows = 15; //number of variables per crop
//      while ((row<maxRows)&&(!abort))
      while (row<=maxRows)
      {
//      	if ((cropnum!=0)||(row!=0)|| (abort)) //if cropnum and row = 0 then this is the first crop, so data is already read into input string
      	if (!abort)
      	{
#ifdef __BCplusplus__
	         rotationFile->getline(dummy,5000,'\n');
//			istrstream inputString(dummy,strlen(dummy));
#else
      		testString.clear();
//      		stringstream inputString(stringstream::in | stringstream::out);
      		getline(*rotationFile, testString);
#ifdef __DEBUGGING__
      		cout << testString << endl;
#endif
// 	   		inputString << testString;
#endif
//				if (!abort)
  //	   			inputString >> str10;  //get NCUI
      	}
		if (verbosity)
         	cout << "Crop sequence " << cropnum << " Row number " << row << endl;
#ifdef __BCplusplus__
		istrstream inputString(dummy,strlen(dummy));
#else
  		stringstream inputString(stringstream::in | stringstream::out);
#ifdef __DEBUGGING__
      	cout << testString << endl;
#endif
   		inputString << testString;
#endif
        //find first year's data
		int leadIn;
		if ((cropnum==0)&&(row==0))
      	leadIn =startCol-2;
		else
			leadIn=startCol-1;
		for (int i=0; i<leadIn; i++)
			inputString >> str10;
		int yearNum=0;
		while (yearNum<theYearPlans->NumOfNodes())
        {
            cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNum);
            NCUCropClass *aNCUCropClass;
            if (row==0)
            {
               aNCUCropClass = new NCUCropClass();
               aNCUCropClass->Initialise();
		      	abort = aNCUCropClass->ReadData(&inputString, row);
               if ((yearNum==0)&&(aNCUCropClass->getWinterCrop()))  //if this is the first year then there is no winter crop available
               	abort=true;
               if (abort)  //no cropping
               {
            	  delete aNCUCropClass;
                  aNCUCropClass = NULL;
               }
               acropRotationClass->gettheCrops()->InsertLast(aNCUCropClass);
            }
            else
            {
            	if (!abort)
               {
                  aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(cropnum);
                  if (aNCUCropClass)
	                  aNCUCropClass->ReadData(&inputString, row);
               }
            }
            yearNum++;
         }
         row++;
		 if (verbosity)
      	   cout << endl;
      }
}

void NCUClass::calcCropManagement(char *inputDir, char *outputDir)
{
	bool abort = false;
   int yearNo=0;
   char inputFileName[256];

	//get timing of sowing of spring barley (used for timing of fertilisation of winter crops)
   strcpy(inputFileName,"SpBarl.csv");
   springBarleySowing=getCropEventDate(inputDir, inputFileName, MARS, 1, true);

	//get crop rotation for required number of years
	while (yearNo<(theYearPlans->NumOfNodes()))
   {
   	cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNo);
      for (int cropNo = 0; cropNo<acropRotationClass->gettheCrops()->NumOfNodes(); cropNo++)
      {
         NCUCropClass * aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(cropNo);
         if (aNCUCropClass)
         {
            string cropName = aNCUCropClass->getCropName();

            //see if this crop needs to be replaced by one for which crop timing is available
            string * localName = new string(cropName);
            getReplacements(localName);

            int sowingDate=0;
            int harvestingDate=0;
            int defaultSowingDate;  //Europe-wide average date, for situations where there is no sowing date within a NUTS region
            int diff = 0;//default difference between sowing and harvesting date, for when harvesting date is missing
            bool gotit = false;
            if ((localName->compare("SWHE")==0) || (localName->compare("DWHE")==0))
            {
               strcpy(inputFileName,"SpWheat.csv");
               diff = 126;
               defaultSowingDate = 99;
               gotit = true;
            }
            if (localName->compare("POTA")==0)
            {
               strcpy(inputFileName,"Potato.csv");
               diff = 137;
               defaultSowingDate=115;
               gotit = true;
            }
            if (localName->compare("BARL")==0)
            {
               strcpy(inputFileName,"SpBarl.csv");
               diff = 128;
               defaultSowingDate=92;
               gotit = true;
            }
            if (localName->compare("MAIF")==0)
            {
               strcpy(inputFileName,"FodMaize.csv");
               diff = 112;
               defaultSowingDate=124;
               gotit = true;
            }
            if (localName->compare("MAIZ")==0)
            {
               strcpy(inputFileName,"GraMaize.csv");
               diff = 165;
               defaultSowingDate=117;
               gotit = true;
            }
            if (localName->compare("RAPE")==0)
            {
               strcpy(inputFileName,"WinRape.csv");
               diff = 306;
               defaultSowingDate=243;
            	aNCUCropClass->setWinterCrop(true);
               gotit = true;
            }
            if (localName->compare("SUGB")==0)
            {
               strcpy(inputFileName,"SugBeet.csv");
               diff = 169;
               defaultSowingDate=108;
               gotit = true;
            }
          if (localName->compare("SUNF")==0)
            {
               strcpy(inputFileName,"SunFlower.csv");
               diff = 150;
               defaultSowingDate=106;
               gotit = true;
            }
            if (localName->compare("WWHE")==0)
            {
               strcpy(inputFileName,"WinWheat.csv");
               diff = 283;
               defaultSowingDate=279;
            	aNCUCropClass->setWinterCrop(true);
               gotit = true;
            }

            if ((localName->compare("SETA")==0)||(localName->compare("GRAS")==0))
            {
               strcpy(inputFileName,"SpBarl.csv");
               diff = 128;
               defaultSowingDate=92;
               gotit = true;
            }

      //need values for winter barley, spring rape
            if (!gotit)
               theMessage->FatalError("File not found for ",cropName.c_str());
            sowingDate=getCropEventDate(inputDir, inputFileName, MARS, 1, true);
            if (sowingDate == 999)
               sowingDate=defaultSowingDate;
            aNCUCropClass->setSowingDay(sowingDate,0);
            if (localName->compare("SETA")!=0)   //skip harvesting for set-aside
            {
               harvestingDate=getCropEventDate(inputDir, inputFileName, MARS,2, true);
               if (harvestingDate==999)
               {
                  harvestingDate=sowingDate+diff;
                  if (harvestingDate>365)
                     harvestingDate-=365;
               }
               if (localName->compare("GRAS")==0)   //default dates for start and stop of grazing
               {
                  aNCUCropClass->setGrazingStartDay(sowingDate,0);
                  aNCUCropClass->setGrazingStopDay(harvestingDate,0);
                  aNCUCropClass->setCuttingDay(harvestingDate,0);
               }
					else
						aNCUCropClass->setHarvestingDay(harvestingDate,0);

            }
            delete localName;
      	}  //end if aNCUCropClass not NULL
   	}
	yearNo++;
   }

   yearNo=0;
	while (yearNo<theYearPlans->NumOfNodes())
   {
   	cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNo);
      for (int cropNo = 0; cropNo<acropRotationClass->gettheCrops()->NumOfNodes(); cropNo++)
      {
         NCUCropClass * aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(cropNo);
         if (aNCUCropClass)
         {
			   if (verbosity)
         		cout << " Year " << yearNo << " Crop " << aNCUCropClass->getCropName() << endl;
            //tillage for all crops occurs just prior to sowing
            aNCUCropClass->setPloughingDay(aNCUCropClass->getSowingDay(0)-3,0);
            aNCUCropClass->setHarrowingDay(aNCUCropClass->getSowingDay(0)-2,0);
            //all solid manure is applied just prior to ploughing
            if (aNCUCropClass->getCattleSolid()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,0);
            if (aNCUCropClass->getPigSolid()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,2);
            if (aNCUCropClass->getPoultrySolid()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,4);
            if (aNCUCropClass->getGrazing()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,6);

            if (!aNCUCropClass->getWinterCrop()) //is a spring crop
            {
               //all fertilisation and liquid manuring of spring crops occurs just prior to sowing
               if (aNCUCropClass->getCattleSlurry()>0.0)
                  aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,1);
               if (aNCUCropClass->getPigSlurry()>0.0)
                  aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,3);
               if (aNCUCropClass->getPoultrySlurry()>0.0)
                  aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,5);

               if (aNCUCropClass->getUreaFertiliser()>0.0)
                  aNCUCropClass->setFertDay(aNCUCropClass->getSowingDay(0)-5,0);
               if (aNCUCropClass->getNitrateFertiliser()>0.0)
                  aNCUCropClass->setFertDay(aNCUCropClass->getSowingDay(0)-5,1);
            }
            else
            {
               // fertilisation and liquid manuring of winter crops occurs in the spring (assumed to be at same time as spring barley would be sown)
               if (aNCUCropClass->getCattleSlurry()>0.0)
                  aNCUCropClass->setManureDay(springBarleySowing,1);
               if (aNCUCropClass->getPigSlurry()>0.0)
                  aNCUCropClass->setManureDay(springBarleySowing,3);
               if (aNCUCropClass->getPoultrySlurry()>0.0)
                  aNCUCropClass->setManureDay(springBarleySowing,5);

               if (aNCUCropClass->getUreaFertiliser()>0.0)
                  aNCUCropClass->setFertDay(springBarleySowing,0);
               if (aNCUCropClass->getNitrateFertiliser()>0.0)
                  aNCUCropClass->setFertDay(springBarleySowing,1);
            }
         }
      }
      yearNo++;
   }

};
struct product {
	double DataID1;
	double DataID2;
	int marsCode;
} ;
struct File {
	char fileName[50];
	vector<product> fileContent;
} ;

static vector<File> fileList;
//! returns the date of sowing (1) or havesting (2) for a particular MARS grid cell
int NCUClass::getCropEventDate(char inputDir[256], char MARSDateFileName[50], int MARScode, int dateID, bool recursive)
{
	char inputFileName[150];
	strcpy(inputFileName,inputDir);
	strcat(inputFileName,MARSDateFileName);
//!Check to make sure that the event requested is either Sowing or harvesting

   if ((dateID<1)||(dateID>2))
      theMessage->FatalError("NCUClass::getCropEventDate - dateID is not 1 or 2");
   bool found=false;
   int placeInVector=-1;

   //find the vector that corresponds to this file name (=crop name)
   for(int i=0;i<fileList.size();i++)	//step through file list
   {
	   for(int j=0;j<50;j++)	//step through characters in file name
	   {
		   int mars=MARSDateFileName[j];
		   int others=fileList.at(i).fileName[j];
		   if(mars==0||others==0)	//break if no data present
			   break;
		   if(fileList.at(i).fileName[j]==MARSDateFileName[j])	//character by character check
		   {
			   found=true;
			   placeInVector=i;
		   }
		   else
		   {
			   found=false;
			   break;
		   }
	   }
	   if(found==true)	//stop when correct element is found
		   break;
   }
   int DayNo=999;

   if(found==false)	//this crop has not been read into a vector yet, so get it
   {
	   File file;
	   for(int i=0;i<50;i++)
	   {
		   file.fileName[i]=MARSDateFileName[i];	//assign fileName to the crop file name
	   }
	   //find data in file and store it in vector
		FILE *afile;
		afile=fopen(inputFileName, "r");
		int idummy1;
		float idummy2, idummy3;
		if (afile==NULL)
			theMessage->FatalError("NCUClass::getCropEventDate file ", inputFileName, " not found");
		char buffer[20];
		fscanf(afile, "%19c ", buffer);
		char semicolon[1];
		//!
		while ( fscanf(afile, "%d %c %f %c %f", &idummy1,semicolon, &idummy2, semicolon, &idummy3) == 5 && DayNo==999 )
		{
			//! Find the correct row in the file
			product item;

			item.marsCode=idummy1;
			item.DataID1=idummy2;
			item.DataID2=idummy3;

		  //! Round up if necessary
		  double temp2=item.DataID1-floor(item.DataID1);
		  if (temp2>=0.5)
			  item.DataID1+=1.0;
		  temp2=item.DataID2-floor(item.DataID2);
		  if (temp2>=0.5)
			  item.DataID2+=1.0;
		  file.fileContent.push_back(item);
		}
		fclose(afile);
		fileList.push_back(file);
   }
   if(found==false)	//data were not in vector array but should now be, so go look for it again
   {
	   for(int i=0;i<fileList.size();i++)
	      {
	   	   for(int j=0;j<50;j++)
	   	   {
	   		   if(fileList.at(i).fileName[j]==MARSDateFileName[j])
	   		   {
	   			   found=true;
	   			   placeInVector=i;
	   		   }
	   		   else
	   		   {
	   			   break;
	   		   }
	   	   }
	      }
   }
   if(found==true)	//the data are now present
   {
	   File item= fileList.at(placeInVector);
	   for(int i=0;i<item.fileContent.size();i++)	//go through vector array of crops
	   {
		   if(item.fileContent.at(i).marsCode==MARScode)	//data corresponding to correct MARS code
		   {

			   if (dateID==1)//! get sowing date
				   DayNo=(int)item.fileContent.at(i).DataID1;
			   if (dateID==2)//! get harvesting date
				   DayNo=(int)item.fileContent.at(i).DataID2;
			   break;
		   }
	   }
   }
   else	//big problems
   {
	   cout <<  "Data for " << MARSDateFileName << " MARS code " << MARScode <<  " dataID " << dateID << "not found" << endl;
	   theMessage->FatalError("NCUClass::getCropEventDate: data not found");
   }

    //! No date was found, so we have to use the average for the NUTS region

    if ((DayNo==999)&& (recursive))
    {
	   DayNo=getAverageCropEventDate(inputDir, MARSDateFileName, dateID);
    }
    return DayNo;
}

struct product1 {
	int nutsCode;
	int marsCode;
} ;

static vector<product1> productList;

//!Returns the average date within a NUTS region on which an operation is performed
int NCUClass::getAverageCropEventDate(char inputDir[256], char MARSDateFileName[50], int dateID)
{
	int retVal=999;
//!create linklist of ops data
   linkList<operationData> *theOpsData;
   theOpsData = new linkList<operationData>;
	char inputFileName[150];
	strcpy(inputFileName,inputDir);
	strcat(inputFileName,"NCU_MARS.txt");
   //! NUTS = first three digits of NCU ID
   int NUTS = NCUI/1000000;


/****************************************************************/

   	   bool found = false;
	   if(productList.size()==0)	//data have not been read into a vector yet, so get it
	   {
		   //productList aproductList;
		   //find data in file and store it in vector
			FILE *afile;
			afile=fopen(inputFileName, "r");
			//!

		    int idummy1, idummy2;
		    //!read thru NCU MARS data correspondence file and find each place where this NUTS region is represented by a MARS grid point
		    if (afile==NULL)
		    	theMessage->FatalError("NCUClass::getAverageCropEventDate: file ", inputFileName, " not found");
			//idummy1 contains NCUI code, idummy2 contains MARS code
		    while ( fscanf(afile, "%d %d", &idummy1,&idummy2) == 2)
		    {
		    	int tempNUTS = idummy1/1000000;//! truncate idummy1 (NCUI) to obtain the NUTS region
				product1 item;
				item.nutsCode=tempNUTS;
				item.marsCode=idummy2;
				productList.push_back(item);
		    }

			fclose(afile);
	   }

       operationData * aoperationData;
	   for(int i=0;i<productList.size();i++)	//step through product list
	   {
		   int productNUTS = productList.at(i).nutsCode;
		   if (productNUTS==NUTS)	//have correct row
		   {
			   bool gotit = true;
			   int productMARS = productList.at(i).marsCode;
			   for (int i=0;i<theOpsData->NumOfNodes(); i++)
		       {
		       	aoperationData=theOpsData->ElementAtNumber(i);
		          if (aoperationData->GetMARSno()==productMARS)  //!one MARS grid point may cover more than one NUTS region. We only want one
		          	gotit=false;
		       }
		      if (gotit)
		       {
		      	//! get the crop event date relating to the target MARS code (i.e. is within the target NUTS region), immobilise recursive calls.
		      	int adate=getCropEventDate(inputDir, MARSDateFileName, productMARS, dateID, false);
		        if (adate!=999)//! 	we found a valid date. Load the MARS and operation date
		         {
		            aoperationData = new operationData();
		            aoperationData->SetMARSno(productMARS);//! productMARS = MARS code
		            aoperationData->SetdayNo(adate);
		            theOpsData->InsertLast(aoperationData);
		            //! print out the details to screen, if that is what we want
		//			  if (verbosity)
		//          	  cout<<idummy1<<","<<idummy2<<endl;
		         }
		       }
		   }
		   if (productNUTS>NUTS)	//have reached end of data for this NUTS
			   break;
	   }
	   int MARSNo=999;
	   //! if no data was found at the NUTS region, return 999
	   if (theOpsData->NumOfNodes()==0)
	   {
	   	delete theOpsData;
	   	return retVal;
	   }
	   else  //calculate average date
	   {
	      double aveDate=0.0;
	      for (int i=0;i<theOpsData->NumOfNodes(); i++)
	      {
	         operationData * aoperationData;
	         aoperationData=theOpsData->ElementAtNumber(i);
	         aveDate+=aoperationData->GetdayNo();
	      }
	      aveDate/=theOpsData->NumOfNodes();
	      theOpsData->Reset();
	      delete theOpsData;
	      //! round up or down, as appropriate
	      if ((aveDate-floor(aveDate))>0.5)
	         retVal=ceil(aveDate);
	      else
	         retVal=floor(aveDate);
	      return retVal;
	   }
}



void NCUClass::OutputData(fstream *afile)
{
	for (int yearNum=0;yearNum<theYearPlans->NumOfNodes();yearNum++)
   {
   }
}

void NCUClass::generateOps()
{
	int slurryApplicMethod = 1;
   int solidApplicMethod = 1;
   int ureaApplicMethod = 1;
	for (int seriesNo=0; seriesNo<numberSeries; seriesNo++)
   {
   	cropSeriesClass * acropSeriesClass = new cropSeriesClass(seriesNo);

      for (int yearNum=0;yearNum<theYearPlans->NumOfNodes();yearNum++)
      {
         cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNum);
         int calendarYear= acropRotationClass->getYear();
         cropRotationClass * nextcropRotationClass;
         if (yearNum<(theYearPlans->NumOfNodes()-1))
            nextcropRotationClass = theYearPlans->ElementAtNumber(yearNum+1);
         else
            nextcropRotationClass=NULL;
//         for (int cropNo = 0; cropNo<acropRotationClass->gettheCrops()->NumOfNodes(); cropNo++)
         NCUCropClass * aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(seriesNo);
         NCUCropClass * nextCropClass;
         if (yearNum<(theYearPlans->NumOfNodes()-1))
            nextCropClass = nextcropRotationClass->gettheCrops()->ElementAtNumber(seriesNo);
         else
            nextCropClass=NULL;
         if (aNCUCropClass)
         {
            if (!aNCUCropClass->getWinterCrop())
            {
            //start with ploughing
            // get first ploughing date
               if (aNCUCropClass->getPloughingDay(0)>0)
               {
                  fieldOpTill *afieldOpTill = new fieldOpTill();
                  afieldOpTill->DefineOp("Ploughing", 1, calendarYear, aNCUCropClass->getPloughingDay(0), 0.25);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpTill);
               }
               if (aNCUCropClass->getSowingDay(0)>0)
               {
                  fieldOpSow *afieldOpSow = new fieldOpSow();
                  afieldOpSow->DefineOp("Sowing", 2, calendarYear, aNCUCropClass->getSowingDay(0), aNCUCropClass->getCropName());
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpSow);
               }
               if (aNCUCropClass->getManureDay(0)>0)  //cattle solid
               {
            	  double amountN = aNCUCropClass->getCattleSolid();
            	  double theNH4N =0.25 * amountN;
                  double theNO3N = 0.0;
	              double theOrgN = amountN - (theNH4N + theNO3N);
	              double theamountC = aNCUCropClass->getCattleSolidC();
	              if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("cattle solid manure without C in ", double(NCU));
                  fieldOpFert *afieldOpFert = new fieldOpFert();
                  afieldOpFert->DefineOp("CattleSolid", 3, calendarYear, aNCUCropClass->getManureDay(0), 3, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
					}
               if (aNCUCropClass->getManureDay(2)>0)  //pig solid
               {
            	  double amountN = aNCUCropClass->getPigSolid();
            	  double theNH4N =0.25 * amountN;
                  double theNO3N = 0.0;
	              double theOrgN = amountN - (theNH4N + theNO3N);
	              double theamountC =aNCUCropClass->getPigSolidC();
	              if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("pig solid manure without C in ", double(NCU));
                  fieldOpFert *afieldOpFert = new fieldOpFert();
                  afieldOpFert->DefineOp("PigSolid", 3, calendarYear, aNCUCropClass->getManureDay(2), 5, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
               }
               if (aNCUCropClass->getManureDay(4)>0)  //poultry solid
               {
            	  double amountN = aNCUCropClass->getPoultrySolid();
            	  double theNH4N =0.25 * amountN;
                  double theNO3N = 0.0;
	              double theOrgN = amountN - (theNH4N + theNO3N);
	              double theamountC =aNCUCropClass->getPoultrySolidC();
	              if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("poultry solid manure without C in ", double(NCU));
	              fieldOpFert *afieldOpFert = new fieldOpFert();
                  afieldOpFert->DefineOp("PoultrySolid", 3, calendarYear, aNCUCropClass->getManureDay(4), 7, theNH4N, theNO3N, theOrgN,  theamountC, solidApplicMethod);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
               }

            }
            //the following operations are always performed in the current year
            if (aNCUCropClass->getManureDay(1)>0)  //cattle slurry
            {
               double amountN = aNCUCropClass->getCattleSlurry();
               double theNH4N =0.5 * amountN;
               double theNO3N = 0.0;
               double theOrgN = amountN - (theNH4N + theNO3N);
               double theamountC =aNCUCropClass->getCattleSlurryC();
               if (theamountC==0.0)
            	  theMessage->WarningWithDisplay("cattle slurry without C in ", double(NCU));
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("CattleSlurry", 3, calendarYear, aNCUCropClass->getManureDay(1), 2, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }
            if (aNCUCropClass->getManureDay(3)>0)  //pig slurry
            {
               double amountN = aNCUCropClass->getPigSlurry();
               double theNH4N =0.7 * amountN;
               double theNO3N = 0.0;
               double theOrgN = amountN - (theNH4N + theNO3N);
               double theamountC = aNCUCropClass->getPigSlurryC();
               if (theamountC==0.0)
            	   theMessage->WarningWithDisplay("pig slurry without C in ", double(NCU));
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("PigSlurry", 3, calendarYear, aNCUCropClass->getManureDay(3), 4, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }

            if (aNCUCropClass->getManureDay(5)>0)  //poultry slurry
            {
               double amountN = aNCUCropClass->getPoultrySlurry();
               double theNH4N =0.90 * amountN;
               double theNO3N = 0.0;
               double theOrgN = amountN - (theNH4N + theNO3N);
               double theamountC = aNCUCropClass->getPoultrySlurryC();
	           if (theamountC==0.0)
	        	   theMessage->WarningWithDisplay("poultry slurry without C in ", double(NCU));
	           fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("PoultrySlurry", 3, calendarYear, aNCUCropClass->getManureDay(5), 6, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }

            if (aNCUCropClass->getFertDay(0)>0)  //fertilising with urea fertiliser
            {
               double amountN = aNCUCropClass->getUreaFertiliser();
               double theNH4N = amountN;
               double theNO3N = 0.0;
               double theOrgN = 0.0;
               double theamountC =(12/28) * amountN;
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("Urea", 3, calendarYear, aNCUCropClass->getFertDay(0), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }

            if (aNCUCropClass->getFertDay(1)>0)  //fertilising with nitrate fertiliser
            {
               double amountN = aNCUCropClass->getNitrateFertiliser();
               double theNH4N = 0.0;
               double theNO3N = amountN;
               double theOrgN = 0.0;
               double theamountC =0.0;
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("Nitrate", 3, calendarYear, aNCUCropClass->getFertDay(1), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }

            if (aNCUCropClass->getHarvestingDay(0)>0)  //harvesting
            {
               double amountN = aNCUCropClass->getHarvestingDay(0);
               bool collectStraw = true;
               fieldOpHarv *afieldOpHarv = new fieldOpHarv();
               afieldOpHarv->DefineOp("Harvesting", 9, calendarYear, aNCUCropClass->getHarvestingDay(0), collectStraw);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpHarv);
            }
            if (aNCUCropClass->getAtmosphericNdep()>0)  //atmospheric deposition
            {
               double amountN = aNCUCropClass->getAtmosphericNdep();
               double theNH4N =0.5 * amountN;
               double theNO3N = 0.5 * amountN;;
               double theOrgN = 0.0;
               double theamountC = 0.0;
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("AtmosphericN", 10, calendarYear, 1,1, theNH4N, theNO3N, theOrgN, theamountC, 0);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }
            if (aNCUCropClass->getManureDay(6)>0)  //excreta
            {
               double amountN = aNCUCropClass->getGrazing();
               double theNH4N =0.5 * amountN;
               double theNO3N = 0.5 * amountN;
               double theOrgN = amountN - (theNH4N + theNO3N);
               double theamountC =aNCUCropClass->getGrazingC();
               if (theamountC==0.0)
            	   theMessage->WarningWithDisplay("excreta without C in ", double(NCU));
               fieldOpFert *afieldOpFert = new fieldOpFert();
               afieldOpFert->DefineOp("Excreta", 3, calendarYear, aNCUCropClass->getManureDay(6), 10, theNH4N, theNO3N, theOrgN,  theamountC, solidApplicMethod);
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
            }
            if (aNCUCropClass->getGrazingStartDay(0)>0)  //grazing starts
            {
               fieldOpGrass *afieldOpGrass = new fieldOpGrass();
               afieldOpGrass->DefineOp("StartGrazing", 5, calendarYear, aNCUCropClass->getGrazingStartDay(0), 1.0);  //default postgrazing LAI = 1.0
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpGrass);
            }
            if (aNCUCropClass->getGrazingStopDay(0)>0)  //grazing stops
            {
               fieldOpGrass *afieldOpGrass = new fieldOpGrass();
               afieldOpGrass->DefineOp("StopGrazing", 6, calendarYear, aNCUCropClass->getGrazingStopDay(0));  //LAI not used
               acropSeriesClass->gettheOperations()->InsertLast(afieldOpGrass);
            }
            if (aNCUCropClass->getCuttingDay(0)>0)  //cutting
            {
               fieldOpCut *afieldOp = new fieldOpCut();
               afieldOp->DefineOp("Cutting", 4, calendarYear, aNCUCropClass->getCuttingDay(0)); //default postgrazing LAI = 0.25
               acropSeriesClass->gettheOperations()->InsertLast(afieldOp);
            }
         }

         if (nextCropClass)
         {
            if (nextCropClass->getWinterCrop())
            {
               if (nextCropClass->getPloughingDay(0)>0)
               {
                  fieldOpTill *afieldOpTill = new fieldOpTill();
                  afieldOpTill->DefineOp("Ploughing", 1, calendarYear, nextCropClass->getPloughingDay(0), 0.25);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpTill);
               }
               if (nextCropClass->getSowingDay(0)>0)
               {
                  fieldOpSow *afieldOpSow = new fieldOpSow();
                  afieldOpSow->DefineOp("Sowing", 2, calendarYear, nextCropClass->getSowingDay(0), nextCropClass->getCropName());
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpSow);
               }
               if (nextCropClass->getManureDay(0)>0)  //cattle solid
               {
            	  double amountN = nextCropClass->getCattleSolid();
            	  double theNH4N =0.25 * amountN;
                  double theNO3N = 0.0;
	              double theOrgN = amountN - (theNH4N + theNO3N);
	              double theamountC = nextCropClass->getCattleSolidC();
	              if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("cattle solid manure without C in ", double(NCU));
                  fieldOpFert *afieldOpFert = new fieldOpFert();
                  afieldOpFert->DefineOp("CattleSolid", 3, calendarYear, nextCropClass->getManureDay(0), 3, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
               }
               if (nextCropClass->getManureDay(2)>0)  //pig solid
               {
            	  double amountN = nextCropClass->getPigSolid();
            	  double theNH4N =0.25 * amountN;
                  double theNO3N = 0.0;
	              double theOrgN = amountN - (theNH4N + theNO3N);
	              double theamountC = nextCropClass->getPigSolidC();
	              if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("pig solid manure without C in ", double(NCU));
                  fieldOpFert *afieldOpFert = new fieldOpFert();
                  afieldOpFert->DefineOp("PigSolid", 3, calendarYear, nextCropClass->getManureDay(2), 5, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
                  acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
               }
               if (nextCropClass->getManureDay(4)>0)  //poultry solid
               {
            	 double amountN = nextCropClass->getPoultrySolid();
            	 double theNH4N =0.25 * amountN;
                 double theNO3N = 0.0;
	             double theOrgN = amountN - (theNH4N + theNO3N);
                 double theamountC = nextCropClass->getPoultrySolidC();
                 if (theamountC==0.0)
	            	  theMessage->WarningWithDisplay("poultry solid manure without C in ", double(NCU));
	             fieldOpFert *afieldOpFert = new fieldOpFert();
                 afieldOpFert->DefineOp("PoultrySolid", 3, calendarYear, nextCropClass->getManureDay(4), 7, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
                 acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
               }
            }
         }
   	}
    acropSeriesClass->sortOps();
    for (int j=0; j<acropSeriesClass->gettheOperations()->NumOfNodes(); j++)
    {
    	fieldOp *afieldOp = acropSeriesClass->gettheOperations()->ElementAtNumber(j);
        	afieldOp->SetverbosePrint(verbosity);
    }
    theSeriesOperations->InsertLast(acropSeriesClass);
  }

}

void NCUClass::outputOps(fstream * outfile)
{
	for (int seriesNo=0; seriesNo<theSeriesOperations->NumOfNodes(); seriesNo++)
   {
   	cropSeriesClass * acropSeriesClass = theSeriesOperations->ElementAtNumber(seriesNo);
      if (acropSeriesClass)
	      acropSeriesClass->OutputOps(outfile, NCU);
   }

}

void NCUClass::loadReplacements(char inputDir[256])
{
   char inputFileName[150];
	char replacementFileName[50] = "replacements.txt";
	strcpy(inputFileName,inputDir);
	strcat(inputFileName,replacementFileName);
	fstream *afile = new fstream();
   afile->open(inputFileName,fstream::in);
   if (afile==NULL)
    	theMessage->FatalError("NCUClass::loadReplacements: file ", inputFileName, " not found");
   int i=0;
   char dummy[5000];
   string wantCrop;
   string repCrop;
   bool finished = false;
   while (!finished)
   {
	   afile->getline(dummy,5000,'\n');
      if (strlen(dummy)==0)
      	finished=true;
      else
      {
#ifdef __BCplusplus__	    
	      istrstream inputString(dummy,strlen(dummy));
#else      
  	   stringstream inputString(stringstream::in | stringstream::out);
		inputString << dummy;
#endif
	   inputString >> wantCrop >> repCrop;
	     replacements[i][0] =wantCrop;
	     replacements[i][1] = repCrop;
		 if (verbosity)
			 cout << "Want " << wantCrop << " Get " << repCrop << endl;
	     i++;
      }
   }
   afile->close();
   delete afile;
}

void NCUClass::getReplacements(string *aCrop)
{
	int i=0;
   string astring = "WRYE";
   while (replacements[i][0].length()>0)
   {
   	if (replacements[i][0].compare(*aCrop)==0)
      	 *aCrop=replacements[i][1];
      i++;
   }
  	if (astring.compare(*aCrop)==0)
   cout << *aCrop  << endl;
}


double NCUClass::getAnnualPrecip(int yearNo)
{
	double ret_val=0.0;
	for (int i=0;i<366; i++)
		ret_val+=getDailyPrecip(yearNo,i);
	return ret_val;
}

double NCUClass::getAnnualMet(int variableNo, int yearNo)
{
	double ret_val=0.0;
	switch (variableNo)
	{
	case 0:
	case 1:theMessage->FatalError("NCUClass::getAnnualMet: not implemented for this variable");
	case 2:ret_val=getAnnualPrecip(yearNo);
		break;
	default:theMessage->FatalError("NCUClass::getAnnualMet: not implemented for this variable");
	}
return ret_val;
}

void NCUClass::writeAnnualMet(fstream *outfileMet)
{
	for (int yearNum=0;yearNum<theYearPlans->NumOfNodes();yearNum++)
	*outfileMet << NCUI << '\t' << getAnnualMet(2, yearNum);
}

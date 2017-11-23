

// Class: NCUClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "NCUClass.h"
#include "climateNCU_MARS_ATEAM.05.Extract_Data.Func.V2.h"
#include "base\message.h"
#include "operationData.h"
#include "fieldOp.h"
#include "fieldOpTill.h"
#include "fieldOpSow.h"
#include "fieldOpFert.h"
#include "fieldOpHarv.h"
#include "fieldOpGrass.h"
#include "fieldOpCut.h"
#include "base\nixcbits.h"
#ifdef __ANSICPP__
	#include <cmath>
	#include <istream>
	#include <iostream>
	#include <cstring>
#else
	#include <fstream.h>
	#include <iostream.h>

#endif
#include "tools/fileAccess.h"
#include "tools/convertData.h"
#include <vector>
extern int getNCU_ID(int,char[100]);
extern int getNCUfromHSMU(int,char[100]);
extern bool verbosity;
extern int routeNumber;

NCUClass::NCUClass ( )
{
 NCU=0;
 NCUI=0;
 N_added=0.0;
};

NCUClass:: ~ NCUClass ( )
{
	delete theYearPlans;
	delete theSeriesOperations;
};

void NCUClass::Initialise(string inputDir, int thenumFields)
{
//load list of replacement crops to be used for those crops not present in CGMS data
	loadReplacements(inputDir);

   theYearPlans= new linkList<cropRotationClass>;
   theSeriesOperations = new linkList<cropSeriesClass>;
   maxFields = thenumFields;
}


int NCUClass::ReadData(int *aNCU, int startDay,  int startYear,  int endDay,  int endYear,  int baseYear, int range,
		string metdataInputDir, string tpvStem, string radStem, string ncu_mars, string inputDir, fstream * rotationFile,
      bool bootstrap)
{

	int ret_val=0;
	string aString;
   for (int year=startYear;year<(endYear+1); year++)
   {
      cropRotationClass * acropRotationClass = new cropRotationClass();
      acropRotationClass->setYear(year);
      theYearPlans->InsertLast(acropRotationClass);
   }

#ifdef __BCplusplus__
	char dummy[5000];
#endif
    if (!bootstrap)   //if choice of NCUs is an input
    {
      NCU = *aNCU;
      char tempString[256];
      strcpy(tempString,ncu_mars.c_str());
      NCUI = getNCU_ID(NCU,tempString);
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
    	char tempString[256];
        strcpy(tempString,ncu_mars.c_str());
        NCU = getNCUfromHSMU(NCUI,tempString);
        *aNCU = NCU;   //to inform calling class of NCU
      	if (NCU==3491)
    	  cout << "here";
    }

   if (verbosity)
	   cout << " NCUI " << NCUI << endl;

   int fieldNum=0; //crop number

   //there is an extra space character in input file between the NCUID and first crop code, compared to between crop codes
   char achar;
   rotationFile->get(achar);
   //loop through crop series
   int startCol = startYear - baseYear + 1;
   while (fieldNum<maxFields)
   {
	   ReadCropSequence(fieldNum, startCol, rotationFile);
      fieldNum++;
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

void NCUClass::ReadCropSequence(int fieldNum, int startCol, fstream * rotationFile)
{

#ifdef __BCplusplus__
	char dummy[5000];
#else
	string testString;
#endif
	char str7[7];

	  bool abort=false;
      int row=0;
      int maxRows = 15; //number of variables per crop
      while (row<=maxRows)
      {
      	if (!abort)
      	{
#ifdef __BCplusplus__
	         rotationFile->getline(dummy,5000,'\n');
#else
      		testString.clear();
      		getline(*rotationFile, testString);
#endif
      	}
		if (verbosity)
		{
         	cout << "Crop sequence " << fieldNum << " Row number " << row << endl;
          	cout << testString << endl;
		}
#ifdef __BCplusplus__
		istrstream inputString(dummy,strlen(dummy));
#else
  		stringstream inputString(stringstream::in | stringstream::out);
   		inputString << testString;
#endif
        //find first year's data
		int leadIn;
		if (row==0)
			leadIn =startCol-1;
		else
		{
			//first read and discard the NCUID
			int dumint;
			inputString >> dumint;
			leadIn=startCol-1;
		}
		for (int i=0; i<leadIn; i++)
		{
			inputString >> str7;
			if (verbosity)
			{
				cout << i << " " << str7 << endl;
			}
		}
		int yearNum=0;
		bool first = true;
		while (yearNum<theYearPlans->NumOfNodes())
        {
			cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNum);
			NCUCropClass *aNCUCropClass;
			if (row==0)
			{
				aNCUCropClass = new NCUCropClass();
				aNCUCropClass->Initialise();
				if (verbosity)
					cout << yearNum << " ";
				abort = aNCUCropClass->ReadData(&inputString, row, first);
				first=false;
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
					aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(fieldNum);
					if (aNCUCropClass)
						aNCUCropClass->ReadData(&inputString, row, false);
				}
			}
			yearNum++;
        }
		row++;
		if (verbosity)
			cout << endl;
      }
}

void NCUClass::calcCropManagement(string inputDir, string outputDir,int startYear)
{
	bool abort = false;
   int yearNo=0;
   int year = startYear;
   char inputFileName[256];

	//get timing of sowing of spring barley (used for timing of fertilisation of winter crops)
	//get crop rotation for required number of years
	while (yearNo<(theYearPlans->NumOfNodes()))
   {
		year = startYear + yearNo;
	   strcpy(inputFileName,"SpBarl.txt");
	   springBarleySowing=getCropEventDate(inputDir, inputFileName, MARS, 1, true, year);
	   strcpy(inputFileName,"WinWheat.txt");
	   winterWheatSowing=getCropEventDate(inputDir, inputFileName, MARS, 1, true, year);
	   int lengthOfGrowingSeason = winterWheatSowing - springBarleySowing + 21;
	   cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNo);
	   for (int cropNo = 0; cropNo<acropRotationClass->gettheCrops()->NumOfNodes(); cropNo++)
	   {
         NCUCropClass * aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(cropNo);
         if (aNCUCropClass)
         {
            string cropName = aNCUCropClass->getCropName();
    		if (!cropName.length()==4)
    			theMessage->FatalError("Crop name not 4 characters ",cropName.c_str());

            //see if this crop needs to be replaced by one for which crop timing is available
            string * localName = new string(cropName);
            getReplacements(localName);
    		if (!localName->length()==4)
    			theMessage->FatalError("Crop name not 4 characters ",localName->c_str());
            int sowingDay=0;
            int harvestingDate=0;
            int defaultsowingDay;  //Europe-wide average date, for situations where there is no sowing date within a NUTS region
            int diff = 0;//default difference between sowing and harvesting date, for when harvesting date is missing
            bool gotit = false;
            if ((localName->compare("SWHE")==0) || (localName->compare("DWHE")==0))
            {
               strcpy(inputFileName,"SpWheat.txt");
               diff = 126;
               defaultsowingDay = 99;
               gotit = true;
            }
            if (localName->compare("POTA")==0)
            {
               strcpy(inputFileName,"Potato.txt");
               diff = 137;
               defaultsowingDay=115;
               gotit = true;
            }
            if (localName->compare("BARL")==0)
            {
               strcpy(inputFileName,"SpBarl.txt");
               diff = 128;
               defaultsowingDay=92;
               gotit = true;
            }
            if (localName->compare("MAIF")==0)
            {
               strcpy(inputFileName,"FodMaize.txt");
               diff = 112;
               defaultsowingDay=124;
               gotit = true;
            }
            if (localName->compare("MAIZ")==0)
            {
               strcpy(inputFileName,"GraMaize.txt");
               diff = 165;
               defaultsowingDay=117;
               gotit = true;
            }
            if (localName->compare("RAPE")==0)
            {
               strcpy(inputFileName,"WinRape.txt");
               diff = 306;
               defaultsowingDay=243;
            	aNCUCropClass->setWinterCrop(true);
               gotit = true;
            }
            if (localName->compare("SUGB")==0)
            {
               strcpy(inputFileName,"SugBeet.txt");
               diff = 169;
               defaultsowingDay=108;
               gotit = true;
            }
          if (localName->compare("SUNF")==0)
            {
               strcpy(inputFileName,"SunFlower.txt");
               diff = 150;
               defaultsowingDay=106;
               gotit = true;
            }
            if (localName->compare("WWHE")==0)
            {
               strcpy(inputFileName,"WinWheat.txt");
               diff = 283;
               defaultsowingDay=279;
            	aNCUCropClass->setWinterCrop(true);
               gotit = true;
            }

            if ((localName->compare("SETA")==0)||(localName->compare("GRAS")==0)||(localName->compare("GRAP")==0))
            {
                strcpy(inputFileName,"WinWheat.txt");
                diff = 283;
                defaultsowingDay=279;
             	aNCUCropClass->setWinterCrop(true);
                gotit = true;
            }

      //need values for winter barley, spring rape
            if (!gotit)
            {
            	cout << "Error in year " << year << " and crop " << cropNo << endl;
               theMessage->FatalError("File not found for ",localName->c_str());
            }
            sowingDay=getCropEventDate(inputDir, inputFileName, MARS, 1, true, year);
            if (sowingDay == 999)
               sowingDay=defaultsowingDay;
            if ((!localName->compare("GRAP")==0)||(yearNo==0)) //do sowing for all crops except permanent grassland (for that too, in first year
            	aNCUCropClass->setSowingDay(sowingDay,0);
            if (localName->compare("SETA")!=0)   //skip harvesting for set-aside
            {

                if ((localName->compare("GRAS")==0)||(localName->compare("GRAP")==0))
                {
                	//default dates for start and stop of grazing
                    aNCUCropClass->setGrazingStartDay(springBarleySowing,0);
                    aNCUCropClass->setGrazingStopDay(springBarleySowing+lengthOfGrowingSeason,0);
                    //assume 1 month between cuts
                    int numberOfCuts = round(lengthOfGrowingSeason/30);
                    if (numberOfCuts>10)
                    	theMessage->FatalError("Too many grass cuts");
                	int cutDate = springBarleySowing + 28;
                	for (int cutNo=0; cutNo<numberOfCuts; cutNo++)
                	{
                        aNCUCropClass->setCuttingDay(harvestingDate,cutNo);
                        cutDate+=28;
                	}
                }
                else	//is not grass or SETA
                {
                	harvestingDate=getCropEventDate(inputDir, inputFileName, MARS,2, true, year);
                	if (harvestingDate==999)
                	{
                		harvestingDate=sowingDay+diff;
                		if (harvestingDate>365)
                			harvestingDate-=365;
                	}
					aNCUCropClass->setHarvestingDay(harvestingDate,0);
                }
            }// end not SETA
            delete localName;
      	}  //end if aNCUCropClass not NULL
   	}
	yearNo++;
   } //end of year loop

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
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,0,0);
            if (aNCUCropClass->getPigSolid()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,2,0);
            if (aNCUCropClass->getPoultrySolid()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,4,0);
            if (aNCUCropClass->getGrazing()>0.0)
               aNCUCropClass->setManureDay(aNCUCropClass->getSowingDay(0)-5,6,0);

            if (!aNCUCropClass->getWinterCrop()) //is a spring crop
            {
            	int sowingDay = aNCUCropClass->getSowingDay(0);
            	int harvestingDay = aNCUCropClass->getHarvestingDay(0);
            	int growthPeriod = harvestingDay - sowingDay;
               //all fertilisation and liquid manuring of spring crops occurs just prior to sowing
               if (aNCUCropClass->getCattleSlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(sowingDay-5,1,0);
               }
               if (aNCUCropClass->getPigSlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(sowingDay-5,3,0);
               }
               if (aNCUCropClass->getPoultrySlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(sowingDay-5,5,0);
               }

               if (aNCUCropClass->getUreaFertiliser()>0.0)
               {
                   aNCUCropClass->setFertDay(sowingDay-5,0,0);
                   aNCUCropClass->setFertDay((sowingDay+0.2*growthPeriod),0,1);
               }
               if (aNCUCropClass->getNitrateFertiliser()>0.0)
               {
                   aNCUCropClass->setFertDay(sowingDay-5,1,0);
                   aNCUCropClass->setFertDay((sowingDay+0.2*growthPeriod),1,1);
               }
            }
            else
            {
               // fertilisation and liquid manuring of winter crops occurs in the spring (assumed to be at same time as spring barley would be sown)
               if (aNCUCropClass->getCattleSlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(springBarleySowing,1,0);
               }
               if (aNCUCropClass->getPigSlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(springBarleySowing,3,0);
               }
               if (aNCUCropClass->getPoultrySlurry()>0.0)
               {
                   aNCUCropClass->setManureDay(springBarleySowing,5,0);
               }
               int harvestingDay = aNCUCropClass->getHarvestingDay(0);
               int growthPeriod = harvestingDay - springBarleySowing;
               if (aNCUCropClass->getUreaFertiliser()>0.0)
               {
                   aNCUCropClass->setFertDay(springBarleySowing,0,0);
                   aNCUCropClass->setFertDay((springBarleySowing + 0.2*growthPeriod),0,1);
               }
               if (aNCUCropClass->getNitrateFertiliser()>0.0)
               {
                   aNCUCropClass->setFertDay(springBarleySowing,1,0);
                   aNCUCropClass->setFertDay((springBarleySowing + 0.2*growthPeriod),1,1);
               }
            }
         }
      }
      yearNo++;
   }

};
struct years
{
	float JSOW;
	float JRIP;

};
struct product {
	vector <years> year;
	int marsCode;
} ;
struct File {
	char fileName[50];
	vector<product> fileContent;
} ;

static vector<File> fileList;
//! returns the date of sowing (1) or harvesting (2) for a particular MARS grid cell
int NCUClass::getCropEventDate(string inputDir, string MARSDateFileName, int MARScode, int dateID, bool recursive, int year)
{
	char inputFileName[256];
	strcpy(inputFileName,MARSDateFileName.c_str());
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
	   fileAccess hd;
	   hd.openFile(inputFileName);

	   int year=0;
	   hd.getLineFile();
	   while ( !hd.endOfFile())
	   {
		   string input=hd.getLineFile();
		   stringstream totalSString( input );
		   string nameOfItem;
		   totalSString >>nameOfItem;
		   if (verbosity)
			   cout << nameOfItem << " ";
		   convertData converts;
		   product item;

		   item.marsCode=converts.StringToInt(nameOfItem);
		   for(int i=1971;i<2031;i++)
		   {
			   years yearItem;
			   totalSString >>nameOfItem;
			   if (verbosity)
				   cout << i << " " << nameOfItem;
			   yearItem.JSOW=converts.StringToDouble(nameOfItem);
			   totalSString >>nameOfItem;
			   yearItem.JRIP=converts.StringToDouble(nameOfItem);
			   if (verbosity)
				   cout << " " << nameOfItem << endl;
			   //! Round up if necessary
			   double temp2=yearItem.JSOW-floor(yearItem.JSOW);
			   if (temp2>=0.5)
				   yearItem.JSOW+=1.0;
			   temp2=yearItem.JRIP-floor(yearItem.JRIP);
			   if (temp2>=0.5)
				   yearItem.JRIP+=1.0;
			   item.year.push_back(yearItem);
		   }
		   file.fileContent.push_back(item);
		   year++;
	   }
	   hd.closeFile();
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
				   DayNo=(int)item.fileContent.at(i).year[year-1971].JSOW;
			   if (dateID==2)//! get harvesting date
				   DayNo=(int)item.fileContent.at(i).year[year-1971].JRIP;
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
	   DayNo=getAverageCropEventDate(inputDir, MARSDateFileName, dateID, year);
    }
    return DayNo;
}

struct product1 {
	int nutsCode;
	int marsCode;
} ;

static vector<product1> productList;

//!Returns the average date within a NUTS region on which an operation is performed
int NCUClass::getAverageCropEventDate(string inputDir, string MARSDateFileName, int dateID, int year)
{
	int retVal=999;
//!create linklist of ops data
   linkList<operationData> *theOpsData;
   theOpsData = new linkList<operationData>;
	char inputFileName[256];
	strcpy(inputFileName,inputDir.c_str());
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
		      	int adate=getCropEventDate(inputDir, MARSDateFileName, productMARS, dateID, false, year);
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
	for (int fieldNo=0; fieldNo<maxFields; fieldNo++)
	{
		cropSeriesClass * acropSeriesClass = new cropSeriesClass(fieldNo);

		for (int yearNum=0;yearNum<theYearPlans->NumOfNodes()-1;yearNum++)
		{
         cropRotationClass * acropRotationClass = theYearPlans->ElementAtNumber(yearNum);	//get the rotation for this year
         int calendarYear= acropRotationClass->getYear();
         cropRotationClass * nextcropRotationClass;
         if (yearNum<(theYearPlans->NumOfNodes()-1))
            nextcropRotationClass = theYearPlans->ElementAtNumber(yearNum+1);	//get the rotation for following year, if relevant
         else
            nextcropRotationClass=NULL;
         NCUCropClass * aNCUCropClass = acropRotationClass->gettheCrops()->ElementAtNumber(fieldNo);	//get the crop grown on this field in the current year
         NCUCropClass * nextCropClass;
         if (yearNum<(theYearPlans->NumOfNodes()-1))
            nextCropClass = nextcropRotationClass->gettheCrops()->ElementAtNumber(fieldNo);	//get the crop grown on this field in the following year
         else
            nextCropClass=NULL;
         if (aNCUCropClass)	//do operations on the current year's crop
        	 doCurrentSpringSummerCropOperations(acropSeriesClass,aNCUCropClass, calendarYear);
		 if (nextCropClass)
		 {
			 if (zeroTillage)
				 doZeroTillage(acropSeriesClass, aNCUCropClass, nextCropClass, calendarYear);
			if (nextCropClass->getWinterCrop())	//if next year's crop is a winter crop, do the operations that need to be done in the current year
            {
				doNextCropAutumnOperations(acropSeriesClass, nextCropClass, calendarYear);
            }
			else
			    if (coverCropping)
			    	doCoverCrop(acropSeriesClass, aNCUCropClass, calendarYear);
		 }
	   }
		//should now have all the operations loaded, so sort them into chronological order
		acropSeriesClass->sortOps();
		for (int j=0; j<acropSeriesClass->gettheOperations()->NumOfNodes(); j++)
		{
			fieldOp *afieldOp = acropSeriesClass->gettheOperations()->ElementAtNumber(j);
			afieldOp->SetverbosePrint(verbosity);
		}
		//store series
		theSeriesOperations->InsertLast(acropSeriesClass);
  }

}

void NCUClass::doCurrentSpringSummerCropOperations(cropSeriesClass * acropSeriesClass,NCUCropClass *aNCUCropClass, int calendarYear)
{
    if (!aNCUCropClass->getWinterCrop())
    {
    //start with ploughing
    // get first ploughing date
    	if (!zeroTillage)
    	{
		   if (aNCUCropClass->getPloughingDay(0)>0)
		   {
			  fieldOpTill *afieldOpTill = new fieldOpTill();
			  afieldOpTill->DefineOp("Ploughing", 1, calendarYear, aNCUCropClass->getPloughingDay(0), 0.25);
			  acropSeriesClass->gettheOperations()->InsertLast(afieldOpTill);
		   }
    	}
       if (aNCUCropClass->getSowingDay(0)>0)
       {
          fieldOpSow *afieldOpSow = new fieldOpSow();
          afieldOpSow->DefineOp("Sowing", 2, calendarYear, aNCUCropClass->getSowingDay(0), aNCUCropClass->getCropName());
          acropSeriesClass->gettheOperations()->InsertLast(afieldOpSow);
       }

       doSolidManureOperations(acropSeriesClass, aNCUCropClass, calendarYear);
    }
    //the following operations are always performed in the current year

    doSlurryOperations(acropSeriesClass, aNCUCropClass, calendarYear);
    doFertiliserOperations(acropSeriesClass, aNCUCropClass, calendarYear);

    if (aNCUCropClass->getHarvestingDay(0)>0)  //harvesting
    {
       bool collectStraw = true;
       fieldOpHarv *afieldOpHarv = new fieldOpHarv();
       afieldOpHarv->DefineOp("Harvesting", 9, calendarYear, aNCUCropClass->getHarvestingDay(0), collectStraw);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpHarv);
    }
    if (aNCUCropClass->getAtmosphericNdep()>0)  //atmospheric deposition
    {
       double amountN = aNCUCropClass->getAtmosphericNdep();
       AddN(amountN);
       double theNH4N =0.5 * amountN;
       double theNO3N = 0.5 * amountN;;
       double theOrgN = 0.0;
       double theamountC = 0.0;
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("AtmosphericN", 10, calendarYear, 1,1, theNH4N, theNO3N, theOrgN, theamountC, 0);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }
    if (aNCUCropClass->getManureDay(6,0)>0)  //excreta
    {
       double amountN = aNCUCropClass->getGrazing();
       AddN(amountN);
       double theNH4N =0.5 * amountN;
       double theNO3N = 0.5 * amountN;
       double theOrgN = amountN - (theNH4N + theNO3N);
       double theamountC =aNCUCropClass->getGrazingC();
       if (theamountC==0.0)
    	   theMessage->WarningWithDisplay("excreta without C in ", double(NCU));
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("Excreta", 3, calendarYear, aNCUCropClass->getManureDay(6,0), 10, theNH4N, theNO3N, theOrgN,  theamountC, solidApplicMethod);
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


void NCUClass::doNextCropAutumnOperations(cropSeriesClass * acropSeriesClass,NCUCropClass *nextCropClass, int calendarYear)
{
	if (!zeroTillage)
	{
		if (nextCropClass->getPloughingDay(0)>0)
		{
		   fieldOpTill *afieldOpTill = new fieldOpTill();
		   afieldOpTill->DefineOp("Ploughing", 1, calendarYear, nextCropClass->getPloughingDay(0), 0.25);
		   acropSeriesClass->gettheOperations()->InsertLast(afieldOpTill);
		}
	}
	if (nextCropClass->getSowingDay(0)>0)
	{
	   fieldOpSow *afieldOpSow = new fieldOpSow();
	   afieldOpSow->DefineOp("Sowing", 2, calendarYear, nextCropClass->getSowingDay(0), nextCropClass->getCropName());
	   acropSeriesClass->gettheOperations()->InsertLast(afieldOpSow);
	}
    doSolidManureOperations(acropSeriesClass, nextCropClass, calendarYear);
}

void NCUClass::doSolidManureOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear)
{
    if (aNCUCropClass->getManureDay(0,0)>0)  //cattle solid - first and only application
    {
		double amountN = aNCUCropClass->getCattleSolid();
		AddN(amountN);
		double theNH4N =0.25 * amountN;
		double theNO3N = 0.0;
		double theOrgN = amountN - (theNH4N + theNO3N);
		double theamountC = aNCUCropClass->getCattleSolidC();
		if (theamountC==0.0)
		  theMessage->WarningWithDisplay("cattle solid manure without C in ", double(NCU));
		fieldOpFert *afieldOpFert = new fieldOpFert();
		afieldOpFert->DefineOp("CattleSolid", 3, calendarYear, aNCUCropClass->getManureDay(0,0), 3, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
		acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
	}
    if (aNCUCropClass->getManureDay(2,0)>0)  //pig solid - first and only application
    {
		double amountN = aNCUCropClass->getPigSolid();
		AddN(amountN);
		double theNH4N =0.25 * amountN;
		double theNO3N = 0.0;
		double theOrgN = amountN - (theNH4N + theNO3N);
		double theamountC =aNCUCropClass->getPigSolidC();
		if (theamountC==0.0)
		  theMessage->WarningWithDisplay("pig solid manure without C in ", double(NCU));
		fieldOpFert *afieldOpFert = new fieldOpFert();
		afieldOpFert->DefineOp("PigSolid", 3, calendarYear, aNCUCropClass->getManureDay(2,0), 5, theNH4N, theNO3N, theOrgN, theamountC, solidApplicMethod);
		acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }
    if (aNCUCropClass->getManureDay(4,0)>0)  //poultry solid - first and only application
    {
		double amountN = aNCUCropClass->getPoultrySolid();
		AddN(amountN);
		double theNH4N =0.25 * amountN;
		double theNO3N = 0.0;
		double theOrgN = amountN - (theNH4N + theNO3N);
		double theamountC =aNCUCropClass->getPoultrySolidC();
		if (theamountC==0.0)
		theMessage->WarningWithDisplay("poultry solid manure without C in ", double(NCU));
		fieldOpFert *afieldOpFert = new fieldOpFert();
		afieldOpFert->DefineOp("PoultrySolid", 3, calendarYear, aNCUCropClass->getManureDay(4,0), 7, theNH4N, theNO3N, theOrgN,  theamountC, solidApplicMethod);
		acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }
}

void NCUClass::doSlurryOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear)
{
    if (aNCUCropClass->getManureDay(1,0)>0)  //cattle slurry - first and only application
    {
       double amountN = aNCUCropClass->getCattleSlurry();
       AddN(amountN);
       double theNH4N =0.5 * amountN;
       double theNO3N = 0.0;
       double theOrgN = amountN - (theNH4N + theNO3N);
       double theamountC =aNCUCropClass->getCattleSlurryC();
       if (theamountC==0.0)
    	  theMessage->WarningWithDisplay("cattle slurry without C in ", double(NCU));
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("CattleSlurry", 3, calendarYear, aNCUCropClass->getManureDay(1,0), 2, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }
    if (aNCUCropClass->getManureDay(3,0)>0)  //pig slurry - first and only application
    {
       double amountN = aNCUCropClass->getPigSlurry();
       AddN(amountN);
       double theNH4N =0.7 * amountN;
       double theNO3N = 0.0;
       double theOrgN = amountN - (theNH4N + theNO3N);
       double theamountC = aNCUCropClass->getPigSlurryC();
       if (theamountC==0.0)
    	   theMessage->WarningWithDisplay("pig slurry without C in ", double(NCU));
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("PigSlurry", 3, calendarYear, aNCUCropClass->getManureDay(3,0), 4, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

    if (aNCUCropClass->getManureDay(5,0)>0)  //poultry slurry - first and only application
    {
       double amountN = aNCUCropClass->getPoultrySlurry();
       AddN(amountN);
       double theNH4N =0.90 * amountN;
       double theNO3N = 0.0;
       double theOrgN = amountN - (theNH4N + theNO3N);
       double theamountC = aNCUCropClass->getPoultrySlurryC();
       if (theamountC==0.0)
    	   theMessage->WarningWithDisplay("poultry slurry without C in ", double(NCU));
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("PoultrySlurry", 3, calendarYear, aNCUCropClass->getManureDay(5,0), 6, theNH4N, theNO3N, theOrgN, theamountC, slurryApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

}
void NCUClass::doFertiliserOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear)
{
    if (aNCUCropClass->getFertDay(0,0)>0)  //fertilising with urea fertiliser - first application
    {
       double amountN = aNCUCropClass->getUreaFertiliser() * aNCUCropClass->getpropFert(0);
       AddN(amountN);
       double theNH4N = amountN;
       double theNO3N = 0.0;
       double theOrgN = 0.0;
       double theamountC =(12/28) * amountN;
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("Urea", 3, calendarYear, aNCUCropClass->getFertDay(0,0), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

    if (aNCUCropClass->getFertDay(0,1)>0)  //fertilising with urea fertiliser - second application
    {
       double amountN = aNCUCropClass->getUreaFertiliser() * aNCUCropClass->getpropFert(1);
       AddN(amountN);
       double theNH4N = amountN;
       double theNO3N = 0.0;
       double theOrgN = 0.0;
       double theamountC =(12/28) * amountN;
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("Urea", 3, calendarYear, aNCUCropClass->getFertDay(0,1), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

    if (aNCUCropClass->getFertDay(1,0)>0)  //fertilising with nitrate fertiliser - first application
    {
       double amountN = aNCUCropClass->getNitrateFertiliser() * aNCUCropClass->getpropFert(0);
       AddN(amountN);
       double theNH4N = 0.0;
       double theNO3N = amountN;
       double theOrgN = 0.0;
       double theamountC =0.0;
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("Nitrate", 3, calendarYear, aNCUCropClass->getFertDay(1,0), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

    if (aNCUCropClass->getFertDay(1,1)>0)  //fertilising with nitrate fertiliser - second application
    {
       double amountN = aNCUCropClass->getNitrateFertiliser() * aNCUCropClass->getpropFert(1);
       AddN(amountN);
       double theNH4N = 0.0;
       double theNO3N = amountN;
       double theOrgN = 0.0;
       double theamountC =0.0;
       fieldOpFert *afieldOpFert = new fieldOpFert();
       afieldOpFert->DefineOp("Nitrate", 3, calendarYear, aNCUCropClass->getFertDay(1,1), 1, theNH4N, theNO3N, theOrgN,theamountC, ureaApplicMethod);
       acropSeriesClass->gettheOperations()->InsertLast(afieldOpFert);
    }

}

void NCUClass::outputOps(fstream * outfile)
{
	for (int fieldNo=0; fieldNo<theSeriesOperations->NumOfNodes(); fieldNo++)
   {
	if (verbosity)
		cout << NCUI << " " << fieldNo << " " << endl;
	cropSeriesClass * acropSeriesClass = theSeriesOperations->ElementAtNumber(fieldNo);
	if (acropSeriesClass)
	  acropSeriesClass->OutputOps(outfile, NCU);
   }
	CheckNBalance();
}

bool NCUClass::CheckNBalance()
{
	double NOut=0.0;
	for (int fieldNo=0; fieldNo<theSeriesOperations->NumOfNodes(); fieldNo++)
   {
	cropSeriesClass * acropSeriesClass = theSeriesOperations->ElementAtNumber(fieldNo);
	if (acropSeriesClass)
		NOut+=acropSeriesClass->GetNOutput();
   }
	if (!N_added==NOut)
	{
		cout << "N In " << N_added << " N output " << NOut << endl;
		theMessage->FatalError("NCUClass: N balance error");
	}
	else
		cout << "N balance OK" << endl;
	return true;
}
void NCUClass::loadReplacements(string inputDir)
{
   string inputFileName;
	inputFileName=inputDir + "replacements.txt";
	fstream *afile = new fstream();
   afile->open(inputFileName.c_str(),fstream::in);
   if (afile==NULL)
    	theMessage->FatalError("NCUClass::loadReplacements: file ", inputFileName.c_str(), " not found");
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

void NCUClass::doZeroTillage(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, NCUCropClass *nextCrop, int calendarYear)
{
	if ((aNCUCropClass->getCropName()=="GRAS")||(aNCUCropClass->getCropName()=="GRAP")||(aNCUCropClass->getCropName()=="SETA"))
		 //insert harvest to remove excess plant material, if one is not planned
	{
		bool collectStraw = true;
	   fieldOpHarv *afieldOpHarv = new fieldOpHarv();
	   afieldOpHarv->DefineOp("Harvesting", 9, calendarYear, nextCrop->getPloughingDay(0)-5, collectStraw);
	   acropSeriesClass->gettheOperations()->InsertLast(afieldOpHarv);
	}
}

void NCUClass::doCoverCrop(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear)
{
	if (!(strcmp(aNCUCropClass->getCropName().c_str(),"SETA")==0))
	{
		if (!zeroTillage) //prepare seedbed
		{
			fieldOpTill *afieldOpTill = new fieldOpTill();
			afieldOpTill->DefineOp("Ploughing", 1, calendarYear, aNCUCropClass->getHarvestingDay(0)+2, 0.25);
			acropSeriesClass->gettheOperations()->InsertLast(afieldOpTill);
		}
		//sow grass as cover crop
		fieldOpSow *afieldOpSow = new fieldOpSow();
		afieldOpSow->DefineOp("Sowing", 2, calendarYear, aNCUCropClass->getHarvestingDay(0)+3, "GRAS");
		acropSeriesClass->gettheOperations()->InsertLast(afieldOpSow);
	}
}

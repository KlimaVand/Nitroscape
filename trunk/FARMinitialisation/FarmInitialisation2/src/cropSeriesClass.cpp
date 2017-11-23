

#include "cropSeriesClass.h"
#include "comsort.h"
#include "fieldOpSow.h"
#include "fieldOpHarv.h"
#include "fieldOpTill.h"
#include "fieldOpFert.h"

extern XMLNode XMLdescription;

cropSeriesClass::cropSeriesClass (int theseriesNumber )
{
	seriesNumber = theseriesNumber;
	theOperations= new linkList<fieldOp>;
};

cropSeriesClass:: ~ cropSeriesClass ( )
{
	delete theOperations;
};

void cropSeriesClass::getcrops(XMLNode *fieldNode, int startYear, int endYear)
{
	int numNodes = fieldNode->nChildNode("crops");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xCropNode=fieldNode->getChildNode("crops",i);
		if (!xCropNode.isEmpty())
		{
			  XMLNode xNode;
			  xNode=xCropNode.getChildNode("cropsownplantdate");
			  if (xNode.getText())
			  {
				  fieldOpSow *afieldOpSow = new fieldOpSow();
				  string date(xNode.getText());
				  int year = getYear(date);
				  int month = getMonth(date);
				  int day = getDay(date);
				  bool doOp = false;
				  if ((year>=startYear)&&(year<=endYear))
				  {
					  doOp=true;
					  if ((year==startYear)&&(month<8))	//using cropping year, which starts 1 August
						  doOp=false;
				  }
				  if (doOp)
				  {
					  xNode=xCropNode.getChildNode("croptypeid");
					  if (xNode.getText())
					  {
						  string cropIDnumber(xNode.getText());
						  string *cropID=getCropID(cropIDnumber);
						  afieldOpSow->DefineOp ("Sowing", 2, year, month, day, *cropID);
						  theOperations->InsertLast(afieldOpSow);
					  }
					  else
						  printf("Sowing crop ID missing '%s'\n");
				  }
			  }
			  xNode=xCropNode.getChildNode("cropharvestdate");
			  if (xNode.getText())
			  {
				  fieldOpHarv *afieldOpHarv = new fieldOpHarv();
				  string date(xNode.getText());
				  int year = getYear(date);
				  int month = getMonth(date);
				  int day = getDay(date);
				  if ((year>startYear)&&(year<=endYear))	//no harvesting in first year
				  {
					  afieldOpHarv->DefineOp ("Harvesting", 9, year, month, day, false);
					  theOperations->InsertLast(afieldOpHarv);
				  }
			  }
		}
	}
};

string *cropSeriesClass::getCropID(string croptypeid)
{
	XMLNode xNode;
	string *theID;
	int numNodes = XMLdescription.nChildNode("croptype");
	XMLNode xcroptypeNode;
	xcroptypeNode=XMLdescription.getChildNode("croptype",0);
	bool gotit = false;
	int i = 0;
	while ((!gotit) && (i<numNodes))
	{
		xNode = xcroptypeNode.getChildNode("croptypeid");
		if (xNode.getText()==croptypeid)
			gotit=true;
		else
		{
			i++;
			xcroptypeNode=XMLdescription.getChildNode("croptype",i);
		}
	}
	if (!gotit)
	{
		  printf("Crop ID not found '%s'\n");
		  return NULL;
	}
	else
	{
		xNode = xcroptypeNode.getChildNode("croptypefasset_id",0);
		theID = new string (xNode.getText());
		return theID;
	}
}


void cropSeriesClass::gettillage(XMLNode *fieldNode,int startYear, int endYear)
{
	int numNodes = fieldNode->nChildNode("tillage");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xTillageNode=fieldNode->getChildNode("tillage",i);
		if (!xTillageNode.isEmpty())
		{
			  XMLNode xNode;
			  xNode=xTillageNode.getChildNode("tillagedate");
			  if (xNode.getText())
			  {
				  fieldOpTill *afieldOpTill = new fieldOpTill();
				  string date(xNode.getText());
				  int year = getYear(date);
				  int month = getMonth(date);
				  int day = getDay(date);
				  bool doOp = false;
				  if ((year>=startYear)&&(year<=endYear))
				  {
					  doOp=true;
					  if ((year==startYear)&&(month<8))	//using cropping year, which starts 1 August
						  doOp=false;
				  }
				  if (doOp)
				  {
					  xNode=xTillageNode.getChildNode("tillagetypeid");
					  if (xNode.getText())
					  {
						  string tillageType(xNode.getText());
						  float tillDepth=0.0;
						  if (tillageType=="2")
							  tillDepth=10.0;
						  if (tillageType=="3")
							  tillDepth=250.0;
						  afieldOpTill->DefineOp ("Tillage", 1, year, month, day, tillDepth);
						  theOperations->InsertLast(afieldOpTill);
					  }
					  else
						  printf("tillagetype missing '%s'\n");
				  }
			  }
		}
	}
};

void cropSeriesClass::getfertilisation(XMLNode *fieldNode,int startYear, int endYear)
{
	int numNodes = fieldNode->nChildNode("fertapps");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xFertNode=fieldNode->getChildNode("fertapps",i);
		if (!xFertNode.isEmpty())
		{
			  XMLNode xNode;
			  xNode=xFertNode.getChildNode("fertappdate");
			  if (xNode.getText())
			  {
				  fieldOpFert *afieldOpFert = new fieldOpFert();
				  string date(xNode.getText());
				  int year = getYear(date);
				  int month = getMonth(date);
				  int day = getDay(date);
				  bool doOp = false;
				  if ((year>=startYear)&&(year<=endYear))
				  {
					  doOp=true;
					  if ((year==startYear)&&(month<8))	//using cropping year, which starts 1 August
						  doOp=false;
				  }
				  if (doOp)
				  {
					  xNode=xFertNode.getChildNode("fertamount");
					  float amount=0.0;
					  if (xNode.getText())
					  {
						  string fertAmount(xNode.getText());
						  amount= atof(fertAmount.c_str());
					  }
					  else
						  printf("fert amount missing '%s'\n");

					  xNode=xFertNode.getChildNode("ferttypeid");
					  float ammonium=0.0;
					  float nitrate=0.0;
					  float organic=0.0;
					  if (xNode.getText())
					  {
						  string fertType(xNode.getText());
						  int theType = atoi(fertType.c_str());
//						  switch (theType)
//						  {
//							  case 2:
//							  {
//								  ammonium=0.5*amount;
//								  nitrate=0.5*amount;
//								  organic = 0.0;
//								  break;
//							  }
//							  case 5:
//							  {
//								  ammonium=0.5*amount;
//								  nitrate=0.5*amount;
//								  organic = 0.0;
//								  break;
//							  }
//						  }
						  ammonium=0.5*amount;
						  nitrate=0.5*amount;
						  organic = 0.0;
						  float carbon=0.0; //but not for urea!
						  xNode=xFertNode.getChildNode("fertappmethodid");
						  int method = 0;
						  if (xNode.getText())
						  {
							  string fertMeth(xNode.getText());
							  method= atoi(fertMeth.c_str());
							  afieldOpFert->DefineOp ("Fertilisation", 3, year, month, day, theType, ammonium, nitrate, organic,
								  					carbon,method);
							  theOperations->InsertLast(afieldOpFert);
						  }
						  else
							  printf("fertappmethodid missing '%s'\n");
					  }
					  else
						  printf("ferttypeid missing '%s'\n");
				  }
			  }
		}
	}
};

void cropSeriesClass::getmanure(XMLNode *fieldNode, int startYear, int endYear)
{
	int numNodes = fieldNode->nChildNode("manureapp");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xManureNode=fieldNode->getChildNode("manureapp",i);
		if (!xManureNode.isEmpty())
		{
			  XMLNode xNode;
			  xNode=xManureNode.getChildNode("manureappdate");
			  if (xNode.getText())
			  {
				  fieldOpFert *afieldOpFert = new fieldOpFert();
				  string date(xNode.getText());
				  int year = getYear(date);
				  int month = getMonth(date);
				  int day = getDay(date);
				  bool doOp = false;
				  if ((year>=startYear)&&(year<=endYear))
				  {
					  doOp=true;
					  if ((year==startYear)&&(month<8))	//using cropping year, which starts 1 August
						  doOp=false;
				  }
				  if (doOp)
				  {
					  xNode=xManureNode.getChildNode("manureamount");
					  float amount=0.0;
					  if (xNode.getText())
					  {
						  string fertAmount(xNode.getText());
						  amount= atof(fertAmount.c_str());
					  }
					  else
						  printf("manure amount missing '%s'\n");

					  xNode=xManureNode.getChildNode("manuretypeid");
					  float ammonium=0.0;
					  float nitrate=0.0;
					  float organic=0.0;
					  float carbon=0.0;
					  if (xNode.getText())
					  {
						  string manureType(xNode.getText());
						  int theType = atoi(manureType.c_str());
						  switch (theType)
						  {
							  case 2:
							  {
								  ammonium=0.7*amount;
								  nitrate=0.0;
								  organic = 0.3*amount;
								  break;
							  }
							  case 5:
							  {
								  ammonium=0.1*amount;
								  nitrate=0.0;
								  organic = 0.9*amount;
								  break;
							  }
						  }
						  carbon=12.0*organic;
						  xNode=xManureNode.getChildNode("manureappmethodid");
						  int method = 0;
						  if (xNode.getText())
						  {
							  string fertMeth(xNode.getText());
							  method= atoi(fertMeth.c_str());
							  afieldOpFert->DefineOp ("Fertilisation", 3, year, month, day, theType, ammonium, nitrate, organic,
									  carbon,method);
							  theOperations->InsertLast(afieldOpFert);
						  }
						  else
							  printf("manureappmethodid missing '%s'\n");
					  }
					  else
						  printf("manuretypeid missing '%s'\n");
				  }
			  }
		}
	}
};

void cropSeriesClass::addOperations(XMLNode *fieldNode,int startYear, int endYear)
{
	getcrops(fieldNode,startYear, endYear);
	gettillage(fieldNode,startYear, endYear);
	getfertilisation(fieldNode,startYear, endYear);
	getmanure(fieldNode,startYear, endYear);
	sortOps();

};


//! Sorts the field operations in order of starting date
void cropSeriesClass::sortOps()
{
	temp_double_array  doubleTime;
	temp_ptr_array ptrlist;
	int Hi=0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
		doubleTime[Hi]=(float) afieldOp->gettheOptime()->GetDateValue();
		ptrlist[i]=(void*) theOperations->ElementAtNumber(i);
		Hi++;
   }

   Hi--;
   // Below is the "Combsort" sorting algorithm, which is nearly as fast as
   // Quicksort, and does not require extra space on heap and stack, i.e. more
   // suited for this purpose than the implemented Quicksort (bmp).
	//Comsort(doubleTime, ptrlist, Hi);
   //Comsort replaced by quicksort because former left earliest operation as last operation in list
	quicksort2(doubleTime, ptrlist, 0, Hi);
   theOperations->ForgetAll();
   for (int counter=0;counter<=Hi;counter++)
   {
		theOperations->InsertLast((fieldOp *) ptrlist[counter]);
   }
//   for (int j=0; j<theOperations->NumOfNodes(); j++)
//   {
//	   fieldOp * afieldOp = theOperations->ElementAtNumber(j);
//	   cout << afieldOp->getopCode() << " " << *afieldOp->gettheOptime() << endl;
//   }
}

void cropSeriesClass::OutputOps(fstream * outfile, int NCU)
{
	int year=0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
      if (year!=afieldOp->gettheOptime()->GetYear())
      {
    	  if (year!=0)
    	  {
      	   	*outfile << endl;
            cout << endl;
    	  }
    	  *outfile << NCU << "\t" << seriesNumber << "\t";
    	  cout << NCU << "\t" << seriesNumber << "\t";
    	  year=afieldOp->gettheOptime()->GetYear();
    	  *outfile << year << "\t" << getNumOpsThisYear(year) << "\t";
    	  cout << year << "\t" << getNumOpsThisYear(year) << "\t";
      }
      afieldOp->outputOps(outfile, true, false);
	  *outfile << "\t";
      cout << "\t";
   }
};

int cropSeriesClass::getYear(string datestring)
{
	string instring(datestring);
	instring.resize(4);
	int year = atoi(instring.c_str());
	return year;
}
int cropSeriesClass::getMonth(string datestring)
{
	string instring(datestring);
	instring.erase(0,5);
	instring.resize(2);
	int month = atoi(instring.c_str());
	return month;
}
int cropSeriesClass::getDay(string datestring)
{
	string instring(datestring);
	instring.erase(0,8);
	int day = atoi(instring.c_str());
	return day;
}

int cropSeriesClass::getNumOpsThisYear(int year)
{
	int numOps = 0;
	for (int i=0; i<theOperations->NumOfNodes(); i++)
	{
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
      int theYear = afieldOp->gettheOptime()->GetYear();
      if (theYear==year)
    	  numOps++;
    }
	return numOps;
}

bool cropSeriesClass::OutputOpsTofnn(ofstream *fieldsfnn, int year)
{
	bool finished = false;
	if (theOperations->NumOfNodes()>0)
	{		
		int tillageOpNum = 0;
		int sowingOpNum = 0;
		int fertOpNum = 0;
		int harvOpNum = 0;
		int OpNumber = 0;
	    fieldOp * afieldOp = theOperations->ElementAtNumber(0);
	    int currentYear=afieldOp->gettheOptime()->GetYear();
	   while ((currentYear<=year+1)&& (OpNumber<theOperations->NumOfNodes()))
	   {
	      afieldOp = theOperations->ElementAtNumber(OpNumber);
	      currentYear=afieldOp->gettheOptime()->GetYear();
	      int month =afieldOp->gettheOptime()->GetMonth();
	      if (((currentYear==year)&&(month>=8))||((currentYear==year+1)&&(month<8)))
	      {
	    	  int opCode = afieldOp->getopCode();
	    	  switch (opCode)
	    	  {
	    	  case 1:
					 afieldOp->outputFnnOps(fieldsfnn, tillageOpNum);
					 tillageOpNum++;
					 break;
	    	  case 2:
					  afieldOp->outputFnnOps(fieldsfnn, sowingOpNum);
					  sowingOpNum++;
					  break;
	    	  case 3:
					  afieldOp->outputFnnOps(fieldsfnn, fertOpNum);
					  fertOpNum++;
					  break;
	    	  case 9:
					  afieldOp->outputFnnOps(fieldsfnn, harvOpNum);
					  harvOpNum++;
	    	  }
	      }
		  OpNumber++;
	   }
	   if (OpNumber==theOperations->NumOfNodes()-1)
		   finished = true;
	}
	return finished;
};


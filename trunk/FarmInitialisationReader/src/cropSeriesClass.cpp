

#include "cropSeriesClass.h"
#include "base\comsort.h"
#include "fieldOpSow.h"
#include "fieldOpHarv.h"
#include "fieldOpTill.h"
#include "fieldOpFert.h"
#include "base\message.h"


extern XMLNode XMLdescription;

cropSeriesClass::cropSeriesClass (int theseriesNumber )
{
	seriesNumber = theseriesNumber;
	theOperations= new linkList<fieldOp>;
	fieldArea=0.0;
	isGood=true;
};

cropSeriesClass:: ~ cropSeriesClass ( )
{
	delete theOperations;
};

bool cropSeriesClass::getcrops(XMLNode *fieldNode, int startYear, int endYear)
{
	bool retVal=true;
	int cropIDnumber;
	string *cropID;
	int numNodes = fieldNode->nChildNode("crop");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xCropNode=fieldNode->getChildNode("crop",i);
		if (!xCropNode.isEmpty())
		{
			XMLNode xNode;
			xNode=xCropNode.getChildNode("croptypeid");
			if (xNode.getText())
			{
				string temp(xNode.getText());
				cropID=getCropID(temp);
				xNode=xCropNode.getChildNode("cropsowplantdate");
				if (xNode.isEmpty())
				{
					if ((cropID->compare("G2"))||cropID->compare("G3"))
						cropID->assign("G1");
				}
				cropIDnumber =atoi(temp.c_str());
				cropDetails aCrop;
				aCrop.cropID=cropIDnumber;
				aCrop.cropName.assign(*cropID);
				theCrops.push_back(aCrop);
			}
			else
			{
				theMessage->WarningWithDisplay("Crop ID missing in field", seriesNumber);
				theMessage->WarningWithDisplay("Crop no ", i);
				retVal=false;
				isGood=false;
			}
			if (retVal)
			{
				xNode=xCropNode.getChildNode("cropsowplantdate");
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
						afieldOpSow->DefineOp ("Sowing", 2, year, month, day, *cropID);
						theOperations->InsertLast(afieldOpSow);
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
	}
	return retVal;
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


bool cropSeriesClass::gettillage(XMLNode *fieldNode,int startYear, int endYear)
{
	bool retVal=true;
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
						if (tillageType=="4")
							tillDepth=250.0;
						afieldOpTill->DefineOp ("Tillage", 1, year, month, day, tillDepth);
						theOperations->InsertLast(afieldOpTill);
					}
					else
					{
						retVal=false;
						theMessage->WarningWithDisplay("Tillage type missing in field", seriesNumber);
						isGood=false;
					}
				}
			}
		}
	}
	return retVal;
};

bool cropSeriesClass::getfertilisation(XMLNode *fieldNode,int startYear, int endYear)
{
	bool retVal=true;
	int numNodes = fieldNode->nChildNode("fertapp");
	for (int i=0;i<numNodes; i++)
	{
		XMLNode xFertNode=fieldNode->getChildNode("fertapp",i);
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
					{
						theMessage->WarningWithDisplay("Fertiliser amount missing in field", seriesNumber);
						retVal=false;
						isGood=false;
					}
					if (amount==0.0)
					{
						theMessage->WarningWithDisplay("Fertiliser amount is zero in field", seriesNumber);
						retVal=false;
						isGood=false;
					}
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
						{
							theMessage->WarningWithDisplay("Fertiliser application method id missing in field", seriesNumber);
							retVal=false;
							isGood=false;
						}
					}
					else
					{
						theMessage->WarningWithDisplay("Fertiliser type id missing in field", seriesNumber);
						retVal=false;
						isGood=false;
					}
				}
			}
		}
	}
	return retVal;
};

bool cropSeriesClass::getmanure(XMLNode *fieldNode, int startYear, int endYear)
{
	bool retVal=true;
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
					{
						isGood=false;
						theMessage->WarningWithDisplay("Manure amount missing in field", seriesNumber);
						retVal=false;
					}

					xNode=xManureNode.getChildNode("manuretypeid");
					float ammonium=0.0;
					float nitrate=0.0;
					float organicN=0.0;
					float carbon=0.0;
					if (xNode.getText())
					{
						string manureType(xNode.getText());
						//this code is unique to the reader - it allows manure types to be distinguished (solid v slurry v fertiliser)
						int theType = atoi(manureType.c_str());
						switch (theType)
						{
						case 1:
						case 2:
						case 3:
						case 4:
						case 11:
						case 12:
						case 13:
						case 18:
						case 19:
						case 20:
							theType*=100;
							break;
						case 5:
						case 6:
						case 7:
						case 8:
						case 9:
						case 10:
						case 14:
						case 15:
						case 16:
						case 17:
						case 21:
						case 22:
						case 23:
						case 24:
							theType*=1000;
							break;
						default:
							cout << "Manure type " << theType << endl;
							theMessage->FatalError("Unknown manure in cropSeriesClass::getmanure");

						}
						//if this is solid manure, multiply by 10
						if ((theType==500)||(theType==600)||(theType==700)||(theType==900))
							theType*=10;
						double manureTotalNConc=0.0;//g/kg fresh weight
						xNode=xManureNode.getChildNode("manuretypencontent");
						if (xNode.getText())
						{
							string manureTotalNStr(xNode.getText());
							manureTotalNConc= atof(manureTotalNStr.c_str());
						}
						else
						{
							/*isGood=false;
							theMessage->WarningWithDisplay("Manure total nitrogen content missing in field", seriesNumber);
							retVal=false;*/
							manureTotalNConc=3.0;
						}
						double manureTANConc=0.0;//g/kg fresh weight
						xNode=xManureNode.getChildNode("manuretypenh4n_content");
						if (xNode.getText())
						{
							string manureTANStr(xNode.getText());
							manureTANConc= atof(manureTANStr.c_str());
						}
						else
						{
							/*isGood=false;
							theMessage->WarningWithDisplay("Manure ammonium nitrogen content missing in field", seriesNumber);
							retVal=false;*/
							manureTANConc=1.5;
						}
						ammonium=amount * manureTANConc;
						organicN=amount*(manureTotalNConc-manureTANConc);
						carbon=12.0*organicN;
						xNode=xManureNode.getChildNode("manureappmethodid");
						int method = 0;
						if (xNode.getText())
						{
							string fertMeth(xNode.getText());
							method= atoi(fertMeth.c_str());
							afieldOpFert->DefineOp ("Fertilisation", 3, year, month, day, theType, ammonium, nitrate, organicN,
									carbon,method);
							theOperations->InsertLast(afieldOpFert);
						}
						else
						{
							isGood=false;
							theMessage->WarningWithDisplay("Manure application method id missing in field", seriesNumber);
							retVal=false;
						}
					}
					else
					{
						theMessage->WarningWithDisplay("Manure type id missing in field", seriesNumber);
						isGood=false;
						retVal=false;
					}
				}
			}
		}
	}
	return retVal;
};

bool cropSeriesClass::addOperations(XMLNode *fieldNode,int startYear, int endYear)
{
	bool retVal=true;
	retVal=getcrops(fieldNode,startYear, endYear);
		retVal=gettillage(fieldNode,startYear, endYear);
		retVal=getfertilisation(fieldNode,startYear, endYear);
		retVal=getmanure(fieldNode,startYear, endYear);
		sortOps();
	return retVal;
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

void cropSeriesClass::OutputOps(fstream * outfile, int farmID)
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
				if (verbosity)
					cout << endl;
			}
		}
		*outfile << farmID << "\t" << seriesNumber << "\t" << "\t";
		cout << farmID << "\t" << seriesNumber << "\t" << "\t";
		afieldOp->outputOps(outfile, true, true);
		*outfile << endl;
		cout << endl;
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

#ifdef __BCplusplus__
	string instring;
	instring =datestring.substr(5,2);
	int month = atoi(instring.c_str());
#else
	string instring(datestring);
	instring.erase(0,5);
	instring.resize(2);
	int month = atoi(instring.c_str());
#endif
	return month;
}
int cropSeriesClass::getDay(string datestring)
{
#ifdef __BCplusplus__
	string instring;
	instring =datestring.substr(8,2);
#else
	string instring(datestring);
	instring.erase(0,8);
#endif
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
	bool retVal = true; //currently is always true
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
//		if (OpNumber==theOperations->NumOfNodes()-1)
	//	   finished = true;
	if (!retVal)
		cout << "here" << endl;
	}
	return retVal;
};

void cropSeriesClass::checkOps()
{
	bool gotSow = false;
	for (int i=0; i<theOperations->NumOfNodes(); i++)
	{
		fieldOp * afieldOp = theOperations->ElementAtNumber(i);
		int opCode = afieldOp->getopCode();
		if (opCode==2)
			gotSow=true;
		if (opCode==3)	//have harvested, so reset sowing
			gotSow = false;
		if ((opCode==1)&& (gotSow==true))
			theMessage->FatalError("Tillage before crop has been harvested");
	}
}


int cropSeriesClass::getCropIDNumber(int index)
{
	return theCrops.at(index).cropID;
}

string cropSeriesClass::getCropName(int index)
{
	string aVal;
	if (index<theCrops.size())
		aVal.assign(theCrops.at(index).cropName);
	else
		aVal.assign("CROP_MISSING");
	return aVal;
}


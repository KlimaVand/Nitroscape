

#include "cropSeriesClass.h"
#include "base/message.h"
#include "base/nixcbits.h"

extern int verbosityLevel;
#include "fieldOp.h"
#include "fieldOpTill.h"
#include "fieldOpSow.h"
#include "fieldOpFert.h"
#include "fieldOpHarv.h"
#include "fieldOpGrass.h"
#include "fieldOpCut.h"
#include "base/unix_util.h"


cropSeriesClass::cropSeriesClass (int theseriesNumber )
{
	seriesNumber = theseriesNumber;
	theOperations= new linkList<fieldOp>;
};

cropSeriesClass:: ~ cropSeriesClass ( )
{
	delete theOperations;
};

//! Sorts the field operations in order of starting date
void cropSeriesClass::sortOps()
{
	temp_double_array  doubleTime;
	temp_ptr_array ptrlist;
	int numPtrs=0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
	   fieldOp * afieldOp = theOperations->ElementAtNumber(i);
	   doubleTime[i]=(double) afieldOp->gettheOptime()->GetDateValue();
	   ptrlist[i]=(void*) theOperations->ElementAtNumber(i);
	   numPtrs++;
   }

   // Below is the "Combsort" sorting algorithm, which is nearly as fast as
   // Quicksort, and does not require extra space on heap and stack, i.e. more
   // suited for this purpose than the implemented Quicksort (bmp).
	Comsort(doubleTime, ptrlist, numPtrs);
   theOperations->ForgetAll();
   for (int counter=0;counter<numPtrs;counter++)
   {
		theOperations->InsertLast((fieldOp *) ptrlist[counter]);
   }
}

void cropSeriesClass::OutputOps(fstream * outfile, int NCU)
{
	int year = 0;
   bool first = true;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
      int OpYear = afieldOp->gettheOptime()->GetYear();
      if (OpYear!=year)
      {
      	if (first)
         	first = false;
         else
			*outfile << endl;
		*outfile << NCU << "\t" << seriesNumber << "\t" << OpYear << "\t" << GetNumOutputOpsInYear(OpYear) << "\t";
         year = OpYear;
      }
      if (verbosityLevel==1)
         cout << OpYear << "\t";
      afieldOp->outputOps(outfile, true, false);
      if (verbosityLevel==1)
         cout << endl;
   }
   if (theOperations->NumOfNodes()>0)
	   *outfile << endl;
};

int cropSeriesClass::GetNumOutputOpsInYear(int targetYear)
{
	int numOps = 0;
   bool gotit=false;
   int i=0;

   int OpYear = 0;
   linkList<fieldOp>::PS p;
   theOperations->PeekHead(p);
   while ((gotit==false)&&(p!=NULL))
   {

      OpYear = p->element->gettheOptime()->GetYear();
      if (OpYear==targetYear)
			gotit=true;
      else
			i++;
      theOperations->OneStep(p);
   }
   fieldOp * afieldOp;
   while ((OpYear==targetYear)&&(i<theOperations->NumOfNodes()))
   {
      afieldOp = theOperations->ElementAtNumber(i);
      OpYear = afieldOp->gettheOptime()->GetYear();
      if (OpYear==targetYear)
      {
      	numOps++;
			i++;
      }
   }
   return numOps;
};

double cropSeriesClass::GetNOutput()
{
	double retVal = 0.0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
	  fieldOp * afieldOp = theOperations->ElementAtNumber(i);
	  retVal+=afieldOp->GetNApplied();
   }
   return retVal;
}

bool cropSeriesClass::OutputOpsTofnn(ofstream *fieldsfnn, bool first, int year)
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
	    	  case 9: if (!first)
	    	  	  	  {
	    		  	  	  afieldOp->outputFnnOps(fieldsfnn, harvOpNum);
	    		  	  	  harvOpNum++;
	    	  	  	  }
	    	  }
	      }
		  OpNumber++;
	   }
	   if (OpNumber==theOperations->NumOfNodes()-1)
		   finished = true;
	}
	return finished;
};


bool cropSeriesClass::ReadFieldOps(ifstream *opFile)
{
	bool finished = false;  //only here because with the Borland compiler, is_open does not want to return false at end of file
	int numOps =0;
	int year =0;
	*opFile >> year >> numOps;
	if (opFile->is_open())
	{
		cout << year  << endl;
		if (year==0)
			finished=true;
		if (year==2030)
			cout << "here" << endl;
		int i=0;
		while((i< numOps)&& !finished)
		{
			int julianDay=0;
			int opCode=0;
			fieldOp * afieldOp;
			*opFile >> julianDay >> opCode;
			//! On basis of operation code (opCode), initialise an instance of the appropriate field operation class
			switch (opCode)
			{
			case 1: //tillage
				afieldOp = new fieldOpTill();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			case 2://sowing
				afieldOp = new fieldOpSow();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			case 3://fertilisation and manuring
				afieldOp = new fieldOpFert();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			case 4://cutting
				afieldOp = new fieldOpCut();
				//afieldOp = new fieldOpGrass();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			case 5://start grazing
				afieldOp = new fieldOpGrass();
				afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
				break;
			case 6://stop grazing
				afieldOp = new fieldOpGrass();
				afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
				break;
			case 9://harvesting
				afieldOp = new fieldOpHarv();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			case 10://atmospheric N deposition
				afieldOp = new fieldOpFert();
				afieldOp->ReadOpResults(opFile,year,julianDay);
				break;
			default:
				char buf[2];
				itoa(opCode,buf,10);
				theMessage->FatalError("Incorrect or unprogrammed operation code ", buf);
				break;
			}
			theOperations->InsertLast(afieldOp);
			cout << "Op no " << i << " Op name " << afieldOp->getname() << " Op code " << afieldOp->getopCode() << " Day " << julianDay << endl;
			i++;
		}
	}
	else
		finished=true;
	return finished;
}

//! Get year of first field operation
int cropSeriesClass::getFirstYear()
{
	int ret_val=0;
	linkList<fieldOp>::PS P;
	theOperations->PeekHead(P);
	fieldOp *afieldOp = P->element;
	ret_val=afieldOp->gettheOptime()->GetYear();
	return ret_val;
}
//! Get year of last field operation
int cropSeriesClass::getLastYear()
{
	int ret_val=0;
	linkList<fieldOp>::PS P;
	theOperations->PeekTail(P);
	fieldOp *afieldOp = P->element;
	ret_val=afieldOp->gettheOptime()->GetYear();
	return ret_val;
}



#include "cropSeriesClass.h"
#include "base\message.h"
#include "base\nixcbits.h"

extern bool verbosity;

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
	int Hi=0;
   for (int i=0; i<theOperations->NumOfNodes(); i++)
   {
      fieldOp * afieldOp = theOperations->ElementAtNumber(i);
		doubleTime[Hi]=(double) afieldOp->gettheOptime()->GetDateValue();
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
      if (verbosity)
         cout << OpYear << "\t";
      afieldOp->outputOps(outfile, true, false);
      if (verbosity)
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
   fieldOp * afieldOp;
   int OpYear = 0;
   while ((gotit==false)&&(i<theOperations->NumOfNodes()))
   {
      afieldOp = theOperations->ElementAtNumber(i);
      OpYear = afieldOp->gettheOptime()->GetYear();
      if (OpYear==targetYear)
			gotit=true;
      else
			i++;
   }
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

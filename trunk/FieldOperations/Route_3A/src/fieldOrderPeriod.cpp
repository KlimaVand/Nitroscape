
 
// Class: fieldOrderPeriod

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "fieldOrderPeriod.h"    				

fieldOrderPeriod:: ~ fieldOrderPeriod ( )
{
  if (StartDate)
  	delete StartDate;
  if (EndDate)
  	delete EndDate;
};

//Copy constructor alternative to compiler provided default copy constructor
fieldOrderPeriod::fieldOrderPeriod (const fieldOrderPeriod& afieldOrderPeriod )
  : base (afieldOrderPeriod)  
{
 StartDate = afieldOrderPeriod.StartDate;
 EndDate = afieldOrderPeriod.EndDate;
 TSum = afieldOrderPeriod.TSum; 
  
} 

//  Function: 	fieldOrderPeriod
//  Purpose: 	
//  Parameters: const char * theName, const int anIndex=-1, const base * theBase=NULL
 fieldOrderPeriod::fieldOrderPeriod(const char * theName, const int anIndex, const base * theBase)
 : base(theName, anIndex, theBase)
{
   TSum = -99.0;
}
//  Function: 	ReadPeriod
//  Purpose: 	
//  Parameters:  fstream* aFile
void fieldOrderPeriod::ReadPeriod( fstream* aFile)  
{
	Setfile(aFile);
	char variableString[MAX_TXT];
   char * tagString="StartDate";
   string * s = new string;     //the 's' strings are copied to date strings in this class and are deleted when the instance is deleted
   strcpy(variableString,GetName());
   strcat(variableString,tagString);
   SetCritical();
   if (GetParameter(variableString,s,Index))
   {
      if (s->length()==0)
     	 theMessage->FatalError("Format for start date incorrect");
      StartDate=s;
      strcpy(variableString,GetName());
      strcat(variableString,"EndDate");
	   s = new string;
      GetParameter(variableString,s,Index);
      if (s->length()==0)
     	 theMessage->FatalError("Format for end date incorrect");
      EndDate=s;
      strcpy(variableString,GetName());
      strcat(variableString,"TSum");
      GetParameter(variableString,&TSum,Index);
   }
//   delete s;
	Setfile(NULL);
}
//  Function: 	GetYear
//  Purpose: 	
//  Parameters:  string* date
int fieldOrderPeriod::GetYear( string* date)  
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetYear();
}
//  Function: 	GetMonth
//  Purpose: 	
//  Parameters:  string* date
int fieldOrderPeriod::GetMonth( string* date)  
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetMonth();
}
//  Function: 	GetDay
//  Purpose: 	
//  Parameters:  string* date
int fieldOrderPeriod::GetDay( string* date)  
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetDay();
}

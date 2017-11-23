
 
// Class: modelControlClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "modelControlClass.h"   
#include "trafworkDataClass.h" 				

modelControlClass:: ~ modelControlClass ( )
{

};

/*! Initialise model runs
\param outputDetail set to true if the model variables our to be written to file
\param startDay day on which to start simulations
\param startMonth month on which to start simulations
\param startYear year on which to start simulations
\param endDay day on which to end simulations
\param endMonth month on which to end simulations
\param endYear year on which to end simulations
*/

void modelControlClass::Initialise(bool outputDetail, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear)  
{
 outputFile = new fstream();
 outputFile->open("results.txt", ios::out);
 *outputFile << "NCU" << "\t" "fieldNo" << "\t" << "Year" << "\t" << "DayNo" << "\t" << "OpNum" << "\t" << "Status" << "\t" << "Date" << "\t" << "Operation" << "\t" << "OpTsum" << "\t" << "CurrTsum" << endl;
 startTime = new bsTime(startDay,startMonth,startYear);
 theTime=*startTime;;
 endTime = new bsTime(endDay,endMonth,endYear);
 endTime->AddOneDay();
 //prepare output file for detailed results
 if (outputDetail)
 {
   char buffer[250];
   getcwd(buffer, 250);
   cout << buffer;
   outputDetailsFile = new fstream();
 	outputDetailsFile->open("details.txt", ios::out);
 }
 //prepare log file for warnings
 theMessage                   = new message;
 theMessage->InitMessage("warnings.txt",NULL,NULL,NULL,NULL);

 // get the data on trafficability and workability
 atrafworkCharClass = new trafworkCharClass();
 atrafworkCharClass->Initialise("trafwork.txt");

 //initialise the location instance
 aLocation.Initialise(0,"DK.txt",outputFile, atrafworkCharClass);

 //if detailed output is required, write the column header information to file
 if (outputDetail)
 {
	*outputDetailsFile << "dayno" << "\t";
   aLocation.outputDetails(outputDetailsFile,true);
   *outputDetailsFile << endl;
 }
}
//! Conducts the simulations
void modelControlClass::Simulate()
{
 //repeat the daily time steps until the final date is reached
 int i=0;
 while (theTime<*endTime)
 {
 	theTime.AddOneDay();
   cout << i << endl;
   if (i==543)
   cout << endl;
	 aLocation.dailyRoutine();
    if (outputDetailsFile)
    {
      *outputDetailsFile << i << "\t";
    	aLocation.outputDetails(outputDetailsFile, false);
      *outputDetailsFile << endl;
    }
    i++;
 }
}
//! Tidy up
void modelControlClass::closeDown()  
{
 //close all files and cleanup memory
 if (outputDetailsFile)
   outputDetailsFile->close();
 outputFile->close();
 delete outputFile;
 delete outputDetailsFile;
 delete atrafworkCharClass;
 aLocation.closedown();
 delete startTime;
 delete endTime;
}

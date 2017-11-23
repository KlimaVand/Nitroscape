
 
// Class: trafworkCharClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "trafworkCharClass.h"   
#include "cropCharClass.h" 				

trafworkCharClass:: ~ trafworkCharClass ( )
{
	delete theTrafWorkData;
};

//!Open data file, read data and store
/*! 
\param theFilename name of data file
*/
void trafworkCharClass::Initialise(char * theFilename)  
{
	theTrafWorkData = new linkList<trafworkDataClass>;
	OpenInputFile(theFilename);
   //note that the data for each soil type should end with the name of the soil type
   //the end of file marker should be at the end of the line containing the data for the last soil type
   char buffer[200];
   int endit=1;
   ReadLine(file,buffer,200);
   while ((!file->eof()) && (endit>0))
   {
   	trafworkDataClass *atrafworkDataClass = new trafworkDataClass();
      endit = atrafworkDataClass->ReadParameters(file);   //endit will be nonzero until the end of the file
      theTrafWorkData->InsertLast(atrafworkDataClass);
   }
   CloseInputFile();
}
//!
/*! 
\param soilNumber Number of soil for which data is required
*/
trafworkDataClass * trafworkCharClass::GetSoilData(int soilNumber)  
{
  linkList<trafworkDataClass>::PS P;
  theTrafWorkData->SearchByNumber(P,soilNumber-1);
  return P->element;
}

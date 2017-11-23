
 
// Class: soilCharClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "soilCharClass.h"   
#include "layerClass.h" 				

soilCharClass:: ~ soilCharClass ( )
{
	delete thesoilData;
};

//! Opens input file and reads data
void soilCharClass::Initialise(char * theFilename)  
{
	thesoilData = new linkList<soilDataClass>;
   //Important! Each line of data must end with the name of the soil type and the end of file marker
   //must be immediately after the name of the last soil type
	OpenInputFile(theFilename);
   char buffer[200];
   int endit=1;
   ReadLine(file,buffer,200);
   while ((!file->eof()) && (endit>0))
   {
   	soilDataClass *soilData = new soilDataClass();
      soilData->Initialise();
      endit = soilData->ReadParameters(file);   //endit will be zero when there is no more input data
      thesoilData->InsertLast(soilData);
   }
   CloseInputFile();
}
//! Returns the soil characteristics for a given soil type
/*!
\param soilNumber number of soil for which parameters are required
*/
soilDataClass * soilCharClass::GetSoilData(int soilNumber)  
{
  linkList<soilDataClass>::PS P;
  thesoilData->SearchByNumber(P,soilNumber-1);
  return P->element;
}

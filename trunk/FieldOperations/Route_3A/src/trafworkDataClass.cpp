
 
// Class: trafworkDataClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "trafworkDataClass.h"   
#include "cropCharClass.h" 				

trafworkDataClass:: ~ trafworkDataClass ( )
{

};

// constructor
trafworkDataClass::trafworkDataClass(const trafworkDataClass& atrafworkDataClass)  
{
   trafficability_grass=atrafworkDataClass.trafficability_grass;
   workability_grass=atrafworkDataClass.workability_grass;
   trafficability_arable=atrafworkDataClass.trafficability_arable;
   workability_arable=atrafworkDataClass.workability_arable;
}

//!Reads the trafficability and workability limits for an individual soil
/*! 
\param afile pointer to the file from which data should be read
*/
int trafworkDataClass::ReadParameters(fstream * afile)  
{
   int dumint;
   char buffer[200];
   *afile >> dumint >> trafficability_grass >> workability_grass >> trafficability_arable >>  workability_arable;
   trafficability_grass/=1500;
   workability_grass/=1500;
   trafficability_arable/=1500;
   workability_arable/=1500;
	*afile >> buffer;
	return strlen(buffer);
}

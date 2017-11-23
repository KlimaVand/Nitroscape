
 
// Class: soilDataClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "soilDataClass.h"   
#include "fieldOp.h" 				

soilDataClass:: ~ soilDataClass ( )
{
	delete theLayers;
};

// constructor
soilDataClass::soilDataClass(const soilDataClass& asoilDataClass)  
{
 maxRootingDepth = asoilDataClass.maxRootingDepth;
 drainageConst = asoilDataClass.drainageConst;
 theLayers = new linkList<layerClass>;
 for ( int i = 0; i < asoilDataClass.theLayers->NumOfNodes(); i++)
 {
 	layerClass *aLayerClass= new layerClass(*asoilDataClass.theLayers->ElementAtNumber(i)) ;
   theLayers->InsertLast(aLayerClass);
 }
}

//!Read the parameters for single soil type
/*! 
\param afile pointer to the input file
*/
int soilDataClass::ReadParameters(fstream * afile)  
{
   int dumint;
   double z_upper = 0.0;
   char buffer[200];
   *afile >> dumint >> maxRootingDepth;
   for(int index=0;index<4;index++)
   {
      layerClass * alayerClass = new layerClass();
      alayerClass->Initialise();
      if (index==0)
	   	z_upper = 0.0;
      else
	   	z_upper = theLayers->ElementAtNumber(index-1)->getz_lower();  //lower boundary of one layer is the upper boundary of the next
      alayerClass->ReadParameters(afile,z_upper);
      theLayers->InsertLast(alayerClass);
   }
	*afile >> drainageConst >> buffer;
	return strlen(buffer);
}
//! Initialises an instance of this class
void soilDataClass::Initialise()  
{
	theLayers = new linkList<layerClass>;
}
//!Returns the parameters for a single layer
/*! 
\param layerNo number of the layer for which the parameters are required
*/
layerClass * soilDataClass::GetLayerDetails(int layerNo)  
{
	return theLayers->ElementAtNumber(layerNo);
}
//! Tidy up this class
void soilDataClass::closeDown()  
{
	delete theLayers;
}

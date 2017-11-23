
 
// Class: layerClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "layerClass.h"   
#include "soilDataClass.h" 				

layerClass:: ~ layerClass ( )
{

};

// constructor
layerClass::layerClass(const layerClass& alayerClass)  
{
 z_lower = alayerClass.z_lower;
 fieldCapacity = alayerClass.fieldCapacity;
 thickness = alayerClass.thickness;
}

//!Read the parameters associated with a single layer
/*! 
\param afile pointer to the input file
\param z_upper depth below the soil surface of the upper boundary of the layer
*/
void layerClass::ReadParameters(fstream * afile, double z_upper)  
{
   *afile >> z_lower >> fieldCapacity;
   thickness = z_lower - z_upper;
   //the field capacity is expressed as 100 * cubic metre/cubic metre
   fieldCapacity/=100.0;
}
//! Initialise an instance of this class
void layerClass::Initialise()  
{ z_lower = 0.0;
 fieldCapacity = 0.0;
}

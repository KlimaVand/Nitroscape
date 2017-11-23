
 
// Class: cropCharClass

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "cropCharClass.h"   
#include "soilDataClass.h" 				

cropCharClass:: ~ cropCharClass ( )
{

};

//! Read the characteristics for all crops
void cropCharClass::ReadParameters()  
{
	SetCritical();
	GetParameter("CROP",&Name);
	GetParameter("croot",&croot);
	GetParameter("Lmax",&Lmax);
   //remaining parameters may or may not be defined, depending upon the crop
   UnsetCritical();
	GetParameter("So",&So);
	GetParameter("Sf",&Sf);
	GetParameter("Sr",&Sr);
	GetParameter("Sm",&Sm);
	GetParameter("Soe",&Soe);
	GetParameter("Sfe",&Sfe);
	GetParameter("Lwinter",&Lwinter);
	GetParameter("Lymax",&Lymax);
	GetParameter("Lmin",&Lmin);
	GetParameter("rdWinter",&rootDepthWinter);
   attenuationCoeff = 0.6;
}
//!Initialise all parameters then read the data for a specific crop
/*! 
\param aFile pointer to the input file
\param index number of the crop in which parameters are required
*/
void cropCharClass::Initialise(fstream * aFile, int index)  
{
   Name = "";
   So = 0.0;
   Sf = 0.0;
   Sr = 0.0;
   Sm = 0.0;
   Soe = 0.0;
   Sfe = 0.0;
   Lmax = 0.0;
   Lwinter = 0.0;
   Lymax = 0.0;
   Lmin = 0.0;
   attenuationCoeff = 0.0;
   croot = 0.0;
   rootDepthWinter = 0.0;
	Setfile(aFile);
   Index = index;
   SetCritical();
   FindSection("Crop",Index);
   UnsetCritical();
   ReadParameters();
	Setfile(0);
}

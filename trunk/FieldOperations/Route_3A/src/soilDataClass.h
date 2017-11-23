// Class: soilDataClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __SOILDATACLASS_H  //Required for current class
  #define __SOILDATACLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for 1:M aggregation (part) classes
#ifndef __LAYERCLASS_H
  #include "layerClass.h"
#endif

//! Contained the parameters for a single soil type

class soilDataClass      
{                       
			
  //! Maximum rooting depth of the soil (millimetres)		
     double maxRootingDepth; //Attribute data member
   //! Rate at which water in excess of field capacity drains from any given layer		
     double drainageConst; //Attribute data member        

  //! List of parameter values for the layers in the soil		  
   linkList<layerClass> *theLayers; //1:M aggregation part data member 

private:

protected:

public:

			//Default constructor
  soilDataClass () {};

 //! Copy constructor with arguments
 /*! 
 \param asoilDataClass pointer to the instance that should be copied
 */
// constructor
soilDataClass::soilDataClass(const soilDataClass& asoilDataClass)  ;

//!Read the parameters for single soil type
/*! 
\param afile pointer to the input file
*/					
   int ReadParameters (fstream * afile)    ;

//! Initialises an instance of this class					
   void Initialise ()    ;

//!Returns the parameters for a single layer
/*! 
\param layerNo number of the layer for which the parameters are required
*/					
   layerClass * GetLayerDetails (int layerNo)    ;

//! Tidy up this class					
   void closeDown ()    ;
 
   ~ soilDataClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getmaxRootingDepth() const  { return maxRootingDepth;}

                       //!Get accessor function for non-static attribute data member
  double getdrainageConst() const  { return drainageConst;}
                       
                       //!Set accessor function for non-static attribute data member
  void setmaxRootingDepth (double amaxRootingDepth)  { maxRootingDepth = amaxRootingDepth;}

                       //!Set accessor function for non-static attribute data member
  void setdrainageConst (double adrainageConst)  { drainageConst = adrainageConst;}
       
                       //!Get accessor function for 1:many aggregation object data member
  linkList <layerClass>* gettheLayers()  { return theLayers;  }
                       
};
			
#endif

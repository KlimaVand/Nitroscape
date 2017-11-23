// Class: layerClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __LAYERCLASS_H  //Required for current class
  #define __LAYERCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

//! Describes a single layer in the soil

class layerClass      
{                       
			
private:

protected:

//! Depth below the soil surface of the lower boundary of the layer (millimetres)
  double z_lower;
//! Plant-available water content of the layer at field capacity
  double fieldCapacity;
//! Thickness of the layer (millimetres)
  double thickness;  

public:

			//!Default constructor
  layerClass () {};

 //! Copy constructor for this class
 /*!
 \param alayerClass pointer to the instance that should be copied
 */
// constructor
layerClass::layerClass(const layerClass& alayerClass)  ;

//!Read the parameters associated with a single layer
/*! 
\param afile pointer to the input file
\param z_upper depth below the soil surface of the upper boundary of the layer
*/					
   void ReadParameters (fstream * afile, double z_upper)    ;

//! Initialise an instance of this class					
   void Initialise ()    ;
//!Destructor 
   ~ layerClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getz_lower() const  { return z_lower;}

                       //!Get accessor function for non-static attribute data member
  double getfieldCapacity() const  { return fieldCapacity;}

                       //!Get accessor function for non-static attribute data member
  double getthickness() const  { return thickness;}
                       
                       //!Set accessor function for non-static attribute data member
  void setz_lower (double az_lower)  { z_lower = az_lower;}

                       //!Set accessor function for non-static attribute data member
  void setfieldCapacity (double afieldCapacity)  { fieldCapacity = afieldCapacity;}

                       //!Set accessor function for non-static attribute data member
  void setthickness (double athickness)  { thickness = athickness;}
       
};
			
#endif

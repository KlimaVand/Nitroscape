// Class: trafworkDataClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __TRAFWORKDATACLASS_H  //Required for current class
  #define __TRAFWORKDATACLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

//! Contains the trafficability and workability parameters for a single soil type

class trafworkDataClass      
{                       
			
  //! Trafficability limit for grass fields (millimetres soil moisture deficit)		
     double trafficability_grass; //Attribute data member
   //! Workability limit for grassland (millimetres soil moisture deficit)		
     double workability_grass; //Attribute data member
   //! Trafficability limit of arable fields (millimetres soil moisture deficit)		
     double trafficability_arable; //Attribute data member
   //! Workability limits of arable fields (millimetres soil moisture deficit)		
     double workability_arable; //Attribute data member        

private:

protected:

public:

			//!Default constructor
  trafworkDataClass () {};

 /*! Copy constructor
 \param atrafworkDataClass name of class to be copied
 */
// constructor
trafworkDataClass::trafworkDataClass(const trafworkDataClass& atrafworkDataClass)  ;

//!Reads the trafficability and workability limits for an individual soil
/*! 
\param afile pointer to the file from which data should be read
*/					
   int ReadParameters (fstream * afile)    ;
//!Destructor 
   ~ trafworkDataClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double gettrafficability_grass() const  { return trafficability_grass;}

                       //!Get accessor function for non-static attribute data member
  double getworkability_grass() const  { return workability_grass;}

                       //!Get accessor function for non-static attribute data member
  double gettrafficability_arable() const  { return trafficability_arable;}

                       //!Get accessor function for non-static attribute data member
  double getworkability_arable() const  { return workability_arable;}
                       
                       //!Set accessor function for non-static attribute data member
  void settrafficability_grass (double atrafficability_grass)  { trafficability_grass = atrafficability_grass;}

                       //!Set accessor function for non-static attribute data member
  void setworkability_grass (double aworkability_grass)  { workability_grass = aworkability_grass;}

                       //!Set accessor function for non-static attribute data member
  void settrafficability_arable (double atrafficability_arable)  { trafficability_arable = atrafficability_arable;}

                       //!Set accessor function for non-static attribute data member
  void setworkability_arable (double aworkability_arable)  { workability_arable = aworkability_arable;}
       
};
			
#endif

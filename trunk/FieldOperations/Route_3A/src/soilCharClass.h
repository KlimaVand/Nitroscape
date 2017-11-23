// Class: soilCharClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __SOILCHARCLASS_H  //Required for current class
  #define __SOILCHARCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
                               //Required for 1:M aggregation (part) classes
#ifndef __SOILDATACLASS_H
  #include "soilDataClass.h"
#endif

//! Holds all the parameters for all soil types

class soilCharClass  : public base        
{                       
			
  //! Name of the file containing the parameters for all soil types		
     char * filename; //Attribute data member        

  //!Pointer to a list of instances containing the soil parameters for each soil type		  
   linkList<soilDataClass> *thesoilData; //1:M aggregation part data member 

private:

protected:

public:

			//Default constructor
  soilCharClass () {};

//! Opens input file and reads data					
   void Initialise (char * theFilename)    ;

//! Returns the soil characteristics for a given soil type
/*!
\param soilNumber number of soil for which parameters are required
*/					
   soilDataClass * GetSoilData (int soilNumber)    ;
//!Destructor 
   ~ soilCharClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  char * getfilename() const  { return filename;}
                       
                       //!Set accessor function for non-static attribute data member
  void setfilename (char * afilename)  { filename = afilename;}
       
                       //!Get accessor function for 1:many aggregation object data member
  linkList <soilDataClass>* getthesoilData()  { return thesoilData;  }
                       
};
			
#endif

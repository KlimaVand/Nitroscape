// Class: trafworkCharClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __TRAFWORKCHARCLASS_H  //Required for current class
  #define __TRAFWORKCHARCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
                               //Required for 1:M aggregation (part) classes
#ifndef __TRAFWORKDATACLASS_H
  #include "trafworkDataClass.h"
#endif

//! Stores details of trafficability and workability soil moisture limits for all soils

class trafworkCharClass  : public base        
{                       
			
  //! Name of file containing the workability and trafficability data		
     char * filename; //Attribute data member        

  //! Pointer to a list of trafficability and workability parameters for all soil types		  
   linkList<trafworkDataClass> *theTrafWorkData; //1:M aggregation part data member 

private:

protected:

public:

			//!Default constructor
  trafworkCharClass () {};

//!Open data file, read data and store
/*! 
\param theFilename name of data file
*/					
   void Initialise (char * theFilename)    ;

//!
/*! 
\param soilNumber Number of soil for which data is required
*/					
   trafworkDataClass * GetSoilData (int soilNumber)    ;
 
   ~ trafworkCharClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  char * getfilename() const  { return filename;}
                       
                       //!Set accessor function for non-static attribute data member
  void setfilename (char * afilename)  { filename = afilename;}
       
                       //!Get accessor function for 1:many aggregation object data member
  linkList <trafworkDataClass>* gettheTrafWorkData()  { return theTrafWorkData;  }
                       
};
			
#endif

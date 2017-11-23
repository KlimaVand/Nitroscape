// Class: modelControlClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __MODELCONTROLCLASS_H  //Required for current class
  #define __MODELCONTROLCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
                               //Required for 1:1 aggregation (part) classes
#ifndef __LOCATIONCLASS_H
  #include "locationClass.h"
#endif
                                                               
//!Controls the model setup and simulations

class modelControlClass  : public base        
{                       
			
  //! The date at the start of simulations		
     bsTime * startTime; //Attribute data member
   //! The date on which to end simulations		
     bsTime * endTime; //Attribute data member
   //! Pointer to an optional file for detailed output of variables		
     fstream * outputDetailsFile; //Attribute data member
   //! Pointer to the instance of the class containing workability and trafficability parameters		
     trafworkCharClass * atrafworkCharClass; //Attribute data member
   //! Pointer to the output file		
     fstream * outputFile; //Attribute data member        
  //!instance of a class describing a single location (NCU)
  locationClass aLocation; //1:1 aggregation part data member        

private:

protected:

public:

			//Default constructor
  modelControlClass () {};

/*! Initialise model runs
\param outputDetail set to true if the model variables our to be written to file
\param startDay day on which to start simulations
\param startMonth month on which to start simulations
\param startYear year on which to start simulations
\param endDay day on which to end simulations
\param endMonth month on which to end simulations
\param endYear year on which to end simulations
*/
					
   void Initialise (bool outputDetail, int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear)    ;

//! Conducts the simulations					
   void Simulate ()    ;

//! Tidy up					
   void closeDown ()    ;
 
   ~ modelControlClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  bsTime * getstartTime() const  { return startTime;}

                       //!Get accessor function for non-static attribute data member
  bsTime * getendTime() const  { return endTime;}

                       //!Get accessor function for non-static attribute data member
  fstream * getoutputDetailsFile() const  { return outputDetailsFile;}

                       //!Get accessor function for non-static attribute data member
  trafworkCharClass * getatrafworkCharClass() const  { return atrafworkCharClass;}

                       //!Get accessor function for non-static attribute data member
  fstream * getoutputFile() const  { return outputFile;}
                       
                       //!Set accessor function for non-static attribute data member
  void setstartTime (bsTime * astartTime)  { startTime = astartTime;}

                       //!Set accessor function for non-static attribute data member
  void setendTime (bsTime * aendTime)  { endTime = aendTime;}

                       //!Set accessor function for non-static attribute data member
  void setoutputDetailsFile (fstream * aoutputDetailsFile)  { outputDetailsFile = aoutputDetailsFile;}

                       //!Set accessor function for non-static attribute data member
  void setatrafworkCharClass (trafworkCharClass * aatrafworkCharClass)  { atrafworkCharClass = aatrafworkCharClass;}

                       //!Set accessor function for non-static attribute data member
  void setoutputFile (fstream * aoutputFile)  { outputFile = aoutputFile;}
       
                       //!Get accessor function for 1:1 aggregation part data member
  locationClass& getaLocation()   { return aLocation;}
                       
                       //!Set accessor function for 1:1 aggregation part data member
  void setaLocation (locationClass& aaLocation)  { aLocation = aaLocation;  }

};
			
#endif

// Class: climateClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __CLIMATECLASS_H  //Required for current class
  #define __CLIMATECLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

class climateClass      
{                       
			
private:

protected:

public:

			//Default constructor
  climateClass () {};

   ~ climateClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

};
			
#endif

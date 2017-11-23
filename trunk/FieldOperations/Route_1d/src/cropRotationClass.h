// Class: NCUClass                     //ANSI C++

#include "common.h"
#include "linklist.h"
#include <fstream>
using namespace std;
#ifndef __CROPROTCLASS_H
  #define __CROPROTCLASS_H


                               //Required for 1:M aggregation (part) classes
#ifndef __NCUCROPCLASS_H
  #include "NCUCropClass.h"
#endif

class cropRotationClass
{
   linkList<NCUCropClass> *theCrops; //1:M aggregation part data member

private:

     int Year; //Attribute data member

protected:

public:

			//Default constructor
  cropRotationClass ();

   ~ cropRotationClass ( );  //Destructor - Delete any pointer data members that used new in constructors

  int getYear() const  { return Year;}
  void setYear (int aYear)  { Year = aYear;}

  linkList <NCUCropClass>* gettheCrops()  { return theCrops;  }
  bool Initialise(fstream *afile);
};

#endif

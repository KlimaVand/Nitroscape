// Class: fieldOrderPeriod                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __FIELDORDERPERIOD_H  //Required for current class
  #define __FIELDORDERPERIOD_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include <message.h>        //Required for include files

#include <bstime.h>        //Required for include files

#include <common.h>        //Required for include files

#include <base.h>        //Required for include files
                               
//! used to read and store field operation periods from file
class fieldOrderPeriod  : public base
{                       
			
private:

protected:
//!Starting date of period
 string* StartDate;
//!End of period
 string* EndDate;
 //!Temperature sum that will trigger the operation
 double TSum;  

public:

			//!Default constructor
  fieldOrderPeriod () {};

	/*!Copy constructor alternative to compiler provided default copy constructor
   \param  afieldOrderPeriod The period to be copied
   */
  fieldOrderPeriod (const fieldOrderPeriod& afieldOrderPeriod );

                       //!Get accessor function for non-static attribute data member
  string* getStartDate() const  { return StartDate;}

                       //!Get accessor function for non-static attribute data member
  string* getEndDate() const  { return EndDate;}

                       //!Get accessor function for non-static attribute data member
  double getTSum() const  { return TSum;}
                       
  /*!Set accessor function for non-static attribute data member
  \param aStartDate date string
  */
  void setStartDate (string* aStartDate)  { StartDate = aStartDate;}

  /*!Set accessor function for non-static attribute data member
  \param aEndDate date string
  */
  void setEndDate (string* aEndDate)  { EndDate = aEndDate;}

  /*!Set accessor function for non-static attribute data member
  \param aTSum a temperature sum
  */
  void setTSum (double aTSum)  { TSum = aTSum;}

  /*!Constructor that also initialises the parameters inherited from base
  \param theName The name of the operation
  \param anIndex Redundant
  \param theBase Redundant
  */
    fieldOrderPeriod (const char * theName, const int anIndex=-1, const base * theBase=NULL)    ;

	/*!Reads the period details from file
   \param aFile Pointer to the input file
   */
   void ReadPeriod ( fstream* aFile)    ;
   /*!Returns the year from the date string
   \param date The date string to be used
   */
   int GetYear ( string* date)    ;

   /*!Returns the month from the date string
   \param date The date string to be used
   */
   int GetMonth ( string* date)    ;

   /*!Returns the day from the date string
   \param date The date string to be used
   */
   int GetDay ( string* date)    ;
   //!destructor
   ~ fieldOrderPeriod ( );  //Destructor - Delete any pointer data members that used new in constructors
};

#endif

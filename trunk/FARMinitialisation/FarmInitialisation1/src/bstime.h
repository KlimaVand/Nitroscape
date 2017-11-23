#ifndef BSTIME_H
   #define BSTIME_H
#include "common.h"

#ifndef __BCplusplus__
using namespace std;
#endif

enum monthSymbol
{
   January,
   February,
   March,
   April,
   May,
   June,
   July,
   August,
   September,
   October,
   November,
   December
};

//!Stores and manages anything to do with dates (documentation incomplete)

class bsTime
{
   private:
	//!Stores day
      int  day;
      //!Stores month
      int  month;
      //!Stores year
      int  year;
      //!Stores number of days in a year
      int  dayInYear;
      //!Stores number of days in a week
      int  dayInWeek;
      //!Stores number of days since 1 January 1900
      long longTime;
      //!Stores number of days in each month for leap years and not leap years
      int  tabDaysPerMonth[2][12];
      //!Stores number of days in each year since 1900
      long AccumulatedDaysSince1900[500];
      //!Initialises number of days per month
      void InitDaysPerMonth(void);

   public:
      //!Default constructor
      bsTime();
      //!Constructor with arguments
      /*!
       * @param d day
       * @param m month
       * @param y year
       */
      bsTime(int d, int m, int y);
      //!Constructor with arguments
      /*!
       * @param pointer to an instance of bsTime
       */
	  bsTime(const bsTime& f);
	  //!Set the time, integer input
	  /*!
       * @param d day
       * @param m month
       * @param y year
       */
      void SetTime(int d, int m, int y);
	  //!Set the time, string input
	  /*!
       * @param d day
       * @param m month
       * @param y year
       */
      void SetTime(string day,string month, string year);
	  //!Set the time, string input: alternative format
      /*!
       * @param aTimeString data as a string with format for separator of ".", "," or "-"
       */
	  void SetTime(string aTimeString);
	  //!Set the time, year and Julian Day
	  /*!
	   * @param Year year
	   * @param JulianDay Julian Day
	   */
	  void SetTime(int Year, int JulianDay);
	  //!Returns year
	  int GetYear() {return year;};
	  //!returns month
	  int GetMonth() {return month;};
	  //!Returns day
	  int GetDay() {return day;};
	  //!Returns number of days in the year
	  int GetDayInYear() {return dayInYear;};
	  //!Print the time
	  void PrintTime(void);
	  //!Add one day to the date
	  void AddOneDay(void);
	  //!Add one year to the date
	  void AddOneYear(void);
	  //!Return day in week at a C string
	  char * WeekDay(void);

	  //bsTime * CurrentDay(void);
	  string GetString();
      string GetString2();
      void CalculateDateValue();
      long GetDateValue() {return longTime;};
      bool FirstDay(int monthNr);
      bool FirstDayInMonth(void);
      bool LastDayInMonth(void);
      bool IsLeapYear(int year);
		int daysInYear(int offset);
      bool operator == (bsTime time2);
      bool operator < (bsTime time2);
      bool operator <=(bsTime time2);
		bool operator > (bsTime time2);
		bool operator >= (bsTime time2);
		int operator - (bsTime time2);
      friend ostream& operator<< (ostream& os, const bsTime& aTime);
      void AddDays(int nrOfDays);
		int GetJulianDay(int aday, int amonth, int ayear);
      int GetJulianDay();
		bool CalcDateFromJulianDay(int aJulianday);
      bsTime* clone() const;
//      virtual bsTime* clone() const;
};

extern bsTime theTime;
extern bsTime prepStopTime;
extern bsTime yearStopTime;
extern bsTime stopTime;

#endif


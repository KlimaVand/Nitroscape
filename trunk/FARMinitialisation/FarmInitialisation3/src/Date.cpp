#include "Date.h"


void Date::SetDate(int aday, int amonth, int ayear)
{
	year=ayear;
	month=amonth;
	day=aday;
};
int Date::getYear()
{
	return year;
}
int Date::getMonth()
{
	return month;
}
int Date::getDay()
{
	return day;
}

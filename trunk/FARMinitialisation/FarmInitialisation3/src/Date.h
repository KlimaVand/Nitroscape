#ifndef DATE_H_
#define DATE_H_
class Date 
	{
	private:
    int year;
    int month;
    int day;
	public:
	// constructors
	Date(){};
	Date(const Date& other) : year(other.year), month(other.month), day(other.day) {};
	// destructor
	~Date( ){};

	int getYear();
	int getMonth();
	int getDay();
	void SetDate(int aday, int amonth, int ayear);

	};
#endif

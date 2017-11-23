#ifndef SECTIONRECORDCLASS_H_
#define SECTIONRECORDCLASS_H_

#ifndef __BCplusplus__
using namespace std;
#endif
//!
#include "xmlParser.h"
#include "base/linklist.h"
#include "feedingRecordClass.h"
#include "Date.h"

class sectionRecordClass
{
	int livestockType;
	double number;
	linkList <feedingRecordClass> *theFeedingRecords;
	Date *theStartDate;
	Date *theEndDate;
public:

	// constructors
	sectionRecordClass();
	sectionRecordClass(sectionRecordClass& other);
	// desctructor
	~sectionRecordClass();

	void setlivestockType(int aVal){livestockType=aVal;}
	int getlivestockType(){return livestockType;}
	linkList <feedingRecordClass> *getTheFeedingRecords() { return theFeedingRecords; };
	int getnumber(){return number;}
	int getnumFeedingRecords(){return theFeedingRecords->NumOfNodes();}
	Date *getEndDate() { return theEndDate; }
	Date *getStartDate() { return theStartDate; }
	bool readsectionRecord(XMLNode *sectionRecordNode);
};
#endif



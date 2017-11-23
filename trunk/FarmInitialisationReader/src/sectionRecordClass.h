#ifndef SECTIONRECORDCLASS_H_
#define SECTIONRECORDCLASS_H_

#ifndef __BCplusplus__
using namespace std;
#endif
//!
#include "xmlParser.h"

class sectionRecordClass
{
	int livestockType;
	double number;

public:

	//constructor
	sectionRecordClass ();
	~sectionRecordClass( ){};

	void setlivestockType(int aVal){livestockType=aVal;}
	int getlivestockType(){return livestockType;}
	int getnumber(){return number;}
	bool readsectionRecord(XMLNode *sectionRecordNode);
};
#endif



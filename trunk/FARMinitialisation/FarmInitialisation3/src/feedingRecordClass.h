/*
 * File:   feedingRecordClass.h
 * Author: maury
 *
 * Created on 10 aout 2012, 14:03
 */

#ifndef FEEDINGRECORDCLASS_H
#define	FEEDINGRECORDCLASS_H
#include "xmlParser.h"
#include <string>
#ifndef __BCplusplus__
using namespace std;
#endif

class feedingRecordClass {
public:
	feedingRecordClass();
	feedingRecordClass(const feedingRecordClass& other) : id(other.id), feedType(other.feedType), feedAmount(other.feedAmount) {};
	feedingRecordClass(int id, int feedType, double feedAmount) : id(id), feedType(feedType), feedAmount(feedAmount) {};
	~feedingRecordClass(){};
	double getFeedAmount() { return feedAmount; };
	int getFeedCode();
	int getFeedType() { return feedType; };
	string getFeedTypeName();
	int getId() { return id; };
	bool readFeedingRecord(XMLNode *aNodeSection);
private:
	int id;
	int feedCode;
	int feedType;
	double feedAmount;
};

#endif	/* FEEDINGRECORDCLASS_H */


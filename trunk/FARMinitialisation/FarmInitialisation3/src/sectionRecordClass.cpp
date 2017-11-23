/**
 * Class for <lstkhousingrecord>
 *
 * Contains list of feedingRecordClass for the nodes <feedingrecord>.
 */
#include "sectionRecordClass.h"
#include "xmlParser.h"
#include <string>
#include "base/message.h"
#include "feedingRecordClass.h"


sectionRecordClass::sectionRecordClass ()
{
	livestockType = 0;
	number=0;
	theFeedingRecords = new linkList <feedingRecordClass>;
	theStartDate=new Date();
	theStartDate->SetDate(0,0,0);
	theEndDate=new Date();
	theEndDate->SetDate(0,0,0);
};
sectionRecordClass::sectionRecordClass(sectionRecordClass& other)
{
	livestockType = other.livestockType;
	number = other.number;
	// populate linkList
	theFeedingRecords = new linkList <feedingRecordClass>;
	int numOfNodes = other.getnumFeedingRecords();
	for (int i=0;i<numOfNodes; i++)
	{
		feedingRecordClass * afeedingRecordClass = other.getTheFeedingRecords()->ElementAtNumber(i);
		feedingRecordClass * newfeedingRecordClass = new feedingRecordClass(*afeedingRecordClass);
		theFeedingRecords->InsertLast(newfeedingRecordClass);
	}
	//-
	theStartDate = new Date(*other.getStartDate());
	theEndDate = new Date(*other.getEndDate());
}
sectionRecordClass::~sectionRecordClass()
{
	delete theStartDate;
	delete theEndDate;
	delete theFeedingRecords;
}
bool sectionRecordClass::readsectionRecord(XMLNode *sectionRecordNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=sectionRecordNode->getChildNode("lstkhousedtypeid");
	string tempstr = xNode.getText();
	if (tempstr.size() > 0)
	{
		livestockType = atoi(tempstr.c_str());
	}
	else
	{
		//livestockType=0;
		int ID=0;
		xNode=sectionRecordNode->getChildNode("lstkhousingrecordid");
		if (tempstr.size())
			ID=atoi(tempstr.c_str());
		theMessage->WarningWithDisplay("Livestock type missing in section record with ID ", ID);
		retVal=false;
	}
	xNode=sectionRecordNode->getChildNode("lstkhousednumber");
	if (tempstr.size())
		number = atoi(tempstr.c_str());
	else
	{
		number =0;
		retVal=false;
   }
	xNode=sectionRecordNode->getChildNode("lstkhousedstartdate");

	if (tempstr.size())
	{
		//get string holding date and parse into Date class
		if (tempstr.size() >= 10)
		{
			string astryear = tempstr.substr(0, 4);
			string astrmonth = tempstr.substr(5, 2);
			string astrday = tempstr.substr(8, 2);
			int ayear = atoi(astryear.c_str());
			int amonth = atoi(astrmonth.c_str());
			int aday = atoi(astrday.c_str());

			theStartDate->SetDate(aday, amonth, ayear);
		}
	}
	else
	{
		retVal=false;
	}

	xNode=sectionRecordNode->getChildNode("lstkhousedenddate");

	if (tempstr.size())
	{
		if (tempstr.size() >= 10)
		{
			string astryear = tempstr.substr(0, 4);
			string astrmonth = tempstr.substr(5, 2);
			string astrday = tempstr.substr(8, 2);
			int ayear = atoi(astryear.c_str());
			int amonth = atoi(astrmonth.c_str());
			int aday = atoi(astrday.c_str());

			theEndDate->SetDate(aday, amonth, ayear);
		}
	}
	else
	{
		retVal=false;

	}

	/* Reads <feedingrecord> */
	int numFeedingRecords = sectionRecordNode->nChildNode("feedingrecord");
	for (int i=0;i<numFeedingRecords; i++) {
		XMLNode feedingRecordNode = sectionRecordNode->getChildNode("feedingrecord",i);
		feedingRecordClass * afeedingRecordClass = new feedingRecordClass();
		retVal = afeedingRecordClass->readFeedingRecord(&feedingRecordNode);
		if (retVal) {
			theFeedingRecords->InsertLast(afeedingRecordClass);
		}
	}
      return retVal;
}

/*int sectionRecordClass::getnumFeedingRecords()
{
		return getTheFeedingRecords()->NumOfNodes();
}*/

#include "sectionRecordClass.h"
#include "xmlParser.h"


sectionRecordClass::sectionRecordClass ()
{
	livestockType = 0;
	number=0;
};

bool sectionRecordClass::readsectionRecord(XMLNode *sectionRecordNode)
{
	bool retVal = true;
	XMLNode xNode;
	xNode=sectionRecordNode->getChildNode("lstkhousedtypeid");
	if (xNode.getText())
		livestockType = atof(xNode.getText());
	else
   {
		livestockType=0;
      retVal=false;
   }
	xNode=sectionRecordNode->getChildNode("lstkhousednumber");
	if (xNode.getText())
		number = atoi(xNode.getText());
	else
	{
		number =0;
      retVal=false;
   }
	return retVal;
}



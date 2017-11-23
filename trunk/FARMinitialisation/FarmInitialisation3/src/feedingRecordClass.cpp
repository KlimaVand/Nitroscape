/*
 * File:   feedingRecordClass.cpp
 * Author: maury
 *
 * Created on 10 aout 2012, 14:03
 */
#include <string>
#include "feedingRecordClass.h"
#include "base/message.h"

feedingRecordClass::feedingRecordClass() {
	id = 0;
	feedType = 0;
	feedAmount = 0.0;
}

int feedingRecordClass::getFeedCode() {
	/*
	 * Generated from the database using
	 * SELECT 'case ' || feedtypeid || ': return ' || fasset_feedcode || ';' FROM f_feedtype_lut ORDER BY feedtypeid;
	 *
	 * TODO: use the XML with description > feedtype > fasset_feedcode
	 */
	switch (feedType) {
	case 1: return 298;
	case 2: return 142;
	case 3: return 213;
	case 4: return 145;
	case 5: return 203;
	case 6: return 155;
	case 7: return 311;
	case 8: return 385;
	case 9: return 427;
	case 10: return 465;
	case 11: return 448;
	case 12: return 602;
	case 13: return 565;
	case 14: return 590;
	case 15: return 593;
	case 16: return 665;
	case 17: return 781;
	case 18: return 312;
	case 19: return 155;
	case 20: return 267;
	case 21: return 261;
	case 22: return 285;
	case 23: return 602;
	case 24: return 201;
	case 25: return 15;
	case 26: return 308;
	case 27: return 602;
	default:
		theMessage->WarningWithDisplay("In the feeding record, unknown livestock category for feedCode: ", feedType);
		return 0;
	}
}
string feedingRecordClass::getFeedTypeName() {
	/*
	 * Generated from the database using
	 * SELECT 'case ' || feedtypeid || ': return "' || feedtypename || '";' FROM f_feedtype_lut ORDER BY feedtypeid;
	 *
	 * TODO: use the XML with description > feedtype > feedtypename
	 */
	switch (feedType) {
		case 2: return "medium energy concentrate";
		case 3: return "high energy concentrate";
		case 4: return "rape cake";
		case 5: return "grain cereal";
		case 6: return "soy beans";
		case 7: return "feed milk";
		case 8: return "sugar beets";
		case 9: return "grass clover fresh";
		case 10: return "grass fresh";
		case 11: return "whole crop fresh";
		case 12: return "clover grass silage";
		case 13: return "grass silage";
		case 14: return "whole crop cereal silage";
		case 15: return "maize silage";
		case 16: return "hay";
		case 17: return "straw";
		case 18: return "whey";
		case 19: return "soybean oil cake";
		case 20: return "beet pulp (dried)";
		case 21: return "wet distillery grain";
		case 22: return "beet pulp silage";
		case 23: return "alfalfa green forage";
		case 24: return "alfalfa silage";
		case 25: return "full-ration concentrate mix";
		case 26: return "fresh milk";
		case 27: return "Other fodder mixes inc. biscuits";
		default:
			theMessage->WarningWithDisplay("In the feeding record, unknown livestock category for feedTypeName: ", feedType);
			return "Unknown feed type";
	}
}

bool feedingRecordClass::readFeedingRecord(XMLNode *sectionRecordNode) {
	bool retVal = true;
	XMLNode xNode;
	/* feedingrecordid */
	xNode = sectionRecordNode->getChildNode("feedingrecordid");
	if (xNode.getText()) {
		id = atoi(xNode.getText());
	}

	/* feedtypeid */
	xNode = sectionRecordNode->getChildNode("feedtypeid");
	if (xNode.getText()) {
		feedType = atof(xNode.getText());
	} else {
		feedType = 0;
		theMessage->WarningWithDisplay("Feed type missing in feeding record with ID ", id);
		retVal = false;
	}

	/* feedamount */
	xNode = sectionRecordNode->getChildNode("feedamount");
	if (xNode.getText())
		feedAmount = atoi(xNode.getText());
	else {
		feedAmount = 0;
		retVal = false;
	}
	return retVal;
}


/*
 * MetDataExtractionRoutinesMARSREMO.h
 *
 *  Created on: 28 Jan 2011
 *      Author: NJH
 */
#include <iostream>
#include <string>
#ifndef __BCplusplus__
#include <cstdlib>
#include <sstream>
#endif
#include <vector>
#include <math.h>

#include "message.h"

#ifndef METDATAEXTRACTIONROUTINESMARSREMO_H_
#define METDATAEXTRACTIONROUTINESMARSREMO_H_

int getMetData(int ID, int startDay, int startYear, int endDay, int endYear, char * inputDir,
	char * tpvStem, char * radStem, char * ncu_mars, int baseYear, int range, double results[][366][6]);


#endif /* METDATAEXTRACTIONROUTINESMARSREMO_H_ */

/*
 * pixiData.cpp
 *
 *  Created on: Jul 19, 2010
 *      Author: JOVE
 */

#include "pixiData.h"
#include "../tools/convert.h"
#include "../tools/fileAccess.h"
#include "globalData.h"
void pixiData::loadData(string path)
{
	fileAccess hd;
	hd.changeDir(path);
	//assumes fasset.dist is two levels up in path hierarchy
    string thePath =hd.getCurrentPath();
    cout << "fasset.dist path " << thePath << endl;
	readFileWithExit("fasset.dist");
	convert conv;

	for(unsigned int i=1;i<items.size();i++)
	{
		if(items.at(i).compare("NULL")==0){
			break;
		}
		cout<<items.at(i)<<endl;
		unsigned int farm=conv.StringToInt(conv.partOfAString(items.at(i),1));
		unsigned int field=conv.StringToInt(conv.partOfAString(items.at(i),2));
		unsigned int eco=conv.StringToInt(conv.partOfAString(items.at(i),3));

		while(farmInformation.size()<=farm)
		{
			vector< vector<int> > fieldVector;
			farmInformation.push_back(fieldVector);
		}
		while(farmInformation.at(farm).size()<=field)
		{
			vector<int> eco;
			farmInformation.at(farm).push_back(eco);
		}
		farmInformation.at(farm).at(field).push_back(eco);

	}


}

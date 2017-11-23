
#ifndef MANURESTORAGE_H_
#define MANURESTORAGE_H_

#include "xmlParser.h"
#include "base/common.h"

class manureStorageClass
{
	int manurestoreid;
	int type;
	double surfaceArea;
	int manureType;
	int storeType;

public:

	//constructor
	manureStorageClass ();
	~manureStorageClass( ){};

	void setType(int aVal){type=aVal;}
	int getType(){return type;}
	void setsurfaceArea(double aVal){surfaceArea = aVal;}
	double getsurfaceArea(){return surfaceArea;}
	int getmanurestoreid(){return manurestoreid;};
	int getmanureType(){return manureType;}
	void setmanureType(int aVal){manureType=aVal;}
	int getStoreType() {return storeType;}

	bool readManureStorage(XMLNode *aNode);
	void printContents();
	void writeStorageToBuildingsFile(ofstream *outfile);
	int getManureName(string *nameString);
};

#endif /* MANURESTORAGE_H_ */

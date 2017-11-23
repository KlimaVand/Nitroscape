#ifndef globalDataH
#define globalDataH
#include <string>
using namespace std;
class globalData
{
private:
	string outputDir;
	string intputDir;
	int farmNr;
public:
	void setFarmNr(int farm){farmNr=farm;};
	void setIndputDir(string indput){intputDir=indput;};
	void setOutputDirectory(string output){outputDir=output;};
	string getInputDirectory(){return intputDir;};
	string getOutputDirectory(){return outputDir;};
	int getFarmNr(){return farmNr;};
};

extern globalData * theGlobalData;
#endif

#include <stdio.h>    // to get "printf" function
#include <stdlib.h>   // to get "free" function
#ifndef __WINDOWS__
	#include <ostream.h>
	#include <istream.h>
#endif
#include <sys/stat.h>
#include "common.h"
#include "farmInitClass.h"
#include "xmlParser.h"

XMLNode XMLdescription;

using namespace std;

int main(int argc, char **argv)
{
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "Reading XML file from "<< tempPath << endl;

  int startYear = 1988;
  int endYear = 1990;
// open and parse the management XML file:

  XMLNode xMainNode=XMLNode::openFileHelper("management.xml","nitroscape");
  XMLdescription = xMainNode.getChildNode("description");

  //!Recover and parse the landscape section
  XMLNode xNodeLandscape=xMainNode.getChildNode("landscape",0);
  //!Return the number of farms in the file
  int numberOfFarms = xNodeLandscape.nChildNode("farm");

  //!Generate a subdirectory for each farm, using the format "farmnn", where nn = the number of the farm in the XML file
  string stub = "farm";
  for (int i=0;i<numberOfFarms; i++)
  {
	  //!Recover the farm node
	  XMLNode xNodeFarm=xNodeLandscape.getChildNode("farm",i);

	  char buf[3];
	  sprintf(buf,"%d",i);
	  string temp = stub + buf;
#ifdef __WINDOWS__
	  int k1=mkdir(temp.c_str(),'-p');
#else
	  int k1=mkdir(temp.c_str(),0777);
#endif
	  if (k1>0)
		  cout << "Error creating directory: error code =  " << k1 << endl;
	  else
	  {
#ifdef __WINDOWS__
		  string dirName = temp;
#else
		  string dirName = "./" + temp + "/";
#endif
		  k1 = chdir(dirName.c_str());
		  if (k1==0)
		  {
			  getcwd(tempPath, 256);
			  cout << "Writing to directory " << tempPath << endl;
		  }
		  else
			  cout << "Error in chdir to: " << dirName << endl;
	  }
	  cout << "Accessing farm number " << i << endl;
	  farmInitClass	*farmData = new farmInitClass();
	  //!Currently only initialises arable farms
	  farmData->initFarm(&xNodeFarm);
	  bool pig=false;
	  bool dairy=false;
	  bool beef=false;
	  if (farmData->writeLivestockfnn(startYear, endYear, &dairy, &beef, &pig)!=0)
		  cout << "Error writing livestock input files";
	  farmData->writeFarmdat(pig, dairy, beef);

	  //!Create and write to field.dat
	  farmData->initFielddat();
	  //!Read the field operations
	  farmData->readFieldOps(startYear, endYear);
//	  farmData->writeFieldOps();
	  farmData->writeFieldsfnn(startYear, endYear);

	  delete farmData;
	  chdir("..//");
  }
}



/*
Reads file 'management.xml' and generates FASSET-FARM input data files
 */
#include <stdio.h>    // to get "printf" function
#include <stdlib.h>   // to get "free" function
#ifndef __WINDOWS__
#include <ostream.h>
#include <istream.h>
#endif
#include <sys/stat.h>
#include "base\common.h"
#include "farmInitClass.h"
#include "xmlParser.h"
#include "tools\fileAccess.h"
#include "base\message.h"
#include "base\bstime.h"
#include <string.h>

#include "farmInitClass.h"
#include "base\linkList.h"

using namespace std;

XMLNode XMLdescription;
message *theMessage;
bsTime theTime;
bool verbosity;


/*
 * Input format for command.txt
 * Line 1 input directory
 * Line 2 output directory
 * Line 3 start year
 * Line 4 end year
 * Line 5 if 'verbosity', detailed output to console will be produced
 * */

int main(int argc, char **argv)
{
	char tempPath[256];
	getcwd(tempPath, 256);
	cout << "Reading command.txt file from "<< tempPath << endl;
	fileAccess hd;
	hd.openFile("command.txt",false);
	string inputDir = hd.getCheckedLineFile();
	string outputDir = hd.getCheckedLineFile();
	string inputFileName = hd.getCheckedLineFile();
	int startYear = hd.GetIntFromFile();
	int endYear = hd.GetIntFromFile();
	string temp = hd.getCheckedLineFile();
	if (temp=="verbosity")
		verbosity=true;
	else
		verbosity=false;
	string cropCode = hd.getCheckedLineFile();
	int opNum = hd.GetIntFromFile();
	hd.changeDir(inputDir);
	hd.closeFile();

	theMessage = new message();
	typedef char string100[256];
	string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
	strcpy(OutputDirectory, outputDir.c_str());
	strcpy(FN1,OutputDirectory);
	strcat(FN1,"warnings.txt");
	strcpy(FN2,OutputDirectory);
	strcat(FN2,"logfile.txt");
	strcpy(FN3,OutputDirectory);
	strcat(FN3,"debug.txt");      //debug file name
	if (!theMessage->InitMessage(FN1,FN2,FN3, true))
		theMessage->FatalError("FARM_initiator: Failed to open one or more reporting files");
	theMessage->SetshowTime(false);

	getcwd(tempPath, 256);
	cout << "Reading XML file from "<< tempPath << endl;

	// open and parse the management XML file:

	XMLNode xMainNode=XMLNode::openFileHelper(inputFileName.c_str(),"nitroscape");
	if (hd.changeDir(outputDir))
	{
		hd.CreateDir(outputDir);
	}

	XMLdescription = xMainNode.getChildNode("description");

	//!Recover and parse the landscape section
	XMLNode xNodeLandscape=xMainNode.getChildNode("landscape",0);
	//!Return the number of farms in the file
	int numberOfFarms = xNodeLandscape.nChildNode("farm");

	linkList <farmInitClass> *theFarms;
	theFarms = new linkList <farmInitClass>;

	for (int i=0;i<numberOfFarms; i++)
	{
		bool cont=true;
		//!Recover the farm node
		XMLNode xNodeFarm=xNodeLandscape.getChildNode("farm",i);


		cout << "Accessing farm number " << i;
		XMLNode xNode=xNodeFarm.getChildNode("farmid",0);
		if (xNode.getText())
		{
			int farmID= atoi(xNode.getText());
			cout << " ID = " << farmID << endl;
		}
		farmInitClass	*farmData = new farmInitClass();
		if (cont)
			cont = farmData->initFarm(&xNodeFarm);

		//!Read the field operations
		//	  if (cont)
		cont = farmData->readFieldOps(startYear, endYear);
		//	  if (cont)
		farmData->writeFieldOps(true);
		hd.changeDir(outputDir);
		if (!cont)
		{
			theMessage->WarningWithDisplay("Farm no ",i," was not processed due to an error ");
			theMessage->WarningWithDisplay("Farm ID is ",farmData->getfarmID());
		}
		else
			theFarms->InsertLast(farmData);
		cont=true;
	}
	fstream * outfile = new fstream();
//	cropCode.assign("W1");
	string cropFileName;
	cropFileName.assign(cropCode);
	cropFileName.append("_");
	char dum[2];
	itoa(opNum,dum,2);
	cropFileName.append(dum);
	cropFileName.append(".txt");
	outfile->open(cropFileName.c_str(),ios::out);
	for (int i=0; i<theFarms->NumOfNodes(); i++)
	{
		farmInitClass *afarmInitClass = theFarms->ElementAtNumber(i);
		afarmInitClass->FindCropOps(cropCode,opNum,outfile);
	}
	outfile->close();
	cout << "Finished" << endl;
	delete theFarms;
}



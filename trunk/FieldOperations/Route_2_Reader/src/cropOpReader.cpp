/*! \mainpage Nitroscape Component 5 Route 1 file reader

   Group of C++ classes that are used to write or read the field operations to be used in
   NitroEurope Component 5.
   The file cropOpReader.cpp provides entry to a simple program that reads the file that provides the detailed
   ecosystem models with field operation data. The program creates instances of classes specific to individual
   field operations (e.g. tillage, harvesting). These classes contain functions that access operation-specific
   data (e.g. amount of nitrate applied in a fertiliser application). The program outputs the year, Julian day,
   operation name and operation code to console.

   For Windows,use __WINDOWS__ compiler directive, for Borland 5.02 use also __BCplusplus__, for CYGWIN, use __ANSICPP__
   For Linux, use __ANSICPP__ only 

   NJH 2.2.10

 */



#include "base\message.h"
#include "base\bstime.h"
#include "fieldOp.h"
#include "fieldOpTill.h"
#include "fieldOpSow.h"
#include "fieldOpFert.h"
#include "fieldOpHarv.h"
#include "fieldOpGrass.h"
#include "fieldOpCut.h"
#include "base\unix_util.h"
#include "tools\fileAccess.h"


//! used to send messages to screen or file
message * theMessage;
//! keeps the time and date
bsTime theTime;

//!main function for Route 1 file reader
int main(int argc,char *argv[])
{
//! If verbosityLevel = 1, detailed output is sent to the screen
	int verbosityLevel = 0;
	fileAccess hd;
	string commandFileName = "command.txt";
	hd.openFile(commandFileName,false);
	string input=hd.getLineFile();
	char inputDir[2000] ;
	strcpy(inputDir,input.c_str());
	int asci=inputDir[input.size()-1];
	if(asci==13)
	{
		inputDir[input.size()-1]='\0';
	}

	char outputDir[2000] ;
	string output=hd.getLineFile();
	strcpy(outputDir,output.c_str());
	asci=outputDir[output.size()-1];
	if(asci==13)
	{
		outputDir[output.size()-1]='\0';
	}

	char fileName[200];
	string atempname;// "DummyCroprot5";
	atempname=hd.getLineFile();
	strcpy(fileName,atempname.c_str());
	asci=fileName[atempname.size()-1];
	if(asci==13)
	{
		fileName[atempname.size()-1]='\0';
	}

	char outfileName[200];
	atempname=hd.getLineFile();
	strcpy(outfileName,atempname.c_str());
	asci=fileName[atempname.size()-1];
	if(asci==13)
	{
		outfileName[atempname.size()-1]='\0';
	}
	int dumint;
	bool stopForEachSeries=false;
	dumint = hd.GetIntFromFile();
	if (dumint==1)
		stopForEachSeries = true;
	verbosityLevel = hd.GetIntFromFile();	
	hd.closeFile();

	typedef char string100[256];
	string100 FN1,FN2, FN3, FN4, FN5, OutputDirectory;
	strcpy(OutputDirectory, outputDir);
	strcpy(FN1,OutputDirectory);
	strcat(FN1,"warnings.txt");
	strcpy(FN2,OutputDirectory);
	strcat(FN2,"logfile.txt");
	strcpy(FN3,OutputDirectory);
	strcat(FN3,"debug.txt");      //debug file name
	theMessage = new message();
	theMessage->InitMessage(FN1,FN2,FN3);

	//change to input directory
	hd.changeDir(input);

	//	chdir(inputDir);
	char LongFileName [200];
	sprintf(LongFileName,"%s\%s",inputDir,fileName);
	ifstream *opFile = new ifstream();
	opFile->open(LongFileName,fstream::in);
	if (!opFile->is_open())
		theMessage->FatalError("Crop operation file ", fileName, " not found");
	else
		cout << "Reading " << LongFileName << endl;

	sprintf(LongFileName,"%s\%s",outputDir,outfileName);
	ofstream *opFileOut = new ofstream();
	opFileOut->open(LongFileName,fstream::out);
	if (!opFileOut->is_open())
		theMessage->FatalError("Unable to open output file");

	int NCUcount=0;
	int NumberOfNoOps = 0;
	int NCU =0;
	int oldNCU=0;
	int cropSeries =0;
	int oldCropSeries=-1;
	int oldYear=0;
	bool finished = false;  //only here because with the Borland compiler, is_open does not want to return false at end of file
	bool canBeGrazed= false;
#ifdef __BCplusplus__
	while ((opFile)&&(!finished))
#else
		while ((opFile->is_open())&&(!finished))
#endif
		{
			*opFile >> NCU >> cropSeries;
			if (opFile->eof())
				finished=true;
			else
			{
				if (verbosityLevel>0)
					cout << "NCU " << NCU << " series " << cropSeries << " ";
				int numOps =0;
				int year =0;
				*opFile >> year >> numOps;
				if (verbosityLevel>0)
					cout << year  << endl;
				if (NCU!=oldNCU)
				{
					oldNCU=NCU;
					oldYear=year-1;
					oldCropSeries=0;
					NCUcount++;
/*					if (stopForEachSeries)
					{
						cout << "<CR> = continue, q = quit" << endl;
						char c;
						cin.get(c);
						if (strcmp(&c,"q")==0)
							exit(1);
					}*/
				}
				//this checks if the end of file has been reached (should not be necessary but sometimes is, depending on compiler implementation of eof() function)
				if (year==0)
					finished=true;
				if (numOps==0)
					NumberOfNoOps++;
				if (cropSeries!=oldCropSeries)
				{
					oldCropSeries=cropSeries;
					oldYear=year;
/*					if (stopForEachSeries)
					{
						cout << "<CR> = continue" << endl;
						char c;
						cin.get(c);
					}*/
				}
				else
				{
					if (year!=oldYear+1)
					{
						cout << "Error - NCU " << NCU << " crop series " << cropSeries << " year " << year << endl;
						theMessage->FatalError("Years are not in sequence");
					}
					else
						oldYear=year;
				}
				bool blockNextSowing=false;
				bool blockNextTillage=false;
				string cropPlanted;
				for (int i=0;i< numOps; i++)
				{
					int julianDay=0;
					int opCode=0;
					fieldOp * afieldOp;
					*opFile >> julianDay >> opCode;
					//! On basis of operation code (opCode), initialise an instance of the appropriate field operation class
					switch (opCode)
					{
					case 1: //tillage
						afieldOp = new fieldOpTill();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						if (blockNextTillage)
						{
							cout << "Existing crop " << cropPlanted << endl;;
							theMessage->FatalError("Tillage before previous crop harvested");
						}
						else
							blockNextSowing=false;
						break;
					case 2://sowing
						afieldOp = new fieldOpSow();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						if (blockNextSowing)
						{
							cout << *afieldOp->gettheOptime() << " " << afieldOp->gettheOptime()->GetDateValue() << " " << afieldOp->getopCode()
											<< " " << ((fieldOpSow *) afieldOp)->getplantName() << endl;
							theMessage->FatalError("Sowing before previous crop harvested");
						}
						else
						{
							blockNextSowing=true;
							if ((((fieldOpSow *) afieldOp)->getplantName()=="SETA")
									|| (((fieldOpSow *) afieldOp)->getisCover()))
								blockNextTillage=false;
							else
								blockNextTillage=true;
						}
						cropPlanted = ((fieldOpSow *)afieldOp)->getplantName();
						if ((cropPlanted=="GRAS")||(cropPlanted=="GRAP"))
							canBeGrazed=true;
						else
							canBeGrazed=false;
						break;
					case 3://fertilisation and manuring
						afieldOp = new fieldOpFert();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						break;
					case 4://cutting
						afieldOp = new fieldOpCut();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						blockNextTillage=false;
						blockNextSowing=false;
						break;
					case 5://start grazing
						if (!canBeGrazed)
							theMessage->FatalError("Cannot graze this crop");
						afieldOp = new fieldOpGrass(5);
						afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
						blockNextTillage=false;
						blockNextSowing=false;
						break;
					case 6://stop grazing
						if (!canBeGrazed)
							theMessage->FatalError("Cannot graze this crop");
						afieldOp = new fieldOpGrass(6);
						afieldOp->ReadOpResults(opFile,year,julianDay,opCode);
						break;
					case 9://harvesting
						afieldOp = new fieldOpHarv();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						blockNextTillage=false;
						blockNextSowing=false;
						break;
					case 10://atmospheric N deposition
						afieldOp = new fieldOpFert();
						afieldOp->ReadOpResults(opFile,year,julianDay);
						break;
					default:
						char buf[2];
						itoa(opCode,buf,10);
						cout << "Operation number " << i << endl;
						theMessage->FatalError("Incorrect or unprogrammed operation code ", buf);
						break;
					}
					//afieldOp->CheckContent();
					if (verbosityLevel==2)
					{
						if (afieldOp->getopCode()==2)
							cout << "Op no " << i << " Op name " << afieldOp->getname() << " Op code " << afieldOp->getopCode() << " Day " << julianDay << " Crop sown " << (((fieldOpSow *) afieldOp)->getplantName()) <<endl;
						else
							cout << "Op no " << i << " Op name " << afieldOp->getname() << " Op code " << afieldOp->getopCode() << " Day " << julianDay << endl;
					}
					delete afieldOp;
				}
			}
		}
	opFile->close();
	opFileOut->close();
	delete opFile;
	delete opFileOut;
	delete theMessage;
	cout << "Finished after " << NCUcount << " NCUs" << " No of NCU without ops " << NumberOfNoOps << endl;
}

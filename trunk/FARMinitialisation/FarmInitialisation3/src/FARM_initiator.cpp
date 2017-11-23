/**
 * Reads file 'management.xml' and generates FASSET-FARM input data files
 *
 * $Id:$
 */
#include <stdio.h>    // to get "printf" function
#include <stdlib.h>   // to get "free" function

// for loadFileConf()
#ifndef VS2008
#include <glib.h>
#endif
#include <sys/stat.h>
#include "base/common.h"
#include "farmInitClass.h"
#include "xmlParser.h"
#include "tools/fileAccess.h"
#include "base/message.h"
#include "base/bstime.h"
#include <string.h>

#include "farmInitClass.h"
#include "base/linklist.h"

using namespace std;

XMLNode XMLdescription;
message *theMessage;
bsTime theTime;
bool verbosity;
#ifdef UNIX
	char directory_separator = '/';
#else
	string directory_separator = "\\\\";
#endif
string log_info = "FASSET\tINFO\t";
string log_warning = "FASSET\tWARNING\t";

// Config
string inputDir;
string outputDir;
string inputFileName; // XML file
int startYear;
int endYear;
string metfileName;

void loadFileConf();
int strpos(char *haystack, char *needle);

//NOTE - no cleanup of pointers in destructors!!!
void writeSystemDat(linkList <farmInitClass> *theFarms, string *destDirectory, string *metfileName, int startYear, int stopYear);
void writeFarmdat(string *destDirectory, bool pig, bool dairy, bool beef);

int main(int argc, char **argv)
{
   loadFileConf();
	if (verbosity) {
		cout << log_info << "Verbosity  : on" << endl;
		cout << log_info << "Input dir  : " << inputDir << endl;
		cout << log_info << "Output dir : " << outputDir << endl;
		cout << log_info << "XML file   : " << inputFileName << endl;
		cout << log_info << "Start year : " << startYear << endl;
		cout << log_info << "End year   : " << endYear << endl;
		cout << log_info << "Meteo file : " << metfileName << endl;
	}
	fileAccess hd;
	if (!hd.directoryExists(inputDir)) {
		cout << log_warning << "The directory " << inputDir << " does not exist!" << endl;
		exit(EXIT_FAILURE);
	}
	hd.changeDir(inputDir);
	if (hd.directoryExists(outputDir)) {
		cout << log_warning << "The directory " << outputDir << " already exists!" << endl;
	}
	else if (hd.CreateDirectoryW(outputDir)) {
		return -1;
	}
   std::string   OutputDirectory1;
   OutputDirectory1.assign(outputDir);
   char FN1[200],FN2[200], FN3[200];
   strcpy(FN1,OutputDirectory1.c_str());
   strcat(FN1,"warnings.txt");
   strcpy(FN2,OutputDirectory1.c_str());
   strcat(FN2,"logfile.txt");
   strcpy(FN3,"\0");
   theMessage = new message();
   if (!theMessage->InitMessage(FN1,FN2,FN3))
	   theMessage->FatalError("FARM_initiator: Failed to open one or more reporting files");
   theMessage->setshowTime(false);

   char message[60];

	cout << log_info << "Reading XML file from "<< hd.getCurrentPath() << endl;

// open and parse the management XML file:

  XMLNode xMainNode=XMLNode::openFileHelper(inputFileName.c_str(),"nitroscape");
  hd.changeDir(outputDir);

  XMLdescription = xMainNode.getChildNode("description");

  //!Recover and parse the landscape section
  XMLNode xNodeLandscape=xMainNode.getChildNode("landscape",0);
  //!Return the number of farms in the file
  int numberOfFarms = xNodeLandscape.nChildNode("farm");

  linkList <farmInitClass> *theFarms;
  theFarms = new linkList <farmInitClass>;

  //!Generate a subdirectory for each farm, using the format "farmnn", where nn = the number of the farm in the XML file
  int k1;
  char buf[3];
  string farmDir;
  string stub = "farm";
  for (int i=0;i<numberOfFarms; i++)
  {
	  bool cont=true;
	  //!Recover the farm node
	  XMLNode xNodeFarm=xNodeLandscape.getChildNode("farm",i);

	  sprintf(buf,"%d",i);
	  farmDir = stub + buf;
	  if (!hd.directoryExists(farmDir)) {
		  k1 = hd.CreateDirectoryW(farmDir);
	  } else {
		  k1 = 0;
	  }
	  if (k1>0)
		  cout << log_warning << "Error creating directory: error code =  " << k1 << endl;
	  else
	  {
#ifdef __WINDOWS__
		  string dirName = farmDir;
#else
		  string dirName = "./" + farmDir + "/";
#endif
		  k1 = chdir(dirName.c_str());
		  if (k1==0)
		  {
				if (verbosity)
					cout << log_info << "Writing to directory " << hd.getCurrentPath() << endl;
		  }
		  else
		  {
			  cout << log_warning << "Error in chdir to: " << dirName << endl;
			  cont=false;
		  }
	  }
		XMLNode xNode=xNodeFarm.getChildNode("farmid",0);
		if (xNode.getText())
		{
			int farmID= atoi(xNode.getText());
			// TODO: remove after Debug
			
			if (farmID == 2006) {
			//	if (chdir("../") != 0) {
				//	cout << log_warning << "Error in chdir to: ../" << endl;
				cout <<"2006";
//				}
			//	continue;
			}
			
			//-
			cout << log_info << "Accessing farm number #" << i << " (ID=" << farmID << ")" << endl;
		} else {
			theMessage->WarningWithDisplay("Farm ID missing for farm #", i);
			continue;
		}
	  farmInitClass	*farmData = new farmInitClass();
	  farmData->setoutputDirectoryName(outputDir);
	  if (cont) {
		  cont = farmData->initFarm(&xNodeFarm);
		  if (!cont) {
			  theMessage->WarningWithDisplay("Farm init failed for farm#", i);
		  }
	  }

	  bool pig=false;
	  bool dairy=false;
	  bool beef=false;
	  bool poultry=false;
	  if (cont)
	  {
		  if (verbosity)
			  cout << log_info << "Reading livestock" << endl;
	  	  cont = farmData->readLivestock(&dairy, &beef, &pig, &poultry);
	  	  if (!cont)
		  {
			  cout << log_warning << "Error reading livestock." << endl;
			  cont=false;
		  }
	  }

	  //!Read the field operations
	  if (cont) {
		  if (verbosity)
			  cout << log_info << "Reading field operations" << endl;
		  cont = farmData->readFieldOps(startYear, endYear);
		  if (!cont) {
			  sprintf(message, "Reading field operations failed for farm #%d", i);
			  theMessage->WarningWithDisplay(log_warning, message);
		  }
	  }
	  // write building data
	  if (cont) {
		  if (verbosity)
			  cout << log_info << "Write building data for farm #" << i << endl;
		  farmData->writeBuildings();
	  }

	  // write farm.dat
	  if (cont) {
		  if (verbosity)
			  cout << log_info << "Write farm.dat for farm #" << i << endl;
		  farmData->writeFarmdat(pig, dairy, beef, poultry, true);
	  }

	  //!Create and write to field.dat
	  if (cont) {
		  if (verbosity)
			  cout << log_info << "Write field data for farm #" << i << endl;
		  cont = farmData->writeFielddat();
	  }

	  //!Create and write to Fields.fnn
	  if (cont) {
		  if (verbosity)
			  cout << log_info << "Write Fields.fnn for farm #" << i << endl;
		  cont = farmData->writeFieldsfnn(startYear, endYear);
	  }

	  // write livestock data
	  if (cont)
	  {
		  if (verbosity)
			  cout << log_info << "Write livestock data for farm #" << i << endl;
	  	  cont = farmData->writeLivestockfnn(dairy, beef, pig, poultry);
	  	  if (!cont)
		  {
			  sprintf(message, "Error writing livestock output files for farm #%d", i);
			  theMessage->WarningWithDisplay(log_warning, message);
			  cont=false;
		  }
	  }
	  /*
	   * another writer for farm.dat !
	  if (cont) {
		  if (verbosity)
			  cout << "writeFarmDat() for Farm #" << i << endl;
		  writeFarmdat(&farmData->getoutputDirectoryName(), pig, dairy, beef);
	  }
	  */

	  if (!cont)
	  {
		  sprintf(message, "Farm no %2d (ID=%d) was not processed due to an error", i, farmData->getfarmID());
		  theMessage->WarningWithDisplay(log_warning, message);
	  }
	  else
		  theFarms->InsertLast(farmData);
	  hd.changeDir("../");
	  cont=true;
  }
  string destDirectory;
  destDirectory.assign(outputDir);
  writeSystemDat(theFarms, &destDirectory, &metfileName, startYear, endYear);
  cout << log_info << "Finished" << endl;
  delete theFarms;
}


void writeSystemDat(linkList <farmInitClass> *theFarms, string *destDirectory, string *metfileName, int startYear, int stopYear)
{
	string sysFileName;
	sysFileName.assign(*destDirectory);
	cout << log_info << "Write system.dat in " << *destDirectory << endl;
	sysFileName.append("system.dat");
	ofstream *systFile = new ofstream();
	systFile->open(sysFileName.c_str());
	*systFile << "PixiDataPath " << "\t" << *destDirectory << endl;
	string stub = "farm";
	int metfileSep = metfileName->find_last_of("/");
	string climateDirectory = metfileName->substr(0, metfileSep + 1);
	string climateFileName = metfileName->substr(metfileSep + 1);
	for (int i=0;i<theFarms->NumOfNodes(); i++)
	{
		farmInitClass *afarm = theFarms->ElementAtNumber(i);
		string outString;
		outString.assign("[run(");
		char achar[10];

		itoa(afarm->getfarmID(),achar,10);
		string tempString = achar;
		outString.append(tempString);
		outString.append(")]");

		*systFile << outString << endl;
		*systFile << "StartYear " << startYear << endl;
		*systFile << "StopYear " << stopYear << endl;
		*systFile << "EnvironmentalIndicator        1/4" << endl;
		char buf[3];
		sprintf(buf,"%d",i);
		string farmDir = stub + buf;
		*systFile << "InputDirectory  " << afarm->getoutputDirectoryName() << farmDir << directory_separator << endl;
		*systFile << "OutputDirectory "  << *destDirectory << "output" << directory_separator << farmDir << directory_separator <<  endl;
		*systFile << "ClimateDirectory " << "\t" << climateDirectory << endl;
		*systFile << "ClimateFileType 0 " << endl;
		*systFile <<"ClimateFileName " << climateFileName  <<  endl;
		*systFile <<"ForcedOperations        1"<<  endl;
		*systFile <<"VolatModel    1"<<  endl;
		*systFile <<"ForcedHarvest     1"<<  endl;
		*systFile <<"HarvestWaste  0.05"<<  endl;
		*systFile <<"StrawLeft 0.45"<<  endl;
	}
	*systFile << "[end]" << endl;
	systFile->close();
}

void writeFarmdat(string *destDirectory, bool pig, bool dairy, bool beef)
{
	string farmFileName;
	farmFileName.assign(*destDirectory);
	farmFileName.append("//farm.dat");
	ofstream farmfile;
	farmfile.open(farmFileName.c_str());
	farmfile << "[farm]" << endl;

	farmfile << "ScaleBuildings" << "\t" << "1" << endl;
	farmfile << "SellAllStraw" << "\t" << "1" << endl;
	farmfile << "Watering" << "\t" << "0" << endl;
	farmfile << "FarmID	" << "\t" << "0" << endl;
	farmfile << "[livestock]" << endl;
	if (pig)
		farmfile << "pigfarm" << "\t" << "0" << endl;
	else
		farmfile << "pigfarm" << "\t" << "0" << endl;
	if (beef)
		farmfile << "beeffarm" << "\t" << "1" << endl;
	else
		farmfile << "beeffarm" << "\t" << "0" << endl;
	if (dairy)
		farmfile << "dairyfarm" << "\t" << "1" << endl;
	else
		farmfile << "dairyfarm" << "\t" << "0" << endl;
	farmfile << "[end]" << endl;
	farmfile.close();
}

/**
 * Load the configuration file for the dataset (dataset.ini) into:
 *
 * - input directory
 * - output directory
 * - XML file
 * - start year
 * - end year
 * - if 'verbosity', detailed output to console will be produced
 * - meteorological file name
 *
 * Needs glib
 */
void loadFileConf() {
#ifdef unix
	fileAccess hd;
	cout << log_info << "Reading dataset.ini file from "<< hd.getCurrentPath() << endl;
	GKeyFile * p_key_file;
	p_key_file = g_key_file_new();
	GError * p_error = NULL;

	/**
	 * Load the file content to strip the comments ';'.
	 * GLib expects '#' as a comment character.
         */
	gchar *contents;
	gsize length = 0;
	if(!g_file_get_contents("dataset.ini", &contents, &length, &p_error)) {
		g_error("Impossible to load config file : %s\n", p_error->message);
		g_error_free(p_error);
		p_error = NULL;
		exit(EXIT_FAILURE);
	}
	bool in_comment = false;
	bool new_line = true;
	for (unsigned int i=0; i<length; i++) {
		if (new_line) {
			if (contents[i] == ';') {
				in_comment = true;
			} else {
				in_comment = false;
			}
		}
		new_line = (contents[i] == '\n');
		if (in_comment && !new_line) {
			contents[i] = ' ';
		}
	}
	if(!g_key_file_load_from_data(p_key_file,contents,-1,G_KEY_FILE_NONE,&p_error)) {
		g_error("Impossible to load the content : %s\n", p_error->message);
		g_error_free(p_error);
		p_error = NULL;
		g_free(p_key_file);
		exit(EXIT_FAILURE);
	}

	string section;
	string landscapeid = g_key_file_get_string(p_key_file, "simulation", "landscape_id", NULL);
	section = "landuse_" + landscapeid;
	inputDir = hd.getCurrentPath();
	outputDir = inputDir + "/" + g_key_file_get_string(p_key_file, section.c_str(), "fasset_input_dir", NULL);
	inputFileName = "management.xml";
	startYear = g_key_file_get_integer(p_key_file, "simulation", "start_year", NULL);
	endYear = startYear + g_key_file_get_integer(p_key_file, "simulation", "nb_days", NULL) / 365;
	int verbose = g_key_file_get_integer(p_key_file, "simulation", "verbose", NULL);
	verbosity = (verbose == 1);
	section = "meteo_" + landscapeid;
	metfileName = inputDir + "/" + g_key_file_get_string(p_key_file, section.c_str(), "fasset_meteo_file", NULL);
	g_free(p_key_file);
#else
	fileAccess hd;
	hd.changeDir("c:\\Home\\Projects\\ECLAIRE\\Input_files\\inputs");
	inputDir = hd.getCurrentPath();
	startYear=2008;
	endYear=2009;
	outputDir = inputDir+"\\"+"FASSET\\";
	inputFileName = "management.xml";
	metfileName=inputDir+"\\"+"FASSET\\foulum.clm";
	int verbose=1;
	verbosity = (verbose == 1);
	cout << "hardcoded values becouse we are running windows" << endl;
#endif



}

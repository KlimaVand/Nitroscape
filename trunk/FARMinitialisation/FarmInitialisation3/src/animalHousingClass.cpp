/**
 * Class for <lstkbuilding>
 *
 * Contains list of animalSectionClass for the nodes <lstkbuildingsection>.
 */
#include "animalHousingClass.h"
#include "animalSectionClass.h"
#include "manureStorageClass.h"
#include "base/linklist.h"
#include "base/message.h"

//!

animalHousingClass::animalHousingClass ()
{
	type=0;
	planArea=0.0;
	height=0.0;
	ID=0;
	houseNumber=0;
	theSections = new linkList <animalSectionClass>;
};

animalHousingClass:: ~ animalHousingClass()
{
	delete theSections;
};

bool animalHousingClass::readHousing(XMLNode *aNodeHouses)
{
	bool retVal=true;
	XMLNode xNode;

	xNode=aNodeHouses->getChildNode("lstkbuilding_id");
	if (xNode.getText())
		ID=atoi(xNode.getText());
	else
		retVal= false;
	xNode=aNodeHouses->getChildNode("buildingtypeid");
	if (xNode.getText())
		type=atoi(xNode.getText());
	else
	{
		type= 0;
		retVal=false;
	}
	xNode=aNodeHouses->getChildNode("buildingarea");
	if (xNode.getText())
		planArea = atof(xNode.getText());
	else
		planArea = 0.0;
	xNode=aNodeHouses->getChildNode("buildingheight");
	if (xNode.getText())
		height = atof(xNode.getText());
	else
		height = 0.0;
	xNode=aNodeHouses->getChildNode("buildingventilationtypeid");
	if (xNode.getText())
		ventilation = atof(xNode.getText());
	else
		ventilation = 0.0;

	int numsections =aNodeHouses->nChildNode("lstkbuildingsection");
	for (int j=0; j<numsections; j++)
	{
		XMLNode sectionNode=aNodeHouses->getChildNode("lstkbuildingsection",j);
		animalSectionClass *aanimalSectionClass = new animalSectionClass();
		if (!aanimalSectionClass->readAnimalSection(&sectionNode))
		{
			string outString ="Data missing in section ";
			char achar[10];
			itoa(j,achar,10);
			string tempString = achar;
			outString.append(tempString);
			outString.append(" of housing with ID ");
			itoa(ID,achar,10);
			tempString = achar;
			outString.append(tempString);
			theMessage->WarningWithDisplay(outString.c_str());
		}
		theSections->InsertLast(aanimalSectionClass);
	}
	return retVal;
}

int animalHousingClass::getHousingType()
{
	int housingCategory=0;
	for (int i=0; i<theSections->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		int housingType=aanimalSectionClass->findLivestockCategory();
		if ((housingCategory>0)&&(!housingCategory==housingType))
    	{
    		cout << "Warning - two species in one animal house" << endl;
            cout << "housingCategory " << housingCategory << " housingType " << housingType << endl;
    		housingCategory=-999;
  		}
    	else
    		housingCategory=housingType;
	}
	return housingCategory;
}
void animalHousingClass::addPigSpecificSections(int manuretoID,string ManureToName,string houseName,int numberOfSection,ofstream *outfile, string contains)
{
	char num[3];
	string sectionName;
	/*	if (manureName=="SLURRY")
			ManureToName=houseName + ".inhouse";
		else
			ManureToName="Dungheap";*/
	sectionName.assign("[");
		sectionName.append(houseName);
		itoa(numberOfSection, num, 10);
		sectionName.append(".Section(");
		sectionName.append(num);
		sectionName.append(")]");
		*outfile <<sectionName<<endl;
		*outfile << "Alias\tDefault" << endl;
		*outfile << "Contains \t "<<contains << endl;
		sectionName.resize(sectionName.size()-1);
		sectionName.append(".floor(0)]");
		*outfile <<sectionName<<endl;
		*outfile << "Alias\tDefault" << endl;
		*outfile << "ManureToID	\t"<< manuretoID << endl;
		*outfile << "ManureToName	\t" <<houseName<<".inhouse"<<endl;
		*outfile << "["+ ManureToName +"("<<manuretoID<<")]"<< endl;
		*outfile << "Alias	Default" << endl;
		//*outfile << "AreaPartition \t"<<1/6 << endl;
}
void animalHousingClass::writeSection(ofstream *outfile, int i,animalSectionClass* aanimalSectionClass,string houseName,string nameString)
{
	string sectionName;
	char num[3];
	sectionName.assign("[");
	string manureName;
	string ManureToName;
		sectionName.append(houseName);
		itoa(i, num, 10);
		sectionName.append(".Section(");
		sectionName.append(num);
		sectionName.append(")]");
		*outfile << sectionName << endl;
		sectionName.resize(sectionName.length()-1);// remove "]"
		
		manureStorageClass * temp = new manureStorageClass();
                temp->setmanureType(aanimalSectionClass->getmanureType());
		temp->getManureName(&manureName);
	    if (manureName.find("SLURRY")!= string::npos)
			ManureToName=houseName + ".inhouse";
		else
			ManureToName="Dungheap";
		if (aanimalSectionClass->writeSectionToBuildingsFile(ManureToName, outfile,sectionName,nameString) != 0) {
			string outString = "Problem in building ";
			itoa(ID, num, 10);
			outString.append(num);
			theMessage->WarningWithDisplay(outString.c_str());
		}
		if (manureName.find("SLURRY")!= string::npos)
		{
				//*outfile << floorname << endl;
				sectionName.assign("[");
				sectionName.append(houseName);
				sectionName.append(".inhouse(0)]");
				*outfile <<sectionName<< endl;
				*outfile << "Alias" << "\t"	<< "Default"  << endl;
				*outfile << "ManureStoreID" << "\t"	<<aanimalSectionClass->getmanureDestination()<< endl;
				*outfile << "ManureToName" << "\t"	<<"Slurrystore"<< endl;
		}
}
void animalHousingClass::writeHousingToBuildingsFile(ofstream *outfile, string houseName)
{
	*outfile << "Alias\tDefault" << endl;
	*outfile << "ID" << "\t"	<< ID << endl;
	*outfile << "Area" << "\t"	<< planArea << endl;
	*outfile << "height" << "\t"	<< height << endl;
	if (ventilation==1)
		*outfile << "controlledVent" << "\t" << "0" << endl;
	else
		*outfile << "controlledVent" << "\t" << "1" << endl;
	string sectionName;
	char num[3];
	//int manuretoID=0;
	string ManureToName;
	string manureName;
	string nameString;
	
	bool found=false;
	int i;
	for (i=0; i<theSections->NumOfNodes();i++)
	{
		string nameString;
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		if (aanimalSectionClass->getLivestockName(&nameString)!=0)
		{
		char message[100];
		theMessage->WarningWithDisplay("The section ", sectionName, " contains no correct livestock");
		theMessage->WarningWithDisplay(message);
		exit(2);
		}
		if(nameString.compare("SOWS")==0)
		{
			found=true;
			break;
		}
		
	}
	if(found==true)
	{
		writeSection(outfile, theSections->NumOfNodes()+1,theSections->ElementAtNumber(i), houseName, "Mating(0)");
		writeSection(outfile, theSections->NumOfNodes()+2,theSections->ElementAtNumber(i), houseName, "Pregnant(0)");
		writeSection(outfile, theSections->NumOfNodes()+3,theSections->ElementAtNumber(i), houseName, "Lactating(0)");
		writeSection(outfile, theSections->NumOfNodes()+4,theSections->ElementAtNumber(i), houseName, "Weaner(0)");
		writeSection(outfile, theSections->NumOfNodes()+5,theSections->ElementAtNumber(i), houseName, "Finisher(0)");
		writeSection(outfile, theSections->NumOfNodes()+6,theSections->ElementAtNumber(i), houseName, "NewSow(0)");
	}
	if(found==false)
	{
		for ( i=0; i<theSections->NumOfNodes();i++)
		{
			string nameString;
			animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
			if (aanimalSectionClass->getLivestockName(&nameString)!=0)
			{
			char message[100];
			theMessage->WarningWithDisplay("The section ", sectionName, " contains no correct livestock");
			theMessage->WarningWithDisplay(message);
			exit(2);
			}
			if(nameString.compare("Weaner")!=0||nameString.compare("Finisher")!=0)
			{
				writeSection(outfile, i,aanimalSectionClass, houseName, nameString);
			}
			//manuretoID=aanimalSectionClass->getmanureDestination();
	}
	}

/*	if(houseName.find("PigHousing")==0)
	{
				//animalSectionClass *aanimalSectionClass = new animalSectionClass();

		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes(), outfile,"Mating(0)");
		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes()+1, outfile,"Pregnant(0)");
		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes()+2, outfile,"Lactating(0)");
		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes()+3, outfile,"Weaner(0)");
		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes()+4, outfile,"Finisher(0)");
		addPigSpecificSections(manuretoID,ManureToName,houseName,theSections->NumOfNodes()+5, outfile,"NewSow(0)");
	}*/

}

void animalHousingClass::CheckSections()
{
	linkList <animalSectionClass> *sectionsToClone = new linkList <animalSectionClass>;
	int baseLivestockType=0;
	int livestockTypesToClone[30];
	for (int i=0; i<30; i++)
		livestockTypesToClone[i]=0;
	int numlivestockTypesToClone=0;
	for (int i=0; i<theSections->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		int numSectionRecords = aanimalSectionClass->getNumberSectionRecords();
		for (int j=0; j<numSectionRecords; j++)
		{
			sectionRecordClass *asectionRecord = aanimalSectionClass->getTheSectionRecords()->ElementAtNumber(j);
			if (j==0)
			{
				baseLivestockType =asectionRecord->getlivestockType();
				if (baseLivestockType==1)
				{
					baseLivestockType=0;
					asectionRecord->setlivestockType(0);
				}
			}
			else
			{
				int tempLivestockType=asectionRecord->getlivestockType();
				if (tempLivestockType==1)
				{
					tempLivestockType=0;
					asectionRecord->setlivestockType(0);
				}
				if (tempLivestockType!=baseLivestockType)
				{
					bool doCloning=true;
					if (numlivestockTypesToClone>0)  //check if already have this livestock type
					{
						for (int k=0; k<numlivestockTypesToClone; k++)
							if (livestockTypesToClone[k]==tempLivestockType)
								doCloning = false;
					}
					if (doCloning)
					{
						sectionsToClone->InsertLast(aanimalSectionClass);
						livestockTypesToClone[numlivestockTypesToClone]=tempLivestockType;
						numlivestockTypesToClone++;
					}				
				}
			}
		}
	}
	cout << log_info << "Number of animal sections: " << theSections->NumOfNodes() << endl;
	cout << log_info << "Number of animal sections to clone: " << sectionsToClone->NumOfNodes() << endl;
	//create clones and add to the list of sections
	for (int i=0; i<sectionsToClone->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = sectionsToClone->ElementAtNumber(i);
		animalSectionClass *animalSectionClone = new animalSectionClass(aanimalSectionClass, livestockTypesToClone[i]);
		theSections->InsertLast(animalSectionClone);
	}
	//
	cout << log_info << "Number of animal sections: " << theSections->NumOfNodes() << endl;
	for (int i=0; i<theSections->NumOfNodes();i++)
	{
		animalSectionClass *aanimalSectionClass = theSections->ElementAtNumber(i);
		int numSectionRecords = aanimalSectionClass->getNumberSectionRecords();
		cout << log_info << "Number of animal section records: " << numSectionRecords << endl;
		int j=0;
		while (j<aanimalSectionClass->getNumberSectionRecords())
		{
			sectionRecordClass *asectionRecord = aanimalSectionClass->getTheSectionRecords()->ElementAtNumber(j);
			if (!asectionRecord)
			{
				cout << log_warning << "section #" << i << ", section record #" << j << " is NULL!" <<endl;
				continue;
			}
			if (j==0)
			{
				baseLivestockType =asectionRecord->getlivestockType();
				j++;
				if (baseLivestockType==1)
				{
					baseLivestockType=0;
					asectionRecord->setlivestockType(0);
				}
			}

			else //need to consider cloning because more than one livestock type in a section. If both are DAIRY, equate small dairy to large dairy. Fudge due to error in database design
				{
					int thisLivestockType = asectionRecord->getlivestockType();
					if (baseLivestockType !=thisLivestockType)
					{
							linkList <sectionRecordClass>::PS aNode;
							aanimalSectionClass->getTheSectionRecords()->SearchByNumber(aNode,j);
							aanimalSectionClass->getTheSectionRecords()->Release(aNode);
					}
					else
						j++;

				}
		}
	}
}




// Class: NCUCropClass                     //ANSI C++

#include "base\common.h"
#include "base\linklist.h"

#ifndef __NCUCROPCLASS_H  //Required for current class
  #define __NCUCROPCLASS_H



#ifdef __BCplusplus__
	#include <strstream>
#else
	#include <sstream>
#endif
const int numberManureTypes = 8;
const int numberFertTypes = 2;

class NCUCropClass
{

     string CropName; //Attribute data member

     double UreaFertiliser; //Attribute data member

     double NitrateFertiliser; //Attribute data member

     double CattleSlurry; //Attribute data member

     double CattleSolid; //Attribute data member

     double PigSlurry; //Attribute data member

     double PigSolid; //Attribute data member

     double PoultrySlurry; //Attribute data member

     double PoultrySolid; //Attribute data member

     double Grazing; //Attribute data member

     double CattleSlurryC; //Attribute data member

     double CattleSolidC; //Attribute data member

     double PigSlurryC; //Attribute data member

     double PigSolidC; //Attribute data member

     double PoultrySlurryC; //Attribute data member

     double PoultrySolidC; //Attribute data member

     double GrazingC; //Attribute data member

     double AtmosphericNdep;


     int SowingDay[5];
     int HarvestingDay[10];
     //!
     int PloughingDay[5];
     int HarrowingDay[5];
     //!Manure application dates; first index - 0= Cattle solid, 1=cattle slurry, 2=pig solid, 3=pig slurry, 4=poultry solid, 5=poultry slurry, 6=grazing
     //!second index - n = application times in order
     int ManureDay[numberManureTypes][4];
     //!Fertiliser application dates; 0= urea, 1=nitrate
     int FertDay[numberFertTypes][4];
     //!Day when grazing starts
     int GrazingStartDay[5];
     //!Day when grazing stops
     int GrazingStopDay[5];
     //!Day when cutting occurs
     int CuttingDay[5];
     //!true if is winter crop
     bool WinterCrop;
     //!proportion of mineral fertiliser applied at each application time
     double propFert[2];

     int numLeadSpaces;

private:

protected:

public:

			//Default constructor
  NCUCropClass () {};

   void Initialise ()    ;

   ~ NCUCropClass ( );  //Destructor - Delete any pointer data members that used new in constructors


                       //!Get accessor function for non-static attribute data member
  double getNitrateFertiliser() const  { return NitrateFertiliser;}
                       //!Get accessor function for non-static attribute data member
  double getUreaFertiliser() const  { return UreaFertiliser;}

                       //!Get accessor function for non-static attribute data member
  double getCattleSlurry() const  { return CattleSlurry;}

                       //!Get accessor function for non-static attribute data member
  double getCattleSolid() const  { return CattleSolid;}

                       //!Get accessor function for non-static attribute data member
  double getPigSlurry() const  { return PigSlurry;}

                       //!Get accessor function for non-static attribute data member
  double getPigSolid() const  { return PigSolid;}

                       //!Get accessor function for non-static attribute data member
  double getPoultrySlurry() const  { return PoultrySlurry;}

                       //!Get accessor function for non-static attribute data member
  double getPoultrySolid() const  { return PoultrySolid;}

                       //!Get accessor function for non-static attribute data member
  double getGrazing() const  { return Grazing;}

  double getCattleSlurryC() const  { return CattleSlurryC;}

                       //!Get accessor function for non-static attribute data member
  double getCattleSolidC() const  { return CattleSolidC;}

                       //!Get accessor function for non-static attribute data member
  double getPigSlurryC() const  { return PigSlurryC;}

                       //!Get accessor function for non-static attribute data member
  double getPigSolidC() const  { return PigSolidC;}

                       //!Get accessor function for non-static attribute data member
  double getPoultrySlurryC() const  { return PoultrySlurryC;}

                       //!Get accessor function for non-static attribute data member
  double getPoultrySolidC() const  { return PoultrySolidC;}

                       //!Get accessor function for non-static attribute data member
  double getGrazingC() const  { return GrazingC;}

	double getAtmosphericNdep(){return AtmosphericNdep;}


                       //!Get accessor function for non-static attribute data member
  string getCropName() const  { return CropName;}

                       //!Get accessor function for non-static attribute data member

   int getSowingDay(int index){return SowingDay[index];}
   int getHarvestingDay(int index){return HarvestingDay[index];}
   bool getWinterCrop(){return WinterCrop;};
   int getPloughingDay(int index) {return PloughingDay[index];}
   int getHarrowingDay(int index) {return HarrowingDay[index];}
   int getManureDay(int index, int appNo) {return ManureDay[index][appNo];}
   int getFertDay(int index, int appNo) {return FertDay[index][appNo];}
   int getGrazingStartDay(int index) {return GrazingStartDay[index];}
   int getGrazingStopDay(int index) {return GrazingStopDay[index];}
   int getCuttingDay(int index) {return CuttingDay[index];}


                       //!Set accessor function for non-static attribute data member
  void setNitrateFertiliser (double aNitrateFertiliser)  { NitrateFertiliser = aNitrateFertiliser;}
                       //!Set accessor function for non-static attribute data member
  void setUreaFertiliser (double aUreaFertiliser)  { UreaFertiliser = aUreaFertiliser;}

                       //!Set accessor function for non-static attribute data member
  void setCattleSlurry (double aCattleSlurry)  { CattleSlurry = aCattleSlurry;}

                       //!Set accessor function for non-static attribute data member
  void setCattleSolid (double aCattleSolid)  { CattleSolid = aCattleSolid;}

                       //!Set accessor function for non-static attribute data member
  void setPigSlurry (double aPigSlurry)  { PigSlurry = aPigSlurry;}

                       //!Set accessor function for non-static attribute data member
  void setPigSolid (double aPigSolid)  { PigSolid = aPigSolid;}

                       //!Set accessor function for non-static attribute data member
  void setPoultrySlurry (double aPoultrySlurry)  { PoultrySlurry = aPoultrySlurry;}

                       //!Set accessor function for non-static attribute data member
  void setPoultrySolid (double aPoultrySolid)  { PoultrySolid = aPoultrySolid;}

                       //!Set accessor function for non-static attribute data member
  void setGrazing (double aGrazing)  { Grazing = aGrazing;}

                       //!Set accessor function for non-static attribute data member
  void setCattleSlurryC (double aCattleSlurryC)  { CattleSlurryC = aCattleSlurryC;}

                       //!Set accessor function for non-static attribute data member
  void setCattleSolidC (double aCattleSolidC)  { CattleSolidC = aCattleSolidC;}

                       //!Set accessor function for non-static attribute data member
  void setPigSlurryC (double aPigSlurryC)  { PigSlurryC = aPigSlurryC;}

                       //!Set accessor function for non-static attribute data member
  void setPigSolidC (double aPigSolidC)  { PigSolidC = aPigSolidC;}

                       //!Set accessor function for non-static attribute data member
  void setPoultrySlurryC (double aPoultrySlurryC)  { PoultrySlurryC = aPoultrySlurryC;}

                       //!Set accessor function for non-static attribute data member
  void setPoultrySolidC (double aPoultrySolidC)  { PoultrySolidC = aPoultrySolidC;}

                       //!Set accessor function for non-static attribute data member
  void setGrazingC (double aGrazingC)  { GrazingC = aGrazingC;}

  void setAtmosphericNdep(double aVal) {AtmosphericNdep = aVal;}

                       //!Set accessor function for non-static attribute data member
  void setCropName (string aCropName)  { CropName = aCropName;}



	void setSowingDay(int aVal, int index) {SowingDay[index]=aVal;}
   void setHarvestingDay(int aVal, int index) {HarvestingDay[index]=aVal;}
   void setWinterCrop(bool aVal){WinterCrop = aVal;}
	void setPloughingDay(int aVal, int index){PloughingDay[index]=aVal;}
	void setHarrowingDay(int aVal, int index){HarrowingDay[index]=aVal;}
	void setManureDay(int aVal, int index, int appNo){ManureDay[index][appNo]=aVal;}
	void setFertDay(int aVal, int index, int appNo){FertDay[index][appNo]=aVal;}
	void setGrazingStartDay(int aVal, int index){GrazingStartDay[index]=aVal;}
	void setGrazingStopDay(int aVal, int index){GrazingStopDay[index]=aVal;}
	void setCuttingDay(int aVal, int index){CuttingDay[index]=aVal;}
	int getNumManureApps(int index);
	int getNumFertApps(int index);
	double getpropFert(int index){return propFert[index];}
#ifdef __BCplusplus__
	bool ReadData(istrstream *instring, int row);
#else
	  bool ReadData(stringstream *instring, int row, bool first);
#endif


};

/*
1 - tillage
2 - sowing
3 - fertilisation
4 - cutting
5 - start grazing
6 - end grazing
7 - start irrigation
8 - stop irrigation
9 - harvesting
*/
#endif

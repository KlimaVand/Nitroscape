
#ifndef CROPPARAMETERSDATA_H_
#define CROPPARAMETERSDATA_H_
#include <vector>
#include "commonData.h"
class beetData
{
	public: beetData();
	string
		cropName;
	double
	TempSumStorage1,               			// Temperature sum for min allocation to storage
	TempSumStorage2,             				// Temperature sum for max allocation to storage
	MinAllocToStorage,          				// Min allocation to storage organs
	MaxAllocToStorage;
};
class cloverData
{
	public: cloverData();
	string
		cropName;
	double
	NFixationThisDay;
};
class ryeData
{
	public: ryeData();
	string
		cropName;
	double
	HeightA,                   // See Olesen, 2002 for description. Value taken from winter wheat
	HeightB;
};
class ryeGrassData
{
	public: ryeGrassData();
	string
		cropName;
double
	MinDayLength, //
	BulkDensity,//
	RespirationTop,//
	RespirationRoot,//
	TransferRate,//
	RecycleFraction,//
	InitialLAIIncrease,//
	liveOMD,//
	deadOMD,//
	RateOfRipeReSeed;
};
class generalData
{
public: generalData();
string
	cropName;
double

// PARAMETERS
	      DMCostPerN,  //Needed for external crop parameterisation of pea
	      MaxFixPerDM, //Needed for external crop parameterisation of pea
	      AmmoniumUptakeRate,
	      CoeffGreenIdx,
	      colonialisationRate,
	      Conversfactor,
	      ConversionCoefficient,
	      DecayRoot,
	      FillFactor,
	      GerminationConversion,
	      InitialCAI,
	      InitialRootDepth,
	      InterceptCoeff,
	      k,
	      LAIDistributionPar,
	      LAIDMRatio,
	      LAINitrogenRatio,
      MaxGLAICrop,
	      MaxAllocToRoot,
	      MaxDMtemp,
	      MaxN_Root,
	      MaxN_Store,
	      MaxPlantHeight,
	      MaxRadUseEff,
	      MinAllocToRoot,
	      MinDMtemp,
	      MinimumRootDens,
	      MinimumSenescense,
	      MinimumSoilAmmonium,
	      MinimumSoilNitrate,
	      MinN_Root,
	      MinN_Store,
	      NCurveParam,
	      NitrateUptakeRate,
	      NPctMax,
	      NPctMin,
	      PhotoSyntActPart,
	      PowDM,
	      ReducedNInYellow,
	      RhizoDepositFraction,
	      RootDensAtBottom,
	      RootPentrRate,
      RootDistrParm,
	      RootpF,
	      SpecificRootLength,
	      StoreForFilling,
	      TempRootMin,
	      TopFraction,
	      WaterDeficitGrainFill,
	      WaterDeficitLagPhase,
	      WaterDeficitVegGrowth,
	      FractionNminInRoots,
	      FractionNminInVegTop,
	      CO2Effect;


		double // STATE VARIABLES

	      MaxRootDepth,
      RelativeDensity;                    // Relative to standard sowing density

double
	DS_Flagligule,
	   	//! Developing System scale for start of grainfill (is it a scale??
	    DS_StartFill,
	  	//!  Temperature sum from sowing to emergence. Temperature is added when it is above TB0
	    TS0,
	    //! Temperature sum from emergence to Anthesis. Temperature is added when it is above TB0 Anthesis is when the flower is fully open and functional.
	    TS1,
	    //! Temperature sum from Anthesis to grain fill. Temperature is added when it is above TB1
	    TS2,
	    //! Temperature sum from grain fillto  ripeness Temperature is added when it is above TB2
	    TS3,
	    //! Base temperature until emergence
	   	TB0,
	   	//! Base temperature until flowering
	    TB1,
	    //! Base temperature after flowering
	    TB2,
	    //! Base temperature for ripening
	   	TB3,
	   	//! Daylength. The min daylight length where a plan can develop. Only used in winter times
	    DB,
	    //! Daylength. The max daylight length where a plan can developOnly used in winter times
	    DMAX,
	    //! Growth phase for linear leaf growth (related to TempSumForLeaf!)
	    LinearLeafPhase;

};
class cropParametersData:commonData{



protected :
    int placeInBaseVector;
    int placeInBeetVector;
    int placeInRyeGrassVector;
    int placeInCcoverVector;
    int placeInRyeVector;
    vector<generalData> data;
    vector<beetData> beet;
    vector<ryeGrassData> ryeGrass;
    vector<cloverData> clover;
    vector<ryeData> rye;
	void processing(bool primaryFile);
    void loaddata();
    void checkData();
public:
/*
    double getTempSumStorage1(){};               			// Temperature sum for min allocation to storage
    double getTempSumStorage2,             				// Temperature sum for max allocation to storage
    double getMinAllocToStorage,          				// Min allocation to storage organs
    double getMaxAllocToStorage;
*/
	void setCrop(string CropName);
	double getDMCostPerN(){return data[placeInBaseVector].DMCostPerN;}
	double getMaxFixPerDM(){return data[placeInBaseVector].MaxFixPerDM;}
	double getAmmoniumUptakeRate(){return data[placeInBaseVector].AmmoniumUptakeRate;}
	double getCoeffGreenIdx(){return data[placeInBaseVector].CoeffGreenIdx;}

	double getConversfactor(){return data[placeInBaseVector].Conversfactor;}
	double getConversionCoefficient(){return data[placeInBaseVector].ConversionCoefficient;}
	double getDecayRoot(){return data[placeInBaseVector].DecayRoot;}
	double getFillFactor(){return data[placeInBaseVector].FillFactor;}
	double getGerminationConversion(){return data[placeInBaseVector].GerminationConversion;}
	double getInitialCAI(){return data[placeInBaseVector].InitialCAI;}
	double getInitialRootDepth(){return data[placeInBaseVector].InitialRootDepth;}
	double getInterceptCoeff(){return data[placeInBaseVector].InterceptCoeff;}
	double getk(){return data[placeInBaseVector].k;}
	double getLAIDistributionPar(){return data[placeInBaseVector].LAIDistributionPar;}
	double getLAIDMRatio(){return data[placeInBaseVector].LAIDMRatio;}
	double getLAINitrogenRatio(){return data[placeInBaseVector].LAINitrogenRatio;}
	double getMaxGLAICrop(){return data[placeInBaseVector].MaxGLAICrop;}
	double getMaxAllocToRoot(){return data[placeInBaseVector].MaxAllocToRoot;}
	double getMaxDMtemp(){return data[placeInBaseVector].MaxDMtemp;}
	double getMaxN_Root(){return data[placeInBaseVector].MaxN_Store;}
	double getMaxN_Store(){return data[placeInBaseVector].MaxN_Store;}
	double getMaxPlantHeight(){return data[placeInBaseVector].MaxPlantHeight;}
	double getMaxRadUseEff(){return data[placeInBaseVector].MaxRadUseEff;}
	double getMinAllocToRoot(){return data[placeInBaseVector].MinAllocToRoot;}
	double getMinDMtemp(){return data[placeInBaseVector].MinDMtemp;}
	double getMinimumRootDens(){return data[placeInBaseVector].MinimumRootDens;}
	double getMinimumSenescense(){return data[placeInBaseVector].MinimumSenescense;}
	double getMinimumSoilAmmonium(){return data[placeInBaseVector].MinimumSoilAmmonium;}
	double getMinimumSoilNitrate(){return data[placeInBaseVector].MinimumSoilNitrate;}
	double getMinN_Root(){return data[placeInBaseVector].MinN_Root;}
	double getMinN_Store(){return data[placeInBaseVector].MinN_Store;}
	double getNCurveParam(){return data[placeInBaseVector].NCurveParam;}
	double getNitrateUptakeRate(){return data[placeInBaseVector].NitrateUptakeRate;}
	double getNPctMax(){return data[placeInBaseVector].NPctMax;}
	double getNPctMin(){return data[placeInBaseVector].NPctMin;}
	double getPhotoSyntActPart(){return data[placeInBaseVector].PhotoSyntActPart;}
	double getPowDM(){return data[placeInBaseVector].PowDM;}
	double getReducedNInYellow(){return data[placeInBaseVector].ReducedNInYellow;}
	double getRhizoDepositFraction(){return data[placeInBaseVector].RhizoDepositFraction;}
	double getRootDensAtBottom(){return data[placeInBaseVector].RootDensAtBottom;}
	double getRootPentrRate(){return data[placeInBaseVector].RootPentrRate;}
	double getRootDistrParm(){return data[placeInBaseVector].RootDistrParm;}
	double getRootpF(){return data[placeInBaseVector].RootpF;}
	double getSpecificRootLength(){return data[placeInBaseVector].SpecificRootLength;}
	double getStoreForFilling(){return data[placeInBaseVector].StoreForFilling;}
	double getTempRootMin(){return data[placeInBaseVector].TempRootMin;}
	double getTopFraction(){return data[placeInBaseVector].TopFraction;}
	double getWaterDeficitGrainFill(){return data[placeInBaseVector].WaterDeficitGrainFill;}
	double getWaterDeficitLagPhase(){return data[placeInBaseVector].WaterDeficitLagPhase;}
	double getWaterDeficitVegGrowth(){return data[placeInBaseVector].WaterDeficitVegGrowth;}
	double getFractionNminInRoots(){return data[placeInBaseVector].FractionNminInRoots;}
	double getFractionNminInVegTop(){return data[placeInBaseVector].FractionNminInVegTop;}
	double getCO2Effect(){return data[placeInBaseVector].CO2Effect;}
	double getMaxRootDepth(){return data[placeInBaseVector].MaxRootDepth;}
	double getRelativeDensity(){return data[placeInBaseVector].RelativeDensity;}
	double getLinearLeafPhase(){return data[placeInBaseVector].LinearLeafPhase;}
	double getDS_Flagligule(){return data[placeInBaseVector].DS_Flagligule;}
	double getDS_StartFill(){return data[placeInBaseVector].DS_StartFill;}
	double getTS0(){return data[placeInBaseVector].TS0;}
	double getTS1(){return data[placeInBaseVector].TS1;}
	double getTS2(){return data[placeInBaseVector].TS2;}
	double getTS3(){return data[placeInBaseVector].TS3;}
	double getTB0(){return data[placeInBaseVector].TB0;}
	double getTB1(){return data[placeInBaseVector].TB1;}
	double getTB2(){return data[placeInBaseVector].TB2;}
	double getTB3(){return data[placeInBaseVector].TB3;}
	double getDB(){return data[placeInBaseVector].DB;}

	double getDMAX(){return data[placeInBaseVector].LinearLeafPhase;};
    cropParametersData(){
    	cout<<"Loading cropData data"<<endl;
    	loaddata();
    	checkData();


    	cout<<"Done with loading and cheking cropData data"<<endl;
    }
};
extern cropParametersData * theCropData;
#endif /* CROPPARAMETERSDATA_H_ */

// Class: NCUClass                     //ANSI C++

#include "base\common.h"
#include "base\linklist.h"

#ifndef __NCUCLASS_H  //Required for current class
  #define __NCUCLASS_H

                              //Required for 1:M aggregation (part) classes
#ifndef __CROPROTCLASS_H
  #include "cropRotationClass.h"
#endif

#ifndef __CROPSERIESCLASS_H
	#include "cropSeriesClass.h"
#endif

//const int numberSeries= 8;

class NCUClass      
{                       
	//! NCU identifier code (includes NUTS coed)
     int NCUI; //Attribute data member
     //! NCU number
     int NCU;
     // MARS grid code number
     int MARS;
     // Date of sowing spring barley; used to define start of growing season
     int springBarleySowing;
     // Date of sowing winter wheat; used to define end of growing season
     int winterWheatSowing;
     //!list of crops grown in a given year
   linkList<cropRotationClass> *theYearPlans; //1:M aggregation part data member
   //!plans for each 'field' for each year
   linkList<cropSeriesClass> *theSeriesOperations; //1:M aggregation part data member
   //! 0 = mean temperature, Celsius; 1=max temperature, Celsius; 2=precipitation (mm); 3=vapour pressure; 4=global radiation, MJ/m**2; 5=potential evapotranspiration (mm)
   double metdata[50][366][6];
   //! list of crops for which there are no sowing and harvesting dates, and equivalent crop to use
   string replacements [50][2];
   //! method of slurry application
	int slurryApplicMethod;
	//! method of applying solid manure
	int solidApplicMethod;
	//! method of applying urea
	int ureaApplicMethod;
	//! is set to true if catch cropping should be used
	bool coverCropping;
	//! is set to true if using zero tillage
	bool zeroTillage;
	double N_added;
	int maxFields;

public:

			//Default constructor
  NCUClass ();

   ~ NCUClass ( );  //Destructor - Delete any pointer data members that used new in constructors

   void Initialise(string inputDir, int thenumFields);

   int ReadData(int *aNCU, int startDay,  int startYear,  int endDay,  int endYear,  int baseYear, int range,
		string metdataInputDir, string tpvStem, string radStem, string ncu_mars, string inputDir, fstream * rotationFile,
      bool bootstrap);

   void ReadCropSequence(int cropnum, int startCol, fstream * rotationFile);

   void calcCropManagement(string inputDir, string outputDir,int startYear);

   int getNCUI() const  { return NCUI;}

                       //!Set accessor function for non-static attribute data member
   void setNCUI (int aNCUI)  { NCUI = aNCUI;}

                       //!Set accessor function for non-static attribute data member
   void setMARS (int aMARS)  { MARS = aMARS;}

                       //!Get accessor function for non-static attribute data member
   int getNCU() const  { return NCU;}

                       //!Set accessor function for non-static attribute data member
   void setNCU (int aNCU)  { NCUI = aNCU;}

   void setspringBarleySowing(int aVal){springBarleySowing=aVal;}

  //!Return a list of pointers to the crop rotations
  linkList <cropRotationClass>* gettheYearPlans()  { return theYearPlans;  }
  //!Return the mean daily temperature (Celsius) for a particular in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getmeanDailyTemp(int yearNo, int dayNo) {return metdata[yearNo][dayNo][0];  }
  //!Return the maximum daily temperature (Celsius) for a particular day in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getmaxDailyTemp(int yearNo, int dayNo) {return metdata[yearNo][dayNo][1];  }
  //!Return the daily precipitation (mm) for a particular day in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getDailyPrecip(int yearNo, int dayNo) {return metdata[yearNo][dayNo][2];  }
  //!Return the mean daily vapour pressure for a particular day in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getDailyVapourPress(int yearNo, int dayNo) {return metdata[yearNo][dayNo][3];  }
  //!Return the mean daily solar radiation (MJ/m**2) for a particular day in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getDailyRadiation(int yearNo, int dayNo) {return metdata[yearNo][dayNo][4];  }
  //!Return the mean daily potential evapotranspiration for a particular day in a particular year
  /*!
   * \param yearNo the number of the year (counting from startYear)
   * \param dayNo the number of the day
   */
  double getDailyPotEvapTrans(int yearNo, int dayNo) {return metdata[yearNo][dayNo][5];  }
  //!
  int getCropEventDate(string inputDir, string MARSDateFileName, int MARScode, int dateID, bool recursive,int year);
  int getspringBarleySowing(){return springBarleySowing;}
  //!Returns the average date within a NUTS region on which an operation is performed
  /*!
   * \param inputDir name of input directory
   * \param MARSDateFileName name of the MARS data file
   * \param dateID 1 = sowing, 2 = harvesting
   */
  int getAverageCropEventDate(string inputDir, string MARSDateFileName, int dateID, int year);
  //!Obsolete
  void OutputData(fstream *afile);
  //!Generate the field operations
  void generateOps();
  //!Output the field operations to text file
  /*!
   * \param outfile pointer to output file
   */
  void outputOps(fstream * outfile);
  //!Load the list of replacement crop names
  /*!
   * \param inputDir name of input directory
   */
  void loadReplacements(string inputDir);
  //!Replace any crops for which data is not available with the name of a crop that is available
  /*!
   * \param aCrop name of the crop for which a replacementis required
   */
    void getReplacements(string *aCrop);
    //!Return the annual precipitation for a particular year
    /*!
     * \param yearNo the number of the year (counting from startYear)
     */
  double getAnnualPrecip(int yearNo);
  //!Return annual meteorological data for a particular year
  /*!
   * \param variableNo 0 = mean temperature, Celsius; 1=max temperature, Celsius; 2=precipitation (mm); 3=vapour pressure; 4=global radiation, MJ/m**2; 5=potential evapotranspiration (mm)
   * \param yearNo the number of the year (counting from startYear)
   */
  double getAnnualMet(int variableNo, int yearNo);
  //!Write the annual meteorological data to file
  /*!
   * \param outfile pointer to output file
   */
  void writeAnnualMet(fstream *outfileMet);
  //!Generate all field operations to be performed in the spring and summer for the current crop
  void doCurrentSpringSummerCropOperations(cropSeriesClass * acropSeriesClass,NCUCropClass *aNCUCropClass, int calendarYear);
  //! generate field operations to be performed in the autumn for the next (winter) crop
  void doNextCropAutumnOperations(cropSeriesClass * acropSeriesClass,NCUCropClass *nextCropClass, int calendarYear);
  int getslurryApplicMethod(){return slurryApplicMethod;}
  int getsolidApplicMethod(){return solidApplicMethod;}
  int getureaApplicMethod() {return ureaApplicMethod;}
  bool setcoverCropping(bool aVal){coverCropping = aVal;}
  bool setzeroTillage(bool aVal){zeroTillage=aVal;}
  void doSolidManureOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear);
  void doSlurryOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear);
  void doFertiliserOperations(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear);

  void AddN(double amount){N_added+=amount;};
  bool CheckNBalance();
  int GetmaxFields(){return maxFields;}
  void SetmaxFields(int aVal){maxFields=aVal;}
  void doZeroTillage(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, NCUCropClass *nextCrop, int calendarYear);
  void doCoverCrop(cropSeriesClass * acropSeriesClass, NCUCropClass *aNCUCropClass, int calendarYear);

};

#endif

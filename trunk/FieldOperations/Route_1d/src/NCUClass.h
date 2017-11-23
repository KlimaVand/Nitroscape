// Class: NCUClass                     //ANSI C++

#include "common.h"
#include "linklist.h"

#ifndef __NCUCLASS_H  //Required for current class
  #define __NCUCLASS_H

                              //Required for 1:M aggregation (part) classes
#ifndef __CROPROTCLASS_H
  #include "cropRotationClass.h"
#endif

#ifndef __CROPSERIESCLASS_H
	#include "cropSeriesClass.h"
#endif

const int numberSeries= 8;

class NCUClass      
{                       
			
     int NCUI; //Attribute data member
     int NCU;
     int MARS;
     int springBarleySowing;

   linkList<cropRotationClass> *theYearPlans; //1:M aggregation part data member
   linkList<cropSeriesClass> *theSeriesOperations; //1:M aggregation part data member
   //! 0 = mean temperature, Celsius; 1=max temperature, Celsius; 2=precipitation (mm); 3=vapour pressure; 4=global radiation, MJ/m**2; 5=potential evapotranspiration (mm)
   double metdata[50][366][6];
   string replacements [50][2];


public:

			//Default constructor
  NCUClass ();

   ~ NCUClass ( );  //Destructor - Delete any pointer data members that used new in constructors

	void Initialise(char *inputDir);

	int ReadData(int *aNCU, int startDay,  int startYear,  int endDay,  int endYear,  int baseYear, int range,
		char *metdataInputDir, char *tpvStem, char *radStem, char *ncu_mars, char *inputDir, fstream * rotationFile,
      bool bootstrap);

	void ReadCropSequence(int cropnum, int startCol, fstream * rotationFile);

   void calcCropManagement(char *inputDir, char *outputDir);

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
  int getCropEventDate(char inputDir[256], char MARSDateFileName[50], int MARScode, int dateID, bool recursive);
  int getspringBarleySowing(){return springBarleySowing;}
  //!Returns the average date within a NUTS region on which an operation is performed
  /*!
   * \param inputDir name of input directory
   * \param MARSDateFileName name of the MARS data file
   * \param dateID 1 = sowing, 2 = harvesting
   */
  int getAverageCropEventDate(char inputDir[256], char MARSDateFileName[50], int dateID);
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
  void loadReplacements(char inputDir[256]);
  //!Replace any crops for which data is not available with the name of a crop that is available
  /*!
   * \param aCrop name of the crop for which a replacement is required
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
};

#endif

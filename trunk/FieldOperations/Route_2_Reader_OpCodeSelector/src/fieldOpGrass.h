// Class: fieldOpGrass                     //ANSI C++

#include "base/common.h"

#ifndef __FIELDOPGRASS_H  //Required for current class
  #define __FIELDOPGRASS_H

                               //Required for base classes
#ifndef __FIELDOP_H
  #include "fieldOp.h"
#endif


//! Describes a grazing operation

class fieldOpGrass  : public fieldOp        
{
			
  //! Leaf area index of the crop after harvesting		
     double harvLAI;

//! Grazing animal type; 1=dairy, 2=other cattle, 3=sheep
     int grazingType;

private:

protected:

public:

     fieldOpGrass () {};
     //Constructor with argument
     /*!
     \param anOpCode code for operation (5 = start grazing, 6 = stop grazing)
     \param theGrazingType the type of animal grazing
     */
     fieldOpGrass (int theGrazingType, int anOpCode);

     //Constructor with argument
     /*!
     \param anOpCode code for operation (5 = start grazing, 6 = stop grazing)
     */
     fieldOpGrass (int anOpCode){opCode = anOpCode;}

  //! Defines the parameters of the start grazing operation
  /*!
  \param aName name of the operation
  \param aNumber identity of the operation
  \param Year year
  \param Month month
  \param Day day
  \param theGrazingType type of animal grazing; 1=dairy, 2=other cattle, 3=sheep
  \param LAI leaf area index after grazing
  */

  void DefineOp(char * aName, int aNumber, int Year, int Month, int Day, int theGrazingType, double LAI);

  //! Defines the parameters of the start grazing operation
  /*!
  \param aName name of the operation
  \param aNumber identity of the operation
  \param Year year
  \param JulainDay day
  \param theGrazingType type of animal grazing; 1=dairy, 2=other cattle, 3=sheep
  \param LAI leaf area index after grazing
  */
  void DefineOp(char * aName, int aNumber, int Year, int JulianDay, int theGrazingType, double LAI);

  //! Defines the parameters of the stop grazing operation
  /*!
  \param aName name of the operation
  \param aNumber identity of the operation
  \param Year year
  \param Month month
  \param Day day
  \param theGrazingType type of animal grazing; 1=dairy, 2=other cattle, 3=sheep
  */

  void DefineOp(char * aName, int aNumber, int Year, int Month, int Day, int theGrazingType);

  //! Defines the parameters of the stop grazing operation
  /*!
  \param aName name of the operation
  \param aNumber identity of the operation
  \param Year year
  \param JulianDay day
  \param theGrazingType type of animal grazing; 1=dairy, 2=other cattle, 3=sheep
  */
  void DefineOp(char * aName, int aNumber, int Year, int JulianDay, int theGrazingType);
   ~ fieldOpGrass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  double getharvLAI() const  { return harvLAI;}
                       
                       //!Set accessor function for non-static attribute data member
  void setharvLAI (double aharvLAI)  { harvLAI = aharvLAI;}

//! Write details of the timing of the operation to a file
/*!
\param outfile pointer to the output file
\param thestatus integer indicating the success or failure of the operation to be performed as desired
\param printYear boolean which if true, prevents the year from being output
*/
	virtual void outputOps(fstream * outfile, int status, bool printYear);

  //!Read the operations from a NitroEurope field operation file
  /*!
  \param infile pointer to the input file
  \param theYear the year of the operation
  \param theJulianDay the Julian day of the operation
  \param anOpCode operation code
   */
	virtual void ReadOpResults(ifstream * infile, int theYear, int theJulianDay, int anOpCode);
};

#endif

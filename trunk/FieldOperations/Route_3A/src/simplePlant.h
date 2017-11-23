// Class: simplePlant                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __SIMPLEPLANT_H  //Required for current class
  #define __SIMPLEPLANT_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
#include <cropCharClass.h>        //Required for include files
                               
//! Describes a crop

class simplePlant  : public base        
{                       
			
  //! Name of the crop		
     string * Name; //Attribute data member
   //! Green leaf area index		
     double Lg; //Attribute data member
   //! Yellow leaf area index		
     double Ly; //Attribute data member
   //! Total leaf area index		
     double Ltot; //Attribute data member
   //! Current root depth (millimetres)		
     double rootDepth; //Attribute data member
   //! Breakpoint for soil moisture below which transpiration is reduced below the potential rate (proportion of available water capacity)		
     double breakpoint; //Attribute data member
   //! Maximum leaf area index		
     double Lmax; //Attribute data member
   //! Leaf area index during the winter		
     double Lwinter; //Attribute data member
   //! Maximum dead leaf area index		
     double Lymax; //Attribute data member
   //! Leaf area index of grass in undersown crops, after the cereal crop has been harvested		
     double Lmin; //Attribute data member
   //! Attenuation coefficient of Leaf area index		
     double attenuationCoeff; //Attribute data member
   //! Temperature sum for crop emergence in the spring (degree days)		
     double So; //Attribute data member
   //! Temperature sum for emergence of crops sown in the autumn (degree days)		
     double Soe; //Attribute data member
   //! Temperature sum to achieve maximum leaf area index (degree days)		
     double Sf; //Attribute data member
   //! Temperature sum to achieve maximum leaf area index in the autumn (degree days)		
     double Sfe; //Attribute data member
   //! Temperature sum for the start of maturation (degree days)		
     double Sr; //Attribute data member
   //! Temperature sum to achieve full maturity (degree days)		
     double Sm; //Attribute data member
   //! Root depth of crops during the winter (millimetres)		
     double rootDepthWinter; //Attribute data member
   //! Rate of increase of rooting depth (millimetres per day)		
     double croot; //Attribute data member
   //! Current temperature sum		
     double plantTsum; //Attribute data member
   //! Set to false if no crop is present		
     bool active; //Attribute data member
   //! Set to true if the transformation is specific to the autumn should be used		
     bool useAutumn; //Attribute data member
   //! Set to true if the transformation is specific to the winter should be used		
     bool useWinter; //Attribute data member        

private:

protected:

public:

			//!Default constructor
  simplePlant () {};

/*! Performs the daily functions
\param Tmean current air temperature (Celsius)
\param winter set to true if the winter functions should be used
*/					
   void dailyRoutine (double Tmean, bool winter)    ;

/*! Initialises the parameters of the crop model
\param thecropChars pointer to an instance containing the crop parameters
*/					
   void loadCropChars (cropCharClass * thecropChars)    ;

/*! Outputs the crop variables to file
\param afile pointer to the output file
\param header set to true if only the variable names should be printed
*/					
   void outputDetails (fstream * afile, bool header)    ;

/*! Deletes all existing crop parameters
\param autumn set to true if the autumn functions should be implemented
*/					
   void Initialise (bool autumn)    ;

//! Sets all plant parameters to 0					
   void kill ()    ;

/*! Harvests the crop
\param residualLAI leaf area index of the crop post harvesting
*/					
   void harvest (double residualLAI)    ;

/*! Cuts a grass crop
\param residualLAI leaf area index of the crop post cutting
*/					
   void cut (double residualLAI)    ;

//! Tidy up					
   void closeDown ()    ;
//!Destructor 
   ~ simplePlant ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  string * getName() const  { return Name;}

                       //!Get accessor function for non-static attribute data member
  double getLg() const  { return Lg;}

                       //!Get accessor function for non-static attribute data member
  double getLy() const  { return Ly;}

                       //!Get accessor function for non-static attribute data member
  double getLtot() const  { return Ltot;}

                       //!Get accessor function for non-static attribute data member
  double getrootDepth() const  { return rootDepth;}

                       //!Get accessor function for non-static attribute data member
  double getbreakpoint() const  { return breakpoint;}

                       //!Get accessor function for non-static attribute data member
  double getLmax() const  { return Lmax;}

                       //!Get accessor function for non-static attribute data member
  double getLwinter() const  { return Lwinter;}

                       //!Get accessor function for non-static attribute data member
  double getLymax() const  { return Lymax;}

                       //!Get accessor function for non-static attribute data member
  double getLmin() const  { return Lmin;}

                       //!Get accessor function for non-static attribute data member
  double getattenuationCoeff() const  { return attenuationCoeff;}

                       //!Get accessor function for non-static attribute data member
  double getSo() const  { return So;}

                       //!Get accessor function for non-static attribute data member
  double getSoe() const  { return Soe;}

                       //!Get accessor function for non-static attribute data member
  double getSf() const  { return Sf;}

                       //!Get accessor function for non-static attribute data member
  double getSfe() const  { return Sfe;}

                       //!Get accessor function for non-static attribute data member
  double getSr() const  { return Sr;}

                       //!Get accessor function for non-static attribute data member
  double getSm() const  { return Sm;}

                       //!Get accessor function for non-static attribute data member
  double getrootDepthWinter() const  { return rootDepthWinter;}

                       //!Get accessor function for non-static attribute data member
  double getcroot() const  { return croot;}

                       //!Get accessor function for non-static attribute data member
  double getplantTsum() const  { return plantTsum;}

                       //!Get accessor function for non-static attribute data member
  bool getactive() const  { return active;}

                       //!Get accessor function for non-static attribute data member
  bool getuseAutumn() const  { return useAutumn;}

                       //!Get accessor function for non-static attribute data member
  bool getuseWinter() const  { return useWinter;}
                       
                       //!Set accessor function for non-static attribute data member
  void setName (string * aName)  { Name = aName;}

                       //!Set accessor function for non-static attribute data member
  void setLg (double aLg)  { Lg = aLg;}

                       //!Set accessor function for non-static attribute data member
  void setLy (double aLy)  { Ly = aLy;}

                       //!Set accessor function for non-static attribute data member
  void setLtot (double aLtot)  { Ltot = aLtot;}

                       //!Set accessor function for non-static attribute data member
  void setrootDepth (double arootDepth)  { rootDepth = arootDepth;}

                       //!Set accessor function for non-static attribute data member
  void setbreakpoint (double abreakpoint)  { breakpoint = abreakpoint;}

                       //!Set accessor function for non-static attribute data member
  void setLmax (double aLmax)  { Lmax = aLmax;}

                       //!Set accessor function for non-static attribute data member
  void setLwinter (double aLwinter)  { Lwinter = aLwinter;}

                       //!Set accessor function for non-static attribute data member
  void setLymax (double aLymax)  { Lymax = aLymax;}

                       //!Set accessor function for non-static attribute data member
  void setLmin (double aLmin)  { Lmin = aLmin;}

                       //!Set accessor function for non-static attribute data member
  void setattenuationCoeff (double aattenuationCoeff)  { attenuationCoeff = aattenuationCoeff;}

                       //!Set accessor function for non-static attribute data member
  void setSo (double aSo)  { So = aSo;}

                       //!Set accessor function for non-static attribute data member
  void setSoe (double aSoe)  { Soe = aSoe;}

                       //!Set accessor function for non-static attribute data member
  void setSf (double aSf)  { Sf = aSf;}

                       //!Set accessor function for non-static attribute data member
  void setSfe (double aSfe)  { Sfe = aSfe;}

                       //!Set accessor function for non-static attribute data member
  void setSr (double aSr)  { Sr = aSr;}

                       //!Set accessor function for non-static attribute data member
  void setSm (double aSm)  { Sm = aSm;}

                       //!Set accessor function for non-static attribute data member
  void setrootDepthWinter (double arootDepthWinter)  { rootDepthWinter = arootDepthWinter;}

                       //!Set accessor function for non-static attribute data member
  void setcroot (double acroot)  { croot = acroot;}

                       //!Set accessor function for non-static attribute data member
  void setplantTsum (double aplantTsum)  { plantTsum = aplantTsum;}

                       //!Set accessor function for non-static attribute data member
  void setactive (bool aactive)  { active = aactive;}

                       //!Set accessor function for non-static attribute data member
  void setuseAutumn (bool auseAutumn)  { useAutumn = auseAutumn;}

                       //!Set accessor function for non-static attribute data member
  void setuseWinter (bool auseWinter)  { useWinter = auseWinter;}
       
};
			
#endif

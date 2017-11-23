// Class: cropCharClass                     //ANSI C++

#include <common.h>                 
#include <clonlist.h>                 

#ifndef __CROPCHARCLASS_H  //Required for current class
  #define __CROPCHARCLASS_H

#ifndef __IOSTREAM_H            //Required for cin and cout
  #include <iostream.h>                 
#endif

                               //Required for base classes
#ifndef __BASE_H
  #include "base.h"
#endif
                               
//! Holds the characteristics of an individual crop

class cropCharClass  : public base        
{                       
			
  //! Name of the crop		
     string Name; //Attribute data member
   //! Temperature sum for crop emergence in the spring		
     double So; //Attribute data member
   //! Temperature sum to achieve maximum leaf area index		
     double Sf; //Attribute data member
   //! Temperature sum for start of maturation (degree days)		
     double Sr; //Attribute data member
   //! Temperature sum to achieve full maturity (degree days)		
     double Sm; //Attribute data member
   //! Temperature sum for germination in the autumn (degree days)		
     double Soe; //Attribute data member
   //! Temperature sum to reach maximum leaf area index in the autumn (degree days)		
     double Sfe; //Attribute data member
   //! Maximum leaf area index of the crop		
     double Lmax; //Attribute data member
   //! Leaf area index of the crop during wintertime		
     double Lwinter; //Attribute data member
   //! Leaf area index of senescence material		
     double Lymax; //Attribute data member
   //! Leaf area index of undersown grass after harvesting of the cereal crop		
     double Lmin; //Attribute data member
   //! Attenuation coefficient of the leaf area index		
     double attenuationCoeff; //Attribute data member
   //! Rate of increase of root depth (millimetres per day)		
     double croot; //Attribute data member
   //! Depth of rooting in the wintertime		
     double rootDepthWinter; //Attribute data member        

private:

protected:

public:

			//!Default constructor
  cropCharClass () {};

//! Read the characteristics for all crops					
   void ReadParameters ()    ;

//!Initialise all parameters then read the data for a specific crop
/*! 
\param aFile pointer to the input file
\param index number of the crop in which parameters are required
*/					
   void Initialise (fstream * aFile, int index)    ;
//!Destructor 
   ~ cropCharClass ( );  //Destructor - Delete any pointer data members that used new in constructors 

                       //!Get accessor function for non-static attribute data member
  string getName() const  { return Name;}

                       //!Get accessor function for non-static attribute data member
  double getSo() const  { return So;}

                       //!Get accessor function for non-static attribute data member
  double getSf() const  { return Sf;}

                       //!Get accessor function for non-static attribute data member
  double getSr() const  { return Sr;}

                       //!Get accessor function for non-static attribute data member
  double getSm() const  { return Sm;}

                       //!Get accessor function for non-static attribute data member
  double getSoe() const  { return Soe;}

                       //!Get accessor function for non-static attribute data member
  double getSfe() const  { return Sfe;}

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
  double getcroot() const  { return croot;}

                       //!Get accessor function for non-static attribute data member
  double getrootDepthWinter() const  { return rootDepthWinter;}
                       
                       //!Set accessor function for non-static attribute data member
  void setName (string aName)  { Name = aName;}

                       //!Set accessor function for non-static attribute data member
  void setSo (double aSo)  { So = aSo;}

                       //!Set accessor function for non-static attribute data member
  void setSf (double aSf)  { Sf = aSf;}

                       //!Set accessor function for non-static attribute data member
  void setSr (double aSr)  { Sr = aSr;}

                       //!Set accessor function for non-static attribute data member
  void setSm (double aSm)  { Sm = aSm;}

                       //!Set accessor function for non-static attribute data member
  void setSoe (double aSoe)  { Soe = aSoe;}

                       //!Set accessor function for non-static attribute data member
  void setSfe (double aSfe)  { Sfe = aSfe;}

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
  void setcroot (double acroot)  { croot = acroot;}

                       //!Set accessor function for non-static attribute data member
  void setrootDepthWinter (double arootDepthWinter)  { rootDepthWinter = arootDepthWinter;}
       
};
			
#endif

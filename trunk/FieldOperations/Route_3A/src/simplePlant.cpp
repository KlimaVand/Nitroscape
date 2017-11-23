// Class: simplePlant

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "simplePlant.h"   
#include "cropCharClass.h" 				

simplePlant:: ~ simplePlant ( )
{

};

/*! Performs the daily functions
\param Tmean current air temperature (Celsius)
\param winter set to true if the winter functions should be used
*/
void simplePlant::dailyRoutine(double Tmean, bool winter)  
{
	useWinter=winter;
	if (active)  //this is set to false if there is no crop present
   {
      if (!useWinter)  //this is set to true if the winter crop  routines should be used
      {
	      plantTsum+=Tmean;
         if (!useAutumn)    //this is set to true if the autumn crop routines to be used
         {
            if (plantTsum > So)    //crop has emerged
            {
					rootDepth+=croot;
               if (*Name=="Grass")
               {
                  Lg = Lmax*(exp(2.4*(plantTsum - So)/(Sf-So))-1)/10.0;
                  if (Lg>Lmax)
                     Lg = Lmax;
               }
               else
               {
                  if (plantTsum < Sr)  //crop has not begun to ripen
                  {
                     Lg = Lmax*(exp(2.4*(plantTsum - So)/(Sf-So))-1)/10.0;
                     if (Lg>Lmax) //green leaf area is limited to Lmax
                        Lg = Lmax;
                  }
                  else
                     if ((plantTsum >= Sr) && (plantTsum < Sm))  //ripening stage
                     {
                        Ltot = Lmax - (Lmax - Lymax)*(plantTsum - Sr)/(Sm-Sr);
                        Lg = Lmax * (1 - (plantTsum - Sr)/(Sm-Sr));
                        if (Lg<Lmin)
                        	Lg=Lmin;
                        if (Lg>Lmax)
                           Lg = Lmax;
                        Ly=Ltot-Lg;
                     }
                     else
                     {
                     	if (Lmin==0) //crop is ripe and there is no undersown grass
                        {
                           Ltot = Lymax;
                           Lg=0.0;
                        }
                        else //crop is right and undersown grasses is present
                        {
                           Lg=Lmin;
                           Ltot = Lg + Ly;
                        }
                     }

               }
            }
         }
         else  //this is the autumn
         {
            if (plantTsum > Soe)    //autumn sown crop has emerged
            {
					rootDepth+=croot;
               Lg = Lmax*(exp(2.4*(plantTsum - Soe)/(Sfe-Soe))-1)/10.0;
               if (Lg>Lmax)
                  Lg = Lmax;
            }
         }
      }
   	else  //this is the winter
      {
			useAutumn = false;
			rootDepth+=croot;
         if (rootDepth>rootDepthWinter)
            rootDepth=rootDepthWinter;
      	if (Lg>0.0)  //if Lg > 0.0, crop already established
         {
         	Lg = Lwinter;
				plantTsum = So+log((10.0*Lg/Lmax)+1)*(Sf-So)/2.4;
         }
      }
      breakpoint = 0.5;
      Ltot = Lg + Ly;
   }
}
/*! Initialises the parameters of the crop model
\param thecropChars pointer to an instance containing the crop parameters
*/
void simplePlant::loadCropChars(cropCharClass * thecropChars)  
{
	if (Name->c_str()) //avoid memory leak
   	delete Name;
	Name=new string(thecropChars->getName());
	Lg=0.0;
   Ltot=Lg;
   Ly=0.0;
   plantTsum = 0.0;
   rootDepth = 0.0;
   attenuationCoeff = thecropChars->getattenuationCoeff();
   Lmax = thecropChars->getLmax();
   Lwinter = thecropChars->getLwinter();
   Lymax = thecropChars->getLymax();
   Lmin = thecropChars->getLmin();
   So = thecropChars->getSo();
   Soe = thecropChars->getSoe();
   Sf = thecropChars->getSf();
   Sfe = thecropChars->getSfe();
   Sr = thecropChars->getSr();
   Sm = thecropChars->getSm();
	rootDepthWinter = thecropChars->getrootDepthWinter();
	croot= thecropChars->getcroot();
   breakpoint = 0.5;
   active = true;
}
/*! Outputs the crop variables to file
\param afile pointer to the output file
\param header set to true if only the variable names should be printed
*/
void simplePlant::outputDetails(fstream * afile, bool header)  
{
	if (header)
		*afile << "Lg" << "\t" << "Ly" << "\t" << "rootDepth" << "\t" << "plantTsum" << "\t" ;
   else
		*afile << Lg << "\t" << Ly << "\t" << rootDepth << "\t" << plantTsum << "\t" ;
}
/*! Deletes all existing crop parameters
\param autumn set to true if the autumn functions should be implemented
*/
void simplePlant::Initialise(bool autumn)  
{
	Name = new string("No Crop");
	kill();
   useAutumn = autumn;
}
//! Sets all plant parameters to 0
void simplePlant::kill()  
{
	if (Name->c_str())
   	delete Name;
	Name = new string("No Crop");
	Lg=0.0;
   Ltot=0.0;
	Ly=0.0;
   rootDepth = 0.0;
   Lmax = 0.0;
   Lwinter =0.0;
   Lymax = 0.0;
   So = 0.0;
   Soe = 0.0;
   Sf = 0.0;
   Sfe = 0.0;
   Sr = 0.0;
   Sm = 0.0;
   active = false;
}
/*! Harvests the crop
\param residualLAI leaf area index of the crop post harvesting
*/
void simplePlant::harvest(double residualLAI)  
{
	if (Lmin==0.0) //there is no undersown grass
		kill();
   else
   {
   	delete Name;
	  Name=new string("Grass");
     Lg = Lmin;
     if (Lg>residualLAI)  //some of the undersown grass may be harvested along with the cereal crop
      {
         Lg=residualLAI;
         Lmin= Lg;
	      Ly=0.0;
      }
      else
           Ly=residualLAI-Lg;
     Ltot=Lg+Ly;
	  plantTsum = So+log((10.0*Lg/Lmax)+1)*(Sf-So)/2.4;
     rootDepth = rootDepthWinter;
  }
}
/*! Cuts a grass crop
\param residualLAI leaf area index of the crop post cutting
*/
void simplePlant::cut(double residualLAI)  
{
	if (Lg>residualLAI)
   {
		Lg=residualLAI;
      Ltot=Lg;
      if (Lg>0.0)
		  plantTsum = So+log((10.0*Lg/Lmax)+1)*(Sf-So)/2.4;
      else
         plantTsum = 0.0;
   }
}
//! Tidy up
void simplePlant::closeDown()  
{
if (Name)
 delete Name;}

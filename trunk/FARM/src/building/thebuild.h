#ifndef THEBUILDINGS_H
  #define THEBUILDINGS_H

class manureManager;
#ifdef _STANDALONE
	#include "base.h"
#endif
#include "stable.h"
#include "PigHousing.h"
#include "cattlehouse.h"
#include "slurrystore.h"
#include "biogasReactor.h"
#include "digestateStore.h"
#include "dungheap.h"
#include "barnStorage.h"
#include "../livestock/animal.h"

/**
 * Should only be instantiated ONCE. MUST be initialized AFTER theProducts
 * This class encapsulates a graph of buildings. (more or less)
 */

class buildings :public base
{
   /* Attributes */
   private:
	// a list of stables
   linkList<stable> *theStables;
   // at list of storage buildings
   linkList<barnStorage> *theStores;
   // a linst of building that store manure
   linkList<manurestore> *theManurestores;
   protected:
    /* Actions */
   private:

   PigHousing * defaultpigstable;
   cattlehouse * defaultcattlehouse;
   slurrystore * defaultslurrystore;
   biogasReactor * defaultbiogasReactor;
   digestateStore * defaultdigestateStore;
   dungheap * defaultdungheap;
   barnStorage * defaultbarnStorage;
   public:
      // Default Constructor
      buildings();
      // Constructor with arguments
      buildings(char *aName,const int aIndex,const base * aOwner);
      // Copy constructor
      buildings(const buildings& someBuildings);
      // Destructor
      ~buildings();
      // Initialization of all the buildings
      void InitializeAll(string  InputFilename, bool pigs, bool dairy, bool beef);


		animalSection * PlaceAnimal(const char * name);
		//!return pointer to the list of animal housing. Used in Nitroscape
      linkList<stable> * GettheStables() {return theStables;};
      // return all manurestore
      linkList<manurestore> * GettheManurestores() {return theManurestores;}
      //add cowstable* GetFreeStable(cowStock* aHerd); etc. as needed
		//Extractproduct expects a pointer to a product with a name and an amount
      //The function then finds that product in the buildigs and returns how much
      //is extracted, and inserts this into the object pointed to by p
      void StoreProduct(product* p);
      product *  ExtractProduct(product* p);
      double AmountProductStored(product* p);                   // JB did this
      virtual void DailyUpdate();
//      void GiveIndicator(int indicatorType);


		void EndBudget(bool show = false);
      double GetNInput();
      double GetNOutput();
		product * GetCopyStoredFeedProduct(product* p);
      void ScaleBuildings(); //scales buildings to animal numbers NJH June 2001
      double GetTotalManureNStored();
      double GetTotalSlurryStored();
      double GetTotalSlurryCap();
		void GetAvailableStoredFeed(feedItem *aProduct);

      product * EmptyManureStores();
		stable* GetStable(int stableNo){return theStables->ElementAtNumber(stableNo);};
		manurestore* GetManurestore(int storeNo){return theManurestores->ElementAtNumber(storeNo);};

};

extern buildings *theBuildings;

#endif


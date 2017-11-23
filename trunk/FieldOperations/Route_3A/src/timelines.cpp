#include "modelControlClass.h"
//#include "message.h"
#include "climateClass.h"

message * theMessage;
bsTime theTime;
int main(int argc,char *argv[])
{
   theTime.SetTime(1,1,1990);
	modelControlClass *aControlClass = new modelControlClass();
   aControlClass->Initialise(true,1,1,1989,31,12,2003);
   aControlClass->Simulate();
   aControlClass->closeDown();
   delete aControlClass;
   delete theMessage;
}

#include <message.h>
#include <bstime.h>

//--------------- Initialize static members ------------------------------------
	fstream * message::fw=NULL;
	fstream * message::fl=NULL;
	fstream * message::fd=NULL;
	fstream * message::ff1=NULL;
	fstream * message::ff2=NULL;
	char * message::WarnFileName=NULL;
	char * message::LogFileName=NULL;
	char * message::DebugFileName=NULL;
	char * message::FieldFile1Name=NULL;     
	char * message::FieldFile2Name=NULL;
	int message::Warnings=0;
	int message::Copies=0;
//------------------------------------------------------------------------------

message::message()
{
	Copies++;
	if (Copies!=1)
		FatalError("message::message - Only one instance of this class is allowed");
}

message::~message()
{
	if (fw)
   {
		delete fw;
		fw=NULL;
		delete fl;
      fl=NULL;
		delete WarnFileName;
		delete LogFileName;
      delete DebugFileName;
	}
	Copies--;
   if (fd)
   {
   	delete fd;
      fd=NULL;
   }
   if (ff1)
   {
   	delete ff1;
      ff1=NULL;
      delete FieldFile1Name;
   }
   if (ff2)
   {
   	delete ff2;
      ff2=NULL;
      delete FieldFile2Name;
   }
}

char * message::AssignString(char * c)
{
	char * ch=(char*)::operator new(strlen(c)+1);
	strcpy(ch,c);
	return ch;
}

// Must be called by simulation start. Can only be called once.
void message::InitMessage(char * WarnFN,char * LogFN, char *DebugFN, char *FieldFile1FN, char *FieldFile2FN)
{
   Warnings = 0;
	if (!fw)
   {
      {
         WarnFileName=AssignString(WarnFN);
         fw = new fstream(WarnFileName,ios::out);
         fw->close();
      }
      if (DebugFN!=NULL)
      {
         DebugFileName=AssignString(DebugFN);
         fd = new fstream(DebugFileName,ios::out);
         fd->close();
      }
      if (LogFN!=NULL)
      {
         LogFileName=AssignString(LogFN);
         fl = new fstream();
         fl->open(LogFileName,ios::out);
         fl->close();
      }
   }
   else
		FatalError("message::InitMessage called twice");
}

void message::ChangeLogfilename(char * cp)
{
	if (fw)
   {
		delete LogFileName;
		LogFileName=AssignString(cp);
		fl->open(LogFileName,ios::out);
		fl->close();
	}
	else
		FatalError("message::ChangeLogfilename called prior to initialization");
}

void message::Warning(const char * cp1,const char * cp2,const char * cp3)
{
	if (fw)
   {
		fw->open(WarnFileName,ios::app);
		*fw << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
		fw->close();
		Warnings++;
	}
	else
		FatalError("message::Warning called prior to initialization");
}

void message::Warning(const string st1, const string st2, const string st3)
{
	Warning((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}

void message::Warning(const char * cp1,const double aNumber,const char * cp3)
{
	char NumberString[40];
	sprintf(NumberString, " %f ", aNumber);
	Warning(cp1,NumberString,cp3);
}

void message::WarningWithDisplay(const char * cp1,const char * cp2,const char * cp3)
{
   Warning(cp1,cp2,cp3);
   cout << endl << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
}

void message::WarningWithDisplay(const string st1, const string st2, const string st3)
{
	WarningWithDisplay((char*) st1.c_str(),(char*) st2.c_str(),(char*) st3.c_str());
}

void message::Warning(const string cp1,const int anint)
{
	if (fw)
   {
		fw->open(WarnFileName,ios::app);
		*fw << cp1 << anint << " (" << theTime << ")" << endl;
		fw->close();
		Warnings++;
	}
	else
		FatalError("message::Warning called prior to initialization");
}
void message::WarningWithDisplay(const char * cp1,const double aNumber,const char * cp3)
{
	char NumberString[40];
	sprintf(NumberString, " %f ", aNumber);
	WarningWithDisplay(cp1,NumberString,cp3);
}

void message::FatalError(const char * cp1,const char * cp2,const char * cp3)
{
	cout << endl << "Fatal error: " << cp1 << cp2 << cp3 << endl;
   cout << " (" << theTime << ")" << endl;
//	cout << "Program will terminate. Press any key." << endl;
	cout << "Press 'Y' to terminate, any other key to continue." << endl; // While debugging
#ifdef __ANSICPP__
	char ch=getchar();
#else
   char ch=getch();
#endif
   if (ch=='y' || ch=='Y' )
   {
		fd->close();
		fl->close();
      exit(99);
   }
}

void message::LogEvent(const char * cp)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << cp << endl;
		fl->close();
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEvent(const char * cp, double number)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << cp << number << endl;
		fl->close();
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

int message::NumOfWarnings()
{
	return Warnings;
}

// Allows "streaming" of logfile-output.
fstream * message::GiveHandle()
{
	if (fl)
		fl->open(LogFileName,ios::app);
	else
		FatalError("message::GiveHandle called prior to initialization");
	return fl;
}

// Must allways be called after a call to "GiveHandle"
void message::CloseHandle()
{
	if (fl)
   {
		fl->close();
	}
	else
		FatalError("message::CloseHandle called prior to initialization");
}

// Allows "streaming" of debugfile-output.
fstream * message::GiveDebugHandle()
{
	if (fd)
		fd->open(DebugFileName,ios::app);
	else
		FatalError("message::GiveHandle called prior to initialization");
	return fd;
}
// this is how streaming should look in the home file
//   *theMessage->GiveDebugHandle() << theTime << " " << N_leached << " " << N_den << " " << N_runoff << " " << endl;
//   theMessage->CloseDebugHandle();


// Must allways be called after a call to "GiveHandle"
void message::CloseDebugHandle()
{
	if (fd)
   {
		fd->close();
	}
	else
		FatalError("message::CloseHandle called prior to initialization");
}

void message::LogEvent(const char * cp1, int anint, const char *cp2, double number)
{
	if (fl) {
		fl->open(LogFileName,ios::app);
		*fl << cp1 << anint << cp2 << number << endl;
		fl->close();
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEventWithTime(const char * cp1, int anint, const char *cp2, double number)
{
	if (fl) {
		fl->open(LogFileName,ios::app);
		*fl << theTime << " ";
		fl->close();
      LogEvent(cp1, anint, cp2, number);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEventWithTime(const char * cp1)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << theTime << " ";
		fl->close();
      LogEvent(cp1);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}

void message::LogEventWithTime(const char * cp, const double number)
{
	if (fl)
   {
		fl->open(LogFileName,ios::app);
		*fl << theTime << " ";
		fl->close();
      LogEvent(cp, number);
	}
	else
		FatalError("message::LogEvent called prior to initialization");
}


fstream * message::GiveFF1Handle()
{
	if (ff1)
		ff1->open(FieldFile1Name,ios::app);
	else
		FatalError("message::GiveFF1Handle called prior to initialization");
	return ff1;
}


void message::CloseFF1Handle()
{
	if (ff1)
   {
		ff1->close();
	}
	else
		FatalError("message::CloseFF1Handle called prior to initialization");
}

fstream * message::GiveFF2Handle()
{
	if (ff2)
		ff2->open(FieldFile2Name,ios::app);
	else
		FatalError("message::GiveFF2Handle called prior to initialization");
	return ff2;
}


void message::CloseFF2Handle()
{
	if (ff2)
   {
		ff2->close();
	}
	else
		FatalError("message::CloseFF2Handle called prior to initialization");
}


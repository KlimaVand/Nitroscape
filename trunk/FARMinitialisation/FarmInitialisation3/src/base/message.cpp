#include "message.h"
#include "bstime.h"
#include <stdio.h>
#include <string.h>

message::message()
{
	Copies++;
	fw=NULL;
	fl=NULL;
	fd=NULL;
	WarnFileName=NULL;
	LogFileName=NULL;
	DebugFileName=NULL;
	Warnings=0;
}

message::~message()
{
	fw->close();
	fl->close();
	fd->close();
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
}

char * message::AssignString(char * c)
{
	char * ch=(char*)::operator new(strlen(c)+1);
	strcpy(ch,c);
	return ch;
}

bool message::InitMessage(char * WarnFN,char * LogFN, char *DebugFN)
{
	bool retVal=true;
	Warnings = 0;
	if (!strlen(WarnFN)==0)
	{
		WarnFileName=AssignString(WarnFN);
		fw = new ofstream(WarnFileName,ios::out);
#ifndef __BCplusplus__
		if (!fw->is_open())
#else
		if (fw)
#endif
			retVal=false;
		else
			fw->close();
	}

	if (!strlen(LogFN)==0)
	{
		LogFileName=AssignString(LogFN);
		fl = new ofstream(LogFileName,ios::out);
#ifndef __BCplusplus__
		if (!fl->is_open())
#else
		if (fl)
#endif
			retVal=false;
		else
			fl->close();
	}
	if (!strlen(DebugFN)==0)
	{
		DebugFileName=AssignString(DebugFN);
		fd = new ofstream(DebugFileName,ios::out);
#ifndef __BCplusplus__
		if (!fd->is_open())
#else
		if (fd)
#endif
			retVal=false;
		else
			fd->close();
	}
	return retVal;
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
		if (showTime)
			*fw << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
		else
			*fw << cp1 << cp2 << cp3 << endl;
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
	if (showTime)
		cout << cp1 << cp2 << cp3 << " (" << theTime << ")" << endl;
	else
		cout << cp1 << cp2 << cp3 << endl;
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
		if (showTime)
			*fw << cp1 << anint << " (" << theTime << ")" << endl;
		else
			*fw << cp1 << anint << endl;
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

void message::WarningWithDisplay(const char * cp1,const int aNumber,const char * cp3)
{
	char NumberString[40];
	sprintf(NumberString, " %i ", aNumber);
	WarningWithDisplay(cp1,NumberString,cp3);
}

void message::FatalError(const char * cp1,const char * cp2,const char * cp3)
{
	cout << endl << "Fatal error: " << cp1 << cp2 << cp3 << endl;
	if (showTime)
		cout << " (" << theTime << ")" << endl;
	cout << "Press 'Y' to terminate, any other key to continue." << endl; // While debugging
	char ch=getchar();
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
ofstream * message::GiveHandle()
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
ofstream * message::GiveDebugHandle()
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

void message::LogEvent(const char * cp, const int number)
{
	if (fl) {
		fl->open(LogFileName,ios::app);
		*fl << cp << number << endl;
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




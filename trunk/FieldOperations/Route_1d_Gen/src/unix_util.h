/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef UNIX_UTIL_H
#define UNIX_UTIL_H

#include <fstream>
#include <string>
using namespace std;

//utility functions - avoids having to rely on compiler having them
extern void reverse( char* s );
#ifndef __BCplusplus__
	extern void itoa( int n, char* s, int maxcount );
//	int do_makeDirectory(const char *path, mode_t mode);
#else
//	int do_makeDirectory(const char *path);
#endif
	int do_makeDirectory(const char *path);
string getLineFile(ifstream *files);

#endif // UNIX_UTIL_H

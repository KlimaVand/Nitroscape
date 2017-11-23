#ifndef __ANSICPP__
   #include <windows.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
   #include <conio.h>
   #include <dir.h>
   #include <math.h>
   #include <base.h>
   #include <linklist.h>
   #include <assert.h>
#else
   #define MAXPATH 1024
	#include <cstdlib>
	#include <stdlib.h>
   #include <string>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
	#ifdef __WINDOWS__
		#include <cstring>
	#endif
#endif // __ANSICPP__

using namespace std;


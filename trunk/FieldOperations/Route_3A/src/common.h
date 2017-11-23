#ifndef __ANSICPP__
// **FN**

   #include <windows.h>
   #include <stdio.h>
   #include <stdlib.h>
   #include <iostream.h>
   #include <iomanip.h>
   #include <fstream.h>
   #include <conio.h>
   #include <dir.h>
   #include <math.h>
   #include <base.h>
   #include <linklist.h>
   #include <clonlist.h>
   #include <assert.h>
   //#define DEVCPP
   #ifdef DEVCPP
       #include <sstream>
       #include <cstring>
       using namespace std;
   #else
      #include <strstrea>
      #include <cstring.h>
   #endif
#else
   #define MAXPATH 1024
   #include <cstdio>
   #include <stdlib.h>
   #include <cstring>
   #include <string>
   #include <iostream>
   #include <iomanip>
   #include <fstream>
   #include <strstream.h>
   #include <algorithm>
   #include <math.h>
   #include <sys/stat.h>
   #include <unistd.h>
   #include "base.h"
   #include "linklist.h"
   #include "unix_util.h"

#endif // __ANSICPP__


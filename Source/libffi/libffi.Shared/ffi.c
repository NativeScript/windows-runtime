#if (_M_ARM)
#include "ARM\ffi.c"
#else
#include "x86\ffi.c"
#endif

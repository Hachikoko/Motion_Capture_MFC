//Copyright (C) 2008 Ehsan Kamrani
//This program is distributed under the terms of the GNU Lesser General Public License.


#pragma once

// DevIL
#ifdef _UNICODE
#undef _UNICODE
#undef UNICODE
#define UNICODE_DISABLED
#endif
#include "IL\IL.h"
#include "IL\ILU.h"
#ifdef UNICODE_DISABLED
#define _UNICODE
#endif
#pragma comment( lib, "DevIL.lib" )
#pragma comment( lib, "ILU.lib" )
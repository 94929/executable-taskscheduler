/* stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#include "targetver.h"

// Exclude rarely-used stuff from Windows headers.
#define WIN32_LEAN_AND_MEAN             

// Windows Header Files.
#include <windows.h>

// Reference additional headers your program requires here.
#define _WIN32_DCOM

#include <stdio.h>
#include <tchar.h>
#include <iostream>
#include <comdef.h>

// Include the task header file.
#include <taskschd.h>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

#define indexed_repeat(i, n) for (i=0; i<n; i++)

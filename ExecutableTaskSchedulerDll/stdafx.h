/* stdafx.h : include file for standard system include files,
 * or project specific include files that are used frequently, but
 * are changed infrequently
 */

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>

// Reference additional headers your program requires here
#define _WIN32_DCOM

#include <windows.h>
#include <iostream>
#include <comdef.h>

// Include the task header file.
#include <taskschd.h>

#pragma comment(lib, "taskschd.lib")
#pragma comment(lib, "comsupp.lib")

#define indexed_repeat(i, n) for (i=0; i<n; i++)
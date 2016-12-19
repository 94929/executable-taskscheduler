// ExecutableTaskSchedulerDll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "ExecutableTaskSchedulerDll.h"


// This is an example of an exported variable
EXECUTABLETASKSCHEDULERDLL_API int nExecutableTaskSchedulerDll = 0;

// This is an example of an exported function.
EXECUTABLETASKSCHEDULERDLL_API int fnExecutableTaskSchedulerDll(void)
{
	return 0;
}

// This is the constructor of a class that has been exported.
// see ExecutableTaskSchedulerDll.h for the class definition
CExecutableTaskSchedulerDll::CExecutableTaskSchedulerDll()
{
	return;
}

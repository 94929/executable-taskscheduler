// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the EXECUTABLETASKSCHEDULERDLL_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// EXECUTABLETASKSCHEDULERDLL_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef EXECUTABLETASKSCHEDULERDLL_EXPORTS
#define EXECUTABLETASKSCHEDULERDLL_API __declspec(dllexport)
#else
#define EXECUTABLETASKSCHEDULERDLL_API __declspec(dllimport)
#endif

// This class is exported from the ExecutableTaskSchedulerDll.dll
class EXECUTABLETASKSCHEDULERDLL_API CExecutableTaskSchedulerDll {
public:
	// Constructor
	CExecutableTaskSchedulerDll(void);
	
	// Other methods here.
	// OTHERS
};

extern EXECUTABLETASKSCHEDULERDLL_API int nExecutableTaskSchedulerDll;

EXECUTABLETASKSCHEDULERDLL_API int fnExecutableTaskSchedulerDll(void);

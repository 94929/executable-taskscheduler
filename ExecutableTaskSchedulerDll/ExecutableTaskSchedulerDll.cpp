// ExecutableTaskSchedulerDll.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include "ExecutableTaskSchedulerDll.h"


// This is an example of an exported variable
EXECUTABLETASKSCHEDULERDLL_API int nExecutableTaskSchedulerDll = 0;

// This is an example of an exported function.
EXECUTABLETASKSCHEDULERDLL_API int fnExecutableTaskSchedulerDll(void)
{
	//  ------------------------------------------------------
	//  Initialize COM.
	HRESULT hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		printf("\nCoInitializeEx failed: %x", hr);
		return 1;
	}

	//  Set general COM security levels.
	hr = CoInitializeSecurity(
		NULL,
		-1,
		NULL,
		NULL,
		RPC_C_AUTHN_LEVEL_PKT_PRIVACY,
		RPC_C_IMP_LEVEL_IMPERSONATE,
		NULL,
		0,
		NULL);

	if (FAILED(hr))
	{
		printf("\nCoInitializeSecurity failed: %x", hr);
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Create a name for the task.
	LPCWSTR wszTaskName = _T("Execute Process Task");

	//  Get the Windows directory and set the path to .exe file.
	PWCHAR pwcharExecutablePath = NULL;
	size_t requiredSize;
	_wgetenv_s(&requiredSize, NULL, 0, _T("WINDIR"));
	pwcharExecutablePath =
		(PWCHAR)realloc(pwcharExecutablePath, requiredSize*sizeof(WCHAR));

	//	Now get the path of WINDIR in PWCHAR format.
	_wgetenv_s(&requiredSize, pwcharExecutablePath, requiredSize, _T("WINDIR"));

	//  Now shorten the pwcharExecutablePath to contain local drive only.
	size_t i;
	indexed_repeat(i, requiredSize)
		if (pwcharExecutablePath[i] == '\\' &&
			pwcharExecutablePath[i + 1] == 'W')
			pwcharExecutablePath[i] = '\0';

	//	Convert the pwchar to wstring.
	std::wstring wstrExecutablePath = pwcharExecutablePath;

	//  Set the path of the process which will be running.
	wstrExecutablePath += _T("\\comtrue\\Procmon.exe");


	//  ------------------------------------------------------
	//  Create an instance of the Task Service.
	ITaskService *pService = NULL;
	hr = CoCreateInstance(CLSID_TaskScheduler,
		NULL,
		CLSCTX_INPROC_SERVER,
		IID_ITaskService,
		(void**)&pService);
	if (FAILED(hr))
	{
		printf("Failed to create an instance of ITaskService: %x", hr);
		CoUninitialize();
		return 1;
	}

	//  Connect to the task service.
	hr = pService->Connect(
		_variant_t(),
		_variant_t(),
		_variant_t(),
		_variant_t());
	if (FAILED(hr))
	{
		printf("ITaskService::Connect failed: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Get the pointer to the root task folder.
	//  This folder will hold the new task that is registered.
	ITaskFolder *pRootFolder = NULL;
	hr = pService->GetFolder(_bstr_t(_T("\\")), &pRootFolder);
	if (FAILED(hr))
	{
		printf("Cannot get Root Folder pointer: %x", hr);
		pService->Release();
		CoUninitialize();
		return 1;
	}

	//  If the same task exists, remove it.
	pRootFolder->DeleteTask(_bstr_t(wszTaskName), 0);

	//  Create the task builder object to create the task.
	ITaskDefinition *pTask = NULL;
	hr = pService->NewTask(0, &pTask);

	pService->Release();  // COM clean up.  Pointer is no longer used.
	if (FAILED(hr))
	{
		printf("Failed to create a task definition: %x", hr);
		pRootFolder->Release();
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Get the registration info for setting the identification.
	IRegistrationInfo *pRegInfo = NULL;
	hr = pTask->get_RegistrationInfo(&pRegInfo);
	if (FAILED(hr))
	{
		printf("\nCannot get identification pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	hr = pRegInfo->put_Author(_T("jsha"));
	pRegInfo->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put identification info: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  ------------------------------------------------------
	//  Create the settings for the task
	ITaskSettings *pSettings = NULL;
	hr = pTask->get_Settings(&pSettings);
	if (FAILED(hr))
	{
		printf("\nCannot get settings pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Set setting values for the task.
	hr = pSettings->put_StartWhenAvailable(/* VARIANT_TRUE */ VARIANT_FALSE);
	pSettings->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put setting info: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Get the trigger collection to insert the boot trigger.
	ITriggerCollection *pTriggerCollection = NULL;
	hr = pTask->get_Triggers(&pTriggerCollection);
	if (FAILED(hr))
	{
		printf("\nCannot get trigger collection: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Add the boot trigger to the task.
	ITrigger *pTrigger = NULL;
	hr = pTriggerCollection->Create(TASK_TRIGGER_BOOT, &pTrigger);
	pTriggerCollection->Release();
	if (FAILED(hr))
	{
		printf("\nCannot create the trigger: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	IBootTrigger *pBootTrigger = NULL;
	hr = pTrigger->QueryInterface(
		IID_IBootTrigger, (void**)&pBootTrigger);
	pTrigger->Release();

	// Delay the task to start 30 seconds after system start.
	hr = pBootTrigger->put_Delay(_T("PT30S"));
	pBootTrigger->Release();
	if (FAILED(hr))
	{
		printf("\nCannot put delay for boot trigger: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Add an Action to the task. This task will execute .exe file.
	IActionCollection *pActionCollection = NULL;

	//  Get the task action collection pointer.
	hr = pTask->get_Actions(&pActionCollection);
	if (FAILED(hr))
	{
		printf("\nCannot get Task collection pointer: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Create the action, specifying it as an executable action.
	IAction *pAction = NULL;
	hr = pActionCollection->Create(TASK_ACTION_EXEC, &pAction);
	pActionCollection->Release();
	if (FAILED(hr))
	{
		printf("\nCannot create the action: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	IExecAction *pExecAction = NULL;
	//  QI for the executable task pointer.
	hr = pAction->QueryInterface(
		IID_IExecAction, (void**)&pExecAction);
	pAction->Release();
	if (FAILED(hr))
	{
		printf("\nQueryInterface call failed for IExecAction: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	//  Set the path of the executable to .exe file.
	hr = pExecAction->put_Path(_bstr_t(wstrExecutablePath.c_str()));
	pExecAction->Release();
	if (FAILED(hr))
	{
		printf("\nCannot set path of executable: %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}


	//  ------------------------------------------------------
	//  Save the task in the root folder.
	IRegisteredTask *pRegisteredTask = NULL;
	hr = pRootFolder->RegisterTaskDefinition(
		_bstr_t(wszTaskName),
		pTask,
		TASK_CREATE_OR_UPDATE,
		_variant_t(),
		_variant_t(),
		TASK_LOGON_INTERACTIVE_TOKEN,
		_variant_t(),
		&pRegisteredTask);
	if (FAILED(hr))
	{
		printf("\nError saving the Task : %x", hr);
		pRootFolder->Release();
		pTask->Release();
		CoUninitialize();
		return 1;
	}

	printf("Success! Task successfully registered. \n");

	//  ------------------------------------------------------
	//  Clean up.
	pRootFolder->Release();
	pTask->Release();
	pRegisteredTask->Release();
	CoUninitialize();
	return 0;
}

// This is the constructor of a class that has been exported.
// see ExecutableTaskSchedulerDll.h for the class definition
CExecutableTaskSchedulerDll::CExecutableTaskSchedulerDll()
{
	return;
}

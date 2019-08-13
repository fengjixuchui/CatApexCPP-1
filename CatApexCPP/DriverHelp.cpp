#include "DriverHelp.h"


#define DEVICE_NAME			"\\\\?\\CatDriver64"

typedef struct rwStruct
{
	HANDLE PID;
	PVOID Address;
	DWORD64 Size;
	DWORD64 ReadAddress;
} RWStruct;

typedef struct Dbg_OpenStruct
{
	HANDLE ID;
	ULONG Access;
	PVOID Handle;
} Dbg_OpenStruct;

HANDLE hDevice = NULL;
HANDLE hGameProcess = NULL;


bool connectDrv()
{
	hDevice = CreateFileA(DEVICE_NAME, 0xC0000000, NULL, NULL, 3, NULL, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	protect();
	return true;
}

bool loadDrv(LPCSTR drvFile)
{
	if (connectDrv())
	{
		return true;
	}

	SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_CREATE_SERVICE);
	if (hSCManager == NULL)
	{
		return false;
	}
	SC_HANDLE hService = OpenServiceA(hSCManager, Service_NAME, SC_MANAGER_QUERY_LOCK_STATUS);
	if (hService == NULL)
	{
		hService = CreateServiceA(hSCManager, Service_NAME, Service_NAME, SC_MANAGER_QUERY_LOCK_STATUS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, drvFile, NULL, NULL, NULL, NULL, NULL);
		if (hService == NULL)
		{
			CloseServiceHandle(hSCManager);
			return false;
		}
	}
	StartServiceA(hService, 0, NULL);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	return connectDrv();
}

HANDLE Debug_OpenProcess(HANDLE ProcessID, ULONG Access)
{
	HANDLE hProc;
	Dbg_OpenStruct os = { ProcessID, Access, &hProc };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_DEBUG_OPENPROCESS, &os, sizeof(os), NULL, 0, &outSize, 0);
	return hProc;
}

HANDLE Debug_OpenThread(HANDLE ThreadID, ULONG Access)
{
	HANDLE hThread;
	Dbg_OpenStruct os = { ThreadID, Access, &hThread };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_DEBUG_OPENTHREAD, &os, sizeof(os), NULL, 0, &outSize, 0);
	return hThread;
}

DWORD64 getBaseModule(HANDLE proc)
{
	DWORD64 baseModule = NULL;
	RWStruct rs = { proc, &baseModule, 8, NULL };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_MODULE, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	return baseModule;
}

void readMem(HANDLE proc, DWORD64 addr, int size, PVOID data)
{
	if (IsBadWritePtr(data, size))
	{
		MessageBoxA(NULL, "RW ERROR", NULL, 0);
		return;
	}
	//RWStruct rs = { proc, data, (DWORD64)size, addr };
	//DWORD outSize;
	//DeviceIoControl(hDevice, IOCTL_IO_READ, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	ReadProcessMemory(hGameProcess, (LPCVOID)addr, data, size, 0);
}

void writeMem(HANDLE proc, DWORD64 addr, int size, PVOID data)
{
	if (IsBadReadPtr(data, size))
	{
		MessageBoxA(NULL, "RW ERROR", NULL, 0);
		return;
	}
	//RWStruct rs = { proc, data, (DWORD64)size, addr };
	//DWORD outSize;
	//DeviceIoControl(hDevice, IOCTL_IO_WRITE, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	WriteProcessMemory(hGameProcess, (LPVOID)addr, data, size, 0);
}

void protect()
{
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_PROTECT, NULL, 0, NULL, 0, &outSize, 0);
}

void unloadDrv()
{
	SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL) return;
	SC_HANDLE hService = OpenServiceA(hSCManager, Service_NAME, SC_MANAGER_ALL_ACCESS);
	if (hService == NULL) return;

	SERVICE_STATUS ServiceStatus;
	ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
	CloseServiceHandle(hService);
	CloseServiceHandle(hSCManager);
	CloseHandle(hDevice);
}

#include "DriverHelp.h"

#define Service_NAME		"Cat64"
#define DEVICE_NAME			"\\\\?\\catrw64"

typedef struct rwStruct
{
	HANDLE PID;
	PVOID Address;
	DWORD64 Size;
	DWORD64 ReadAddress;
} RWStruct;

HANDLE hDevice = NULL;

bool connectDrv()
{
	hDevice = CreateFileA(DEVICE_NAME, 3221225472, NULL, NULL, 3, NULL, NULL);
	if (hDevice == INVALID_HANDLE_VALUE)
	{
		return false;
	}
	//protect();
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
	PVOID tmpMemory = malloc(size);
	memset(tmpMemory, 0, size);
	RWStruct rs = { proc, tmpMemory, (DWORD64)size, addr };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_READ, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	memcpy(data, tmpMemory, size);
	free(tmpMemory);
}

void writeMem(HANDLE proc, DWORD64 addr, int size, PVOID data)
{
	PVOID tmpMemory = malloc(size);
	memcpy(tmpMemory, data, size);
	RWStruct rs = { proc, tmpMemory, (DWORD64)size, addr };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_WRITE, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	free(tmpMemory);
}

void protect()
{
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_PROTECT, NULL, 0, NULL, 0, &outSize, 0);
}
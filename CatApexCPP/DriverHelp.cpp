#include "DriverHelp.h"
#include "proc_help.h"


#define DEVICE_NAME			"\\\\?\\KDSEVDNPSHON"
	
DWORD IOCTL_IO_MODULE = 0;
DWORD IOCTL_IO_READ = 0;
DWORD IOCTL_IO_WRITE = 0;
DWORD IOCTL_IO_HIDEDRV = 0;
DWORD IOCTL_IO_PROTECT = 0;
DWORD IOCTL_IO_DEBUG_OPENPROCESS = 0;
DWORD IOCTL_IO_DEBUG_OPENTHREAD = 0;
DWORD IOCTL_IO_READ_CR3 = 0;
DWORD IOCTL_IO_WRITE_CR3 = 0;
DWORD IOCTL_IO_OPEN_PROCESS = 0;



typedef struct _RWStruct
{
	DWORD64 PEP;
	PVOID BufferAddress;
	SIZE_T Size;
	DWORD64 TargetAddress;
} RWStruct, * PRWStruct;

typedef struct _OPENStruct
{
	HANDLE PID;
	PVOID BufferAddress;
} OPENStruct, * POPENStruct;

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

bool LoadDrv(LPCSTR drvFile)
{
	IOCTL_IO_MODULE = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1000, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_READ = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1001, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_WRITE = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1002, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_HIDEDRV = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1003, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_PROTECT = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1004, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_DEBUG_OPENPROCESS = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1010, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_DEBUG_OPENTHREAD = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1011, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_READ_CR3 = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1006, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_WRITE_CR3 = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1007, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	IOCTL_IO_OPEN_PROCESS = CTL_CODE(FILE_DEVICE_UNKNOWN, 0x1008, METHOD_IN_DIRECT, FILE_ANY_ACCESS);
	if (connectDrv())
	{
		return true;
	}

	SC_HANDLE hSCManager = OpenSCManagerA(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		return false;
	}

	SC_HANDLE hService = CreateServiceA(hSCManager, Service_NAME, Service_NAME, SC_MANAGER_ALL_ACCESS, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, drvFile, NULL, NULL, NULL, NULL, NULL);
	if (hService)
	{
		StartServiceA(hService, 0, NULL);
		DeleteService(hService);
		CloseServiceHandle(hService);
	}
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

void LookupOpenProcess(HANDLE pid, PVOID buff) {
	OPENStruct os = { pid, buff };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_OPEN_PROCESS, &os, sizeof(os), NULL, 0, &outSize, 0);
}

DWORD64 getBaseModule(DWORD64 proc)
{
	DWORD64 baseModule = NULL;
	RWStruct rs = { proc, &baseModule, 8, NULL };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_MODULE, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	return baseModule;
}

void readMem(DWORD64 proc, DWORD64 addr, int size, PVOID data)
{
	RWStruct rs = { proc, data, (DWORD64)size, addr };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_READ, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	//ReadProcessMemory(hGameProcess, (LPCVOID)addr, data, size, 0);
}

void writeMem(DWORD64 proc, DWORD64 addr, int size, PVOID data)
{
	RWStruct rs = { proc, data, (DWORD64)size, addr };
	DWORD outSize;
	DeviceIoControl(hDevice, IOCTL_IO_WRITE, &rs, sizeof(rs), NULL, 0, &outSize, 0);
	//WriteProcessMemory(hGameProcess, (LPVOID)addr, data, size, 0);
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

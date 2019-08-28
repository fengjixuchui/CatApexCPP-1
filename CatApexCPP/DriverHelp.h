#include <windows.h>

bool LoadDrv(LPCSTR drvFile);
HANDLE Debug_OpenProcess(HANDLE ProcessID, ULONG Access);
HANDLE Debug_OpenThread(HANDLE ThreadID, ULONG Access);
void LookupOpenProcess(HANDLE pid, PVOID buff);
DWORD64 getBaseModule(DWORD64 proc);
void readMem(DWORD64 proc, DWORD64 addr, int size, PVOID data);
void writeMem(DWORD64 proc, DWORD64 addr, int size, PVOID data);
void protect();
void unloadDrv();

extern char * Service_NAME;
extern HANDLE hDevice;
extern HANDLE hGameProcess;
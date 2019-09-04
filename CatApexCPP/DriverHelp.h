#include <windows.h>

bool LoadDrv(LPCSTR drvFile);
HANDLE Debug_OpenProcess(HANDLE ProcessID, ULONG Access);
HANDLE Debug_OpenThread(HANDLE ThreadID, ULONG Access);
DWORD64 getBaseModule(HANDLE proc);
void readMem(HANDLE proc, DWORD64 addr, int size, PVOID data);
void writeMem(HANDLE proc, DWORD64 addr, int size, PVOID data);
void protect();
void unloadDrv();

extern char * Service_NAME;
extern HANDLE hDevice;
extern HANDLE hGameProcess;
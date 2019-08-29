#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "proc_help.h"
#include "DriverHelp.h"
#include "DrawEng.h"
#include "hackerThreads.h"
#include <atlstr.h>
#include <chrono>

//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//不显示窗口
//#pragma comment(linker,"/MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR")//减小编译体积

using namespace std;

DWORD64 gameHandle;
HANDLE gamePID;
__int64 hGameModule;
int fontSize;
HWND hGameWind;
char * Service_NAME = 0;


int main() {
	std::chrono::milliseconds uptime = std::chrono::milliseconds(GetTickCount64());
	printf("UP TIME: %lld s\n", uptime.count() / 1000);
	auto boot_time = std::chrono::system_clock::now() - uptime;
	printf("BOOT TIME: %lld\n", ((boot_time.time_since_epoch().count() / 10000000) / 60) * 60);
	Service_NAME = (char *)malloc(50);
	memset(Service_NAME, 0, 50);
	sprintf(Service_NAME, "Z%lld", ((boot_time.time_since_epoch().count() / 10000000) / 60) * 60);
	printf("Service Name: %s\n", Service_NAME);
	initColor();
	initConfig();
	initMenu();
	const char *mySys = "D:\\rw.sys";
	bool hDriver = LoadDrv(mySys);
	printf("%s\n", mySys);
	if (!hDriver) {
		MessageBoxA(nullptr, "安装驱动服务失败, 请检查杀毒软件是否拦截或重复开启", nullptr, 0);
		return -1;
	}
	DEVMODE devMode;
	devMode.dmSize = sizeof(devMode);
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &devMode);
	printf("OK Your display frequency is %d\n", devMode.dmDisplayFrequency);
	hGameWind = 0;
	while (!hGameWind) {
		hGameWind = FindWindowA("Respawn001", "Apex Legends");
		Sleep(100);
	}
	gamePID = (HANDLE)GetProcessIDByName("r5apex.exe");
	if (!gamePID) {
		MessageBoxA(nullptr, "进程ID获取错误，请检查您的进程名是否为r5apex.exe", nullptr, 0);
		return -1;
	}
	printf("进程ID: %p\n", gamePID);
	//hGameProcess = Debug_OpenProcess(gamePid, PROCESS_ALL_ACCESS);
	//printf("打开进程 HANDLE: %d\n", hGameProcess);
	gameHandle = 0;
	LookupOpenProcess(gamePID, &gameHandle);
	hGameModule = getBaseModule(gameHandle);
	printf("内核进程句柄指针: %I64x\n", gameHandle);
	if (gameHandle == 0)
	{
		printf("LOOKUP失败\n");
		exit(0);
	}
	printf("模块地址: %I64x\n", hGameModule);
	
	//unloadDrv();
	//printf("卸载驱动\n", hGameModule);
	ShowWindow(hGameWind, 9);
	fontSize = 64;
	startThreads();
	StartDraw(0);

}
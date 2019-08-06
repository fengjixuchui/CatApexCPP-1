#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "proc_help.h"
#include "DriverHelp.h"
#include "DrawEng.h"
#include "hackerThreads.h"
#include <atlstr.h>

HANDLE gamePid;
__int64 hGameModule;
int fontSize;
HWND hGameWind;

int main() {
	initColor();
	initConfig();
	initMenu();
	const char *mySys = "C:\\catdrv.sys";
	bool hDriver = loadDrv(mySys);
	printf("%s\n", mySys);
	if (!hDriver) {
		MessageBoxA(nullptr, "安装驱动服务失败, 请检查杀毒软件是否拦截或重复开启", nullptr, 0);
		return -1;
	}
	printf("OK\n");
	hGameWind = 0;
	while (!hGameWind) {
		hGameWind = FindWindowA("Respawn001", "Apex Legends");
		Sleep(100);
	}
	gamePid = (HANDLE)GetProcessIDByName("r5apex.exe");
	if (!gamePid) {
		MessageBoxA(nullptr, "进程ID获取错误，请检查您的进程名是否为r5apex.exe", nullptr, 0);
		return -1;
	}
	printf("进程ID: %p\n", gamePid);
	hGameModule = getBaseModule((HANDLE)gamePid);
	printf("模块地址: %lld\n", hGameModule);
	ShowWindow(hGameWind, 9);
	fontSize = 64;
	startThreads();
	StartDraw(0);

}
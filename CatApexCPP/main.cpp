#include <iostream>
#include <stdio.h>
#include <Windows.h>
#include "proc_help.h"
#include "DriverHelp.h"
#include "DrawEng.h"
#include "hackerThreads.h"
#include <atlstr.h>
#include <chrono>

//#pragma comment(linker,"/subsystem:\"windows\" /entry:\"mainCRTStartup\"")//����ʾ����
//#pragma comment(linker,"/MERGE:.rdata=.text /MERGE:.data=.text /SECTION:.text,EWR")//��С�������

using namespace std;

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
		MessageBoxA(nullptr, "��װ��������ʧ��, ����ɱ������Ƿ����ػ��ظ�����", nullptr, 0);
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
		MessageBoxA(nullptr, "����ID��ȡ�����������Ľ������Ƿ�Ϊr5apex.exe", nullptr, 0);
		return -1;
	}
	printf("����ID: %p\n", gamePID);
	//hGameProcess = Debug_OpenProcess(gamePid, PROCESS_ALL_ACCESS);
	//printf("�򿪽��� HANDLE: %d\n", hGameProcess);
	hGameModule = getBaseModule(gamePID);
	printf("ģ���ַ: %I64x\n", hGameModule);
	
	//unloadDrv();
	//printf("ж������\n", hGameModule);
	ShowWindow(hGameWind, 9);
	fontSize = 64;
	startThreads();
	StartDraw(0);

}
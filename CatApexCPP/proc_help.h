#define _SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS 1
#pragma once

#include <dinput.h>
#include <tchar.h>
#include <stdio.h>
#include <TlHelp32.h>
#include <comdef.h>
#include <vector>
#include <hash_map>
#include "imgui/imgui.h"


#define CEntityList				0x1F1AC78
#define CLocalEntity			0x22CE368
#define ViewRender				0xC579FF8
#define NameList				0xC57AE40
#define m_location				0x13C
#define m_mouse					0x21B8
#define m_renderArray			0x1A93D0

#define m_iHealth                    0x3E0
#define m_iMaxHealth                 0x500
#define m_shieldHealth				0x164
#define m_iTeamNum                   0x3F0
#define m_lifeState                  0x720
#define m_bleedoutState              0x2378
#define m_armorType                  0x4208
#define m_helmetType                 0x4204
#define m_duckState                  0x2660
#define m_fFlags                     0x8C
#define m_vecVelocity                0x418
#define m_latestPrimaryWeapons       0x1704
#define m_latestNonOffhandWeapons    0x170C
#define m_selectedOffhands           0x170E
#define m_iSignifierName             0x508
#define m_customScriptInt            0x1308
#define m_vecAimPunch                 0x2114
#define m_flBulletSpeed               0x1C24
#define m_flWeaponSpread1             0x1370
#define m_flWeaponSpread2             0x1380
#define CheckSum                     0x88C4F0

#define m_EyePosition                  0x4224

typedef struct __ColorStruct {
	unsigned int ShiShi;
	unsigned int ShengQi;
	unsigned int XiYou;
	unsigned int GaoJi;
	unsigned int PuTong;
	unsigned int NengLiang;
	unsigned int ZhongXing;
	unsigned int SanDan;
	unsigned int QingXing;
} ColorStruct;

typedef struct __AppConfigs {
	bool MenuStatus;
	bool FangKuang;
	float TouShiFanWei;
	bool GuGeTouShi;
	bool WuPingTouShi;
	float WuPingFanWei;
	bool QiangXieTouShi;
	bool ZiDanTouShi;
	bool FangJuTouShi;
	bool PeiJianTouShi;
	bool ZhongYaoWuZi;
	bool ZiDongMiaoZhun;
	float ZiMiaoFanWei;
	int MiaoZhunReJian;
} AppConfigs;

typedef struct ItemInfo {
	const char *name;
	unsigned int color;
} ApexItem;

typedef struct EntityStruct {
	__int64 point;
	int type; // 0-item 1-player
	int flag;
	char *name;
	char *apexName;
	unsigned int color;
	int zc;
	ApexItem item;
} ApexEntity;

typedef struct __Vec3D {
	float x;
	float y;
	float z;
} Vec3D;

typedef struct __Point_INDEX {
	__int64 point;
	int index;
} Point_Index;

extern HWND myHWND;
extern HANDLE gamePid;
extern HWND hGameWind;
extern __int64 hGameModule;
extern int fontSize;
extern ColorStruct colors;
extern AppConfigs appConfigs;
extern int menu_zuobian;
extern int menu_dingbian;
extern int menu_kuan;
extern int menu_gao;
extern int menu_hangju;
extern int menu_select;
extern int menu_size;
extern int menu_fenge;
extern int nengnenglvse;
extern int nengnenglanse;
extern POINT gamePoint;
extern RECT gameRect;
extern int windowW;
extern int windowH;
extern POINT CentWindow;
extern std::vector<void *> needFrees;


//point

extern __int64 TempPoint;
extern __int64 ArrayPoint;
extern __int64 MySelfPoint;
extern __int64 EntityListPoint;

//data
extern int MyTeam;
extern float worldArray[4][4];
extern std::vector<ApexEntity> apexEntityList;
extern std::hash_map<int, ApexItem> entityNames;
extern bool aim;
extern bool aimThreadStop;
extern HANDLE hAimThread;
extern __int64 aimEntity;


DWORD GetProcessIDByName(const char *pName);

char *rand_str(char *str, const int len);

int getRandom(int a, int b);

void initColor();

void initConfig();

void initMenu();

void startThreads();

void initEntityNames();
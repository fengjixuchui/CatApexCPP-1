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

#define rotation					57.295779513082f

#define CEntityList					0x1F1AC78
#define CLocalEntity				0x22CE368
#define ViewRender					0xC579FF8
#define NameList					0xC57AE40
#define m_location					0x13C
#define m_mouse						0x21B8
#define m_renderArray				0x1A93D0

#define m_iHealth					0x3E0
#define m_iMaxHealth				0x500
#define m_shieldHealth				0x164
#define m_iTeamNum					0x3F0
#define m_lifeState					0x720
#define m_bleedoutState				0x2378
#define m_armorType					0x4208
#define m_helmetType				0x4204
#define m_duckState					0x2660
#define m_fFlags					0x8C
#define m_vecVelocity				0x418
#define m_latestPrimaryWeapons		0x1704
#define m_latestNonOffhandWeapons	0x170C
#define m_selectedOffhands			0x170E
#define m_iSignifierName			0x508
#define m_customScriptInt			0x1308
#define m_vecAimPunch				0x2114
#define m_flBulletSpeed				0x1C24
#define m_flWeaponSpread1			0x1370
#define m_flWeaponSpread2			0x1380
#define m_EyePosition				0x4224
#define m_bone						0xED8
#define m_entityTypeInfo			0x40

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
	bool DanwuSanShe;
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
	int distance;
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
extern __int64 MouseAddr;

//data
extern int MyTeam;
extern float worldArray[4][4];
extern std::vector<ApexEntity> apexEntityList;
extern std::hash_map<int, ApexItem> entityNames;
extern bool aim;
extern bool aimThreadStop;
extern HANDLE hAimThread;
extern __int64 aimEntity;

extern int Ñ°Ñª[3][16];
extern int Ö±²¼[3][16];
extern int ÃüÂö[3][16];
extern int Ì½Â·[3][16];
extern int ¶¯Á¦[3][16];

extern int ¶ñÁé[3][16];
extern int °à¼Ó[3][16];
extern int ÇÖÊ´[3][16];
extern int »ÃÏó[3][16];
extern int »ªÉ­[3][16];



DWORD GetProcessIDByName(const char *pName);
void rand_str(char *str, const int len);
wchar_t* char2wchar_t(char* cstr);
void initColor();
void initConfig();
void initMenu();
void startThreads();
void initEntityNames();
void initBones();
void usleep(unsigned int usec);


enum Bones : int {
	head = 12,
	¾± = 11,
	ĞØ = 7,
	¸¹ = 6,
	ŒÅ = 5,


	ÃüÂö×ó¼ç = 40, // +1
	ÃüÂöÓÒ¼ç = 15, // +1
	ÃüÂö×óËø¹Ç = 38, // ×ó¼ç - 2
	ÃüÂöÓÒËø¹Ç = 13, // ÓÒ¼ç - 2
	ÃüÂö×ó±Û = 41, // ×ó¼ç + 1
	ÃüÂö×ó±Û1 = 43, // Ö÷¹Ø½Ú + 2
	ÃüÂö×ó±Û2 = 42, // 1¹Ø½Ú - 1
	ÃüÂö×ó±Û3 = 44, // 1¹Ø½Ú + 1
	ÃüÂö×ó±Û4 = 45, // 3¹Ø½Ú + 1
	ÃüÂö×ó±Û5 = 47, // 4¹Ø½Ú + 2 ÖĞ¼äÓĞ¸ö¹Ø½Ú
	ÃüÂöÓÒ±Û = 16, // ÓÒ¼ç + 1
	ÃüÂöÓÒ±Û1 = 18, // Ö÷¹Ø½Ú + 2
	ÃüÂöÓÒ±Û2 = 17, // 1¹Ø½Ú -1
	ÃüÂöÓÒ±Û3 = 19, // 1¹Ø½Ú + 1
	ÃüÂöÓÒ±Û4 = 20, // 3¹Ø½Ú + 1
	ÃüÂöÓÒ±Û5 = 22, // 4¹Ø½Ú + 2 ÖĞ¼äÓĞ¸ö¹Ø½Ú
	ÃüÂö×óÏ¥ = 70, // N
	ÃüÂö×ó½Å = 71, // ×óÏ¥ + 1
	ÃüÂö×ó½Å1 = 72, // ×ó½Å + 1
	ÃüÂöÓÒÏ¥ = 65, // N
	ÃüÂöÓÒ½Å = 66, // ÓÒÏ¥ + 1
	ÃüÂöÓÒ½Å1 = 67, // ÓÒ½Å + 1


	Ö±²¼×ó¼ç = 43,
	Ö±²¼ÓÒ¼ç = 16,
	Ö±²¼×óÏ¥ = 79,
	Ö±²¼ÓÒÏ¥ = 74,

	Ñ°Ñª×ó¼ç = 49,
	Ñ°ÑªÓÒ¼ç = 16,
	Ñ°Ñª×óÏ¥ = 94,
	Ñ°ÑªÓÒÏ¥ = 90,

	¶ñÁé×ó¼ç = 40,
	¶ñÁéÓÒ¼ç = 15,
	¶ñÁé×óÏ¥ = 97,
	¶ñÁéÓÒÏ¥ = 92,

	Ì½Â·×ó¼ç = 28,
	Ì½Â·ÓÒ¼ç = 14,
	Ì½Â·×óÏ¥ = 77,
	Ì½Â·ÓÒÏ¥ = 70,

	°à¼Ó×ó¼ç = 44,
	°à¼ÓÓÒ¼ç = 15,
	°à¼Ó×óÏ¥ = 87,
	°à¼ÓÓÒÏ¥ = 82,

	ÇÖÊ´×ó¼ç = 38,
	ÇÖÊ´ÓÒ¼ç = 14,
	ÇÖÊ´×óÏ¥ = 81,
	ÇÖÊ´ÓÒÏ¥ = 76,

	»ÃÏó×ó¼ç = 41,
	»ÃÏóÓÒ¼ç = 14,
	»ÃÏó×óÏ¥ = 91,
	»ÃÏóÓÒÏ¥ = 86,

	¶¯Á¦×ó¼ç = 44, // 44
	¶¯Á¦ÓÒ¼ç = 15,
	¶¯Á¦×óÏ¥ = 92, // 92
	¶¯Á¦ÓÒÏ¥ = 82, // 82

	»ªÉ­×ó¼ç = 69,
	»ªÉ­ÓÒ¼ç = 44,
	»ªÉ­×óÏ¥ = 116, // 92
	»ªÉ­ÓÒÏ¥ = 110, // 82
};
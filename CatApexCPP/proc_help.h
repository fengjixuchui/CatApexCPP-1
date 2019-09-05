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

#define CEntityList					0x1f27268//0x1F01B08//0x1F1AC78
#define CLocalEntity				0x22da978//0x22B5138//0x22CE368
#define ViewRender					0xcb4a038//0xC57AE90//0xC579FF8
#define NameList					0x173E498//0xC57BCE0//0xC57AE40
#define m_location					0x144//0x13C
#define m_mouse						0x2150//0x21F8//0x21B8
#define m_renderMouse				0x10
#define m_renderArray				0x1A93D0
#define m_nameArrayListSub			0xA40
#define m_entityBasePunch			0x213c

#define m_iHealth					0x3E0
#define m_iMaxHealth				0x510//0x500
#define m_shieldHealthMax			0x16c//0x164
#define m_iTeamNum					0x3F0
#define m_lifeState					0x730//0x720
#define m_bleedoutState				0x2310//0x23B8//0x2378
#define m_armorType					0x4248//0x4208
#define m_helmetType				0x4244//0x4204
#define m_duckState					0x26A0//0x2660
#define m_vecVelocity				0x41C//0x418
#define m_latestPrimaryWeapons		0x1704//0x1744//0x1704
#define m_latestNonOffhandWeapons	0x170c//0x174C//0x170C
#define m_allWeapons				0x2C//????
#define m_selectedOffhands			0x174E//0x170E
#define m_iSignifierName			0x518//0x508
#define m_customScriptInt			0x1308//0x1348//0x1308
#define m_vecAimPunch				0x20b4//0x2154//0x2114
#define m_flBulletSpeed				0x1C3C//0x1C6C//0x1C24
#define m_bone						0xED8//0xF18//0xED8
#define m_entityTypeInfo			0x820//0x828//0x40
#define m_flWeaponSpread1			0x1370//??????
#define m_flWeaponSpread2			0x1380//??????

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
	bool XianShiZhaZhu;
	bool KaiFaZheXuanXiang;
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
	const char * WeaponName;
} ApexEntity;

typedef struct __Vec3D {
	float x;
	float y;
	float z;
} Vec3D;

typedef struct __DVec3D {
	double x;
	double y;
	double z;
} DVec3D;

typedef struct __Point_INDEX {
	__int64 point;
	int index;
} Point_Index;

typedef struct __AimEntityDrawData {
	float BoxX;
	float BoxY;
	float BoxY1;
	ImColor color;
	char * text;
} AimEntityDrawData;

extern HWND myHWND;
extern HANDLE gamePID;
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

extern int ѰѪ[3][16];
extern int ֱ��[3][16];
extern int ����[3][16];
extern int ̽·[3][16];
extern int ����[3][16];

extern int ����[3][16];
extern int ���[3][16];
extern int ��ʴ[3][16];
extern int ����[3][16];
extern int ��ɭ[3][16];


DWORD GetProcessIDByName(const char *pName);
void rand_str(char *str, const int len);
void initColor();
void initConfig();
void initMenu();
void startThreads();
void initEntityNames();
void initBones();
void usleep(unsigned int usec);
int getRandomInt(int min, int max);
void Print_Memory(const unsigned char* start, unsigned int length);


enum Bones : int {
	head = 12,
	neck = 11,
	�� = 7,
	�� = 6,
	�� = 5,


	������� = 40, // +1
	�����Ҽ� = 15, // +1
	���������� = 38, // ��� - 2
	���������� = 13, // �Ҽ� - 2
	������� = 41, // ��� + 1
	�������1 = 43, // ���ؽ� + 2
	�������2 = 42, // 1�ؽ� - 1
	�������3 = 44, // 1�ؽ� + 1
	�������4 = 45, // 3�ؽ� + 1
	�������5 = 47, // 4�ؽ� + 2 �м��и��ؽ�
	�����ұ� = 16, // �Ҽ� + 1
	�����ұ�1 = 18, // ���ؽ� + 2
	�����ұ�2 = 17, // 1�ؽ� -1
	�����ұ�3 = 19, // 1�ؽ� + 1
	�����ұ�4 = 20, // 3�ؽ� + 1
	�����ұ�5 = 22, // 4�ؽ� + 2 �м��и��ؽ�
	������ϥ = 70, // N
	������� = 71, // ��ϥ + 1
	�������1 = 72, // ��� + 1
	������ϥ = 65, // N
	�����ҽ� = 66, // ��ϥ + 1
	�����ҽ�1 = 67, // �ҽ� + 1


	ֱ����� = 43,
	ֱ���Ҽ� = 16,
	ֱ����ϥ = 79,
	ֱ����ϥ = 74,

	ѰѪ��� = 49,
	ѰѪ�Ҽ� = 16,
	ѰѪ��ϥ = 94,
	ѰѪ��ϥ = 90,

	������� = 40,
	�����Ҽ� = 15,
	������ϥ = 97,
	������ϥ = 92,

	̽·��� = 28,
	̽·�Ҽ� = 14,
	̽·��ϥ = 77,
	̽·��ϥ = 70,

	������ = 44,
	����Ҽ� = 15,
	�����ϥ = 87,
	�����ϥ = 82,

	��ʴ��� = 38,
	��ʴ�Ҽ� = 14,
	��ʴ��ϥ = 81,
	��ʴ��ϥ = 76,

	������� = 41,
	�����Ҽ� = 14,
	������ϥ = 91,
	������ϥ = 86,

	������� = 44, // 44
	�����Ҽ� = 15,
	������ϥ = 92, // 92
	������ϥ = 82, // 82

	��ɭ��� = 69,
	��ɭ�Ҽ� = 44,
	��ɭ��ϥ = 116, // 92
	��ɭ��ϥ = 110, // 82
};
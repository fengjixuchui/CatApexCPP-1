#include "proc_help.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <windows.h>
#include <random>
#include "imgui/imgui.h"
#include "hackerThreads.h"

ColorStruct colors;
AppConfigs appConfigs;
int menu_zuobian;
int menu_dingbian;
int menu_kuan;
int menu_gao;
int menu_hangju;
int menu_select;
int menu_size;
int menu_fenge;
int nengnenglvse;
int nengnenglanse;
std::hash_map<int, ApexItem> entityNames;
HANDLE hAimThread;


int Ñ°Ñª[3][16] = {};
int Ö±²¼[3][16] = {};
int ÃüÂö[3][16] = {};
int Ì½Â·[3][16] = {};
int ¶¯Á¦[3][16] = {};

int ¶ñÁé[3][16] = {};
int °à¼Ó[3][16] = {};
int ÇÖÊ´[3][16] = {};
int »ÃÏó[3][16] = {};
int »ªÉ­[3][16] = {};


typedef struct __KeyBoardHookStruct {
	int vk_Code;
	int scan_Code;
	int flags;
	int time;
	int dwExtraInfo;
} KeyBoardHookStruct;

using namespace std;

LRESULT CALLBACK keyboardHook(_In_  int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam);
DWORD WINAPI KeyBoardHookThread(LPVOID lpParam);

HHOOK keyHook = 0;

DWORD GetProcessIDByName(const char *pName) {
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return 0;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		_bstr_t info_p_Name(pe.szExeFile);
		if (strcmp(info_p_Name, pName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
		//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
	}
	CloseHandle(hSnapshot);
	return 0;
}

void initColor() {
	colors = { ImColor({255, 255, 95}), ImColor({255, 0, 255}), ImColor({188, 102, 255}), ImColor({0, 191, 255}),
			  ImColor({255, 255, 255}), ImColor({197, 219, 70}), ImColor({111, 205, 168}), ImColor({106, 221, 34}),
			  ImColor({255, 188, 0}) };
}

void initConfig() {
	appConfigs = { true, true, 600.f, true, true, 50.f, true, true, true, false, true, true, 150.f, 2, true };
	DWORD tid = 0;
	HANDLE tHandle = CreateThread(NULL, 0, KeyBoardHookThread, 0, 0, &tid);
	CloseHandle(tHandle);
}

DWORD WINAPI KeyBoardHookThread(LPVOID lpParam) {
	keyHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHook, 0, 0);
	if (keyHook == 0) {
		MessageBoxA(NULL, "°²×°¼üÅÌ¹³×ÓÊ§°Ü", NULL, 0);
		return 0;
	}
	MSG msg;
	ZeroMemory(&msg, sizeof(msg));
	while (msg.message != WM_QUIT) {
		if (::PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE)) {
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
			continue;
		}
		Sleep(1);
	}
	return 0;

}

LRESULT CALLBACK keyboardHook(_In_ int nCode, _In_ WPARAM wParam, _In_ LPARAM lParam) {
	if (nCode == 0 && wParam == 256) {
		KeyBoardHookStruct *kbh = (KeyBoardHookStruct *)lParam;
		switch (kbh->vk_Code) {
		case VK_F1:
			appConfigs.FangKuang = !appConfigs.FangKuang;
			break;
		case VK_F2:
			if (appConfigs.TouShiFanWei == 1000) {
				appConfigs.TouShiFanWei = 100;
				break;
			}
			appConfigs.TouShiFanWei += 100;
			break;
		case VK_F3:
			appConfigs.WuPingTouShi = !appConfigs.WuPingTouShi;
			break;
		case VK_F4:
			if (appConfigs.WuPingFanWei == 600) {
				appConfigs.WuPingFanWei = 0;
				break;
			}
			appConfigs.WuPingFanWei += 50;
			break;
		case VK_F5:
			appConfigs.PeiJianTouShi = !appConfigs.PeiJianTouShi;
			break;
		case VK_F6:
			appConfigs.ZiDongMiaoZhun = !appConfigs.ZiDongMiaoZhun;
			break;
		case VK_F7:
			appConfigs.XianShiZhaZhu = !appConfigs.XianShiZhaZhu;
			break;
		case VK_F12:
			appConfigs.KaiFaZheXuanXiang = !appConfigs.KaiFaZheXuanXiang;
			break;
		case VK_HOME:
			appConfigs.MenuStatus = !appConfigs.MenuStatus;
			break;
		}
	}
	return CallNextHookEx(keyHook, nCode, wParam, lParam);
}

void initMenu() {
	int ju = fontSize * 6;
	menu_zuobian = 10;
	menu_kuan = ju + fontSize * 2.65f + 10;
	menu_gao = fontSize * 13 + 7;
	menu_fenge = 2;
	menu_dingbian = 150;
	menu_hangju = 16;
	menu_select = 1;
	menu_size = 27;
	nengnenglvse = 10091863;
	nengnenglanse = 16088846;

}

void startThreads() {
	DWORD tid = 0;
	HANDLE tHandle = CreateThread(NULL, 0, InfoThread, 0, 0, &tid);
	CloseHandle(tHandle);
	tHandle = CreateThread(NULL, 0, EntityManager, 0, 0, &tid);
	CloseHandle(tHandle);
	tHandle = CreateThread(NULL, 0, HentaiThread, 0, 0, &tid);
	CloseHandle(tHandle);
	hAimThread = CreateThread(NULL, 0, SuperAim, 0, 0, &tid);

}

void rand_str(char *str, const int len)
{
	int i;
	for (i = 0; i < len; ++i)
	{
		switch ((getRandomInt(0, RAND_MAX) % 3))
		{
		case 1:
			str[i] = 'A' + getRandomInt(0, RAND_MAX) % 26;
			break;
		case 2:
			str[i] = 'a' + getRandomInt(0, RAND_MAX) % 26;
			break;
		default:
			str[i] = '0' + getRandomInt(0, RAND_MAX) % 10;
			break;
		}
	}
	str[++i] = '\0';
}

wchar_t* char2wchar_t(char* cstr) {
	int len = MultiByteToWideChar(CP_ACP, 0, cstr, strlen(cstr), NULL, 0);
	wchar_t * wstr = new wchar_t[len + 1];
	MultiByteToWideChar(CP_ACP, 0, cstr, strlen(cstr), wstr, len);
	wstr[len] = '\0';
	return wstr;
}

void usleep(unsigned int usec)
{
	HANDLE timer;
	LARGE_INTEGER ft;

	ft.QuadPart = -(10 * (__int64)usec);

	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

int getRandomInt(int min, int max) {
	std::random_device rd;
	std::mt19937 mt(rd());
	return mt() % (max - min + 1) + min;
}

void initEntityNames() {
	entityNames[1] = { u8"AWM", colors.ShiShi };
	entityNames[2] = { u8"¿ÕÍ¶Åç×Ó", colors.ShiShi };
	entityNames[3] = { u8"¿ÕÍ¶¹·Åç×Ó", colors.ShiShi };
	entityNames[4] = { u8"¹þÎÖ¿Ë", colors.NengLiang };
	entityNames[5] = { u8"¹þÎÖ¿Ë[½ð]", colors.ShiShi };
	entityNames[6] = { u8"Çá»úÇ¹", colors.NengLiang };
	entityNames[8] = { u8"ÈýÖØ¾Ñ", colors.NengLiang };
	entityNames[10] = { u8"VK47", colors.ZhongXing };
	entityNames[12] = { u8"SLÈýÁ¬·¢", colors.ZhongXing };
	entityNames[13] = { u8"G7¾Ñ»÷Ç¹", colors.QingXing };
	entityNames[15] = { u8"×ª»»Õß³å·æÇ¹", colors.QingXing };
	entityNames[17] = { u8"R99³å·æÇ¹", colors.QingXing };
	entityNames[18] = { u8"ÁÔÊÞ³å·æÇ¹", colors.ZhongXing };
	entityNames[19] = { u8"Ç±Ï®³å·æÇ¹[½ð]", colors.ShiShi };
	entityNames[20] = { u8"³¤¹­¾Ñ»÷Ç¹", colors.ZhongXing };
	entityNames[22] = { u8"M600", colors.ShiShi };
	entityNames[23] = { u8"R301", colors.QingXing };
	entityNames[28] = { u8"ÄªÉ£±È¿Ë", colors.SanDan };
	entityNames[26] = { u8"ºÍÆ½º´ÎÀÕß", colors.SanDan };
	entityNames[25] = { u8"S12k", colors.SanDan };
	entityNames[30] = { u8"ÎÞµÐ×óÂÖ", colors.ZhongXing };
	entityNames[31] = { u8"ÎÞµÐ×óÂÖ[½ð]", colors.ShiShi };
	entityNames[34] = { u8"R45", colors.QingXing };
	entityNames[32] = { u8"p2020", colors.QingXing };

	entityNames[36] = { u8"ÇáÐÍµ¯Ò©", colors.QingXing };
	entityNames[37] = { u8"ÄÜÁ¿µ¯Ò©", colors.NengLiang };
	entityNames[38] = { u8"ö±µ¯Ç¹µ¯Ò©", colors.SanDan };
	entityNames[39] = { u8"ÖØÐÍµ¯Ò©", colors.ZhongXing };

	entityNames[41] = { u8"·ï»Ë", colors.ShengQi };
	entityNames[42] = { u8"´ó¼±¾ÈÏä", colors.ShengQi };
	entityNames[43] = { u8"×¢ÉäÆ÷", colors.ShengQi };
	entityNames[44] = { u8"´ó»¤¶Üµç³Ø", colors.ShengQi };
	entityNames[45] = { u8"Ð¡»¤¶Üµç³Ø", colors.ShengQi };

	entityNames[46] = { u8"Í· lv.1", colors.PuTong };
	entityNames[47] = { u8"Í· lv.2", colors.GaoJi };
	entityNames[48] = { u8"Í· lv.3", colors.ShiShi };
	entityNames[49] = { u8"Í· lv.4", colors.ShiShi };

	entityNames[50] = { u8"¼× lv.1", colors.PuTong };
	entityNames[51] = { u8"¼× lv.2", colors.GaoJi };
	entityNames[52] = { u8"¼× lv.3", colors.ShiShi };
	entityNames[53] = { u8"¼× lv.4", colors.ShiShi };

	entityNames[54] = { u8"¶Ü lv.1", colors.PuTong };
	entityNames[55] = { u8"¶Ü lv.2", colors.GaoJi };
	entityNames[56] = { u8"¶Ü lv.3", colors.ShengQi };
	entityNames[57] = { u8"¶Ü lv.4", colors.ShiShi };

	entityNames[58] = { u8"°ü lv.1", colors.PuTong };
	entityNames[59] = { u8"°ü lv.2", colors.GaoJi };
	entityNames[60] = { u8"°ü lv.3", colors.ShiShi };
	entityNames[61] = { u8"°ü lv.4", colors.ShiShi };

	entityNames[62] = { u8"ÂÁÈÈ¼Á", colors.GaoJi };
	entityNames[63] = { u8"À×", colors.GaoJi };
	entityNames[64] = { u8"µç", colors.GaoJi };
	entityNames[65] = { u8"ºìµã", colors.GaoJi };
	entityNames[66] = { u8"2x", colors.GaoJi };
	entityNames[67] = { u8"È«Ï¢", colors.GaoJi };

	entityNames[68] = { u8"1-2x", colors.ShengQi };
	entityNames[69] = { u8"1-2x", colors.ShengQi };
	entityNames[70] = { u8"3x", colors.GaoJi };
	entityNames[71] = { u8"2-4x", colors.GaoJi };
	entityNames[72] = { u8"6x", colors.ShengQi };
	entityNames[73] = { u8"4-8x", colors.ShengQi };
	entityNames[74] = { u8"4-10x", colors.ShengQi };
	entityNames[75] = { u8"Ç¹¹ÜÎÈ¶¨", colors.PuTong };
	entityNames[76] = { u8"Ç¹¹ÜÎÈ¶¨Lv.2", colors.GaoJi };
	entityNames[77] = { u8"Ç¹¹ÜÎÈ¶¨Lv.3", colors.ShengQi };
	entityNames[78] = { u8"Ç¹¹ÜÎÈ¶¨Lv.4", colors.ShengQi };
	entityNames[79] = { u8"ÇáÐÍÀ©ÈÝLv.1", colors.PuTong };
	entityNames[80] = { u8"ÇáÐÍÀ©ÈÝLv.2", colors.GaoJi };
	entityNames[81] = { u8"ÇáÐÍÀ©ÈÝLv.3", colors.ShiShi };

	entityNames[82] = { u8"ÖØÐÍÀ©ÈÝLv.1", colors.PuTong };
	entityNames[83] = { u8"ÖØÐÍÀ©ÈÝLv.2", colors.GaoJi };
	entityNames[84] = { u8"ÖØÐÍÀ©ÈÝLv.3", colors.ShiShi };

	entityNames[85] = { u8"ÄÜÁ¿À©ÈÝLv.1", colors.PuTong };
	entityNames[86] = { u8"ÄÜÁ¿À©ÈÝLv.2", colors.GaoJi };
	entityNames[87] = { u8"ÄÜÁ¿À©ÈÝLv.3", colors.ShengQi };

	entityNames[88] = { u8"ö±µ¯Ç¹Í·Lv.1", colors.PuTong };
	entityNames[89] = { u8"ö±µ¯Ç¹Í·Lv.2", colors.GaoJi };
	entityNames[90] = { u8"ö±µ¯Ç¹Í·Lv.3", colors.ShengQi };

	entityNames[91] = { u8"Ç¹Æ¨¹ÉLv.1", colors.PuTong };
	entityNames[92] = { u8"Ç¹Æ¨¹ÉLv.2", colors.GaoJi };
	entityNames[93] = { u8"Ç¹Æ¨¹ÉLv.3", colors.ShengQi };

	entityNames[94] = { u8"¾ÑÆ¨¹ÉLv.1", colors.PuTong };
	entityNames[95] = { u8"¾ÑÆ¨¹ÉLv.2", colors.GaoJi };
	entityNames[96] = { u8"¾ÑÆ¨¹ÉLv.3", colors.ShengQi };

	entityNames[97] = { u8"ÎÐÂÖÔöÑ¹Æ÷", colors.ShengQi };
	entityNames[98] = { u8"Éä»÷Ä£Ê½Ñ¡ÔñÆ÷", colors.ShengQi };
	entityNames[99] = { u8"¾«×¼ÊÕÊøÆ÷", colors.ShengQi };
	entityNames[100] = { u8"±¬Í·ÉËº¦Åä¼þ", colors.ShengQi };
	entityNames[101] = { u8"¸ÉÈÅÕßµ¯Ò©", colors.ShengQi };
	entityNames[102] = { u8"´¸µãµ¯Ò©", colors.ShengQi };

}

void initBones() {
	//Ñ°Ñª
	{
		Ñ°Ñª[0][0] = Bones::head;
		Ñ°Ñª[0][1] = Bones::neck;
		Ñ°Ñª[0][2] = Bones::ÐØ;
		Ñ°Ñª[0][3] = Bones::¸¹;
		Ñ°Ñª[0][4] = Bones::ŒÅ;

		Ñ°Ñª[1][0] = Bones::Ñ°Ñª×ó¼ç + 7;
		Ñ°Ñª[1][1] = Bones::Ñ°Ñª×ó¼ç + 5;
		Ñ°Ñª[1][2] = Bones::Ñ°Ñª×ó¼ç + 4;
		Ñ°Ñª[1][3] = Bones::Ñ°Ñª×ó¼ç + 2;
		Ñ°Ñª[1][4] = Bones::Ñ°Ñª×ó¼ç + 3;
		Ñ°Ñª[1][5] = Bones::Ñ°Ñª×ó¼ç + 1;
		Ñ°Ñª[1][6] = Bones::Ñ°Ñª×ó¼ç;
		Ñ°Ñª[1][7] = Bones::Ñ°Ñª×ó¼ç - 2;

		Ñ°Ñª[1][8] = Bones::Ñ°ÑªÓÒ¼ç - 2;
		Ñ°Ñª[1][9] = Bones::Ñ°ÑªÓÒ¼ç;
		Ñ°Ñª[1][10] = Bones::Ñ°ÑªÓÒ¼ç + 1;
		Ñ°Ñª[1][11] = Bones::Ñ°ÑªÓÒ¼ç + 3;
		Ñ°Ñª[1][12] = Bones::Ñ°ÑªÓÒ¼ç + 2;
		Ñ°Ñª[1][13] = Bones::Ñ°ÑªÓÒ¼ç + 4;
		Ñ°Ñª[1][14] = Bones::Ñ°ÑªÓÒ¼ç + 5;
		Ñ°Ñª[1][15] = Bones::Ñ°ÑªÓÒ¼ç + 7;

		Ñ°Ñª[2][0] = Bones::Ñ°Ñª×óÏ¥ + 2;
		Ñ°Ñª[2][1] = Bones::Ñ°Ñª×óÏ¥ + 1;
		Ñ°Ñª[2][2] = Bones::Ñ°Ñª×óÏ¥;
		Ñ°Ñª[2][3] = Bones::ŒÅ;
		Ñ°Ñª[2][4] = Bones::Ñ°ÑªÓÒÏ¥;
		Ñ°Ñª[2][5] = Bones::Ñ°ÑªÓÒÏ¥ + 1;
		Ñ°Ñª[2][6] = Bones::Ñ°ÑªÓÒÏ¥ + 2;
	}
	//Ö±²¼
	{
		Ö±²¼[0][0] = Bones::head;
		Ö±²¼[0][1] = Bones::neck;
		Ö±²¼[0][2] = Bones::ÐØ;
		Ö±²¼[0][3] = Bones::¸¹;
		Ö±²¼[0][4] = Bones::ŒÅ;

		Ö±²¼[1][0] = Bones::Ö±²¼×ó¼ç + 7;
		Ö±²¼[1][1] = Bones::Ö±²¼×ó¼ç + 5;
		Ö±²¼[1][2] = Bones::Ö±²¼×ó¼ç + 4;
		Ö±²¼[1][3] = Bones::Ö±²¼×ó¼ç + 2;
		Ö±²¼[1][4] = Bones::Ö±²¼×ó¼ç + 3;
		Ö±²¼[1][5] = Bones::Ö±²¼×ó¼ç + 1;
		Ö±²¼[1][6] = Bones::Ö±²¼×ó¼ç;
		Ö±²¼[1][7] = Bones::Ö±²¼×ó¼ç - 2;

		Ö±²¼[1][8] = Bones::Ö±²¼ÓÒ¼ç - 2;
		Ö±²¼[1][9] = Bones::Ö±²¼ÓÒ¼ç;
		Ö±²¼[1][10] = Bones::Ö±²¼ÓÒ¼ç + 1;
		Ö±²¼[1][11] = Bones::Ö±²¼ÓÒ¼ç + 3;
		Ö±²¼[1][12] = Bones::Ö±²¼ÓÒ¼ç + 2;
		Ö±²¼[1][13] = Bones::Ö±²¼ÓÒ¼ç + 4;
		Ö±²¼[1][14] = Bones::Ö±²¼ÓÒ¼ç + 5;
		Ö±²¼[1][15] = Bones::Ö±²¼ÓÒ¼ç + 7;

		Ö±²¼[2][0] = Bones::Ö±²¼×óÏ¥ + 2;
		Ö±²¼[2][1] = Bones::Ö±²¼×óÏ¥ + 1;
		Ö±²¼[2][2] = Bones::Ö±²¼×óÏ¥;
		Ö±²¼[2][3] = Bones::ŒÅ;
		Ö±²¼[2][4] = Bones::Ö±²¼ÓÒÏ¥;
		Ö±²¼[2][5] = Bones::Ö±²¼ÓÒÏ¥ + 1;
		Ö±²¼[2][6] = Bones::Ö±²¼ÓÒÏ¥ + 2;
	}
	//ÃüÂö
	{
		ÃüÂö[0][0] = Bones::head;
		ÃüÂö[0][1] = Bones::neck;
		ÃüÂö[0][2] = Bones::ÐØ;
		ÃüÂö[0][3] = Bones::¸¹;
		ÃüÂö[0][4] = Bones::ŒÅ;

		ÃüÂö[1][0] = Bones::ÃüÂö×ó¼ç + 7;
		ÃüÂö[1][1] = Bones::ÃüÂö×ó¼ç + 5;
		ÃüÂö[1][2] = Bones::ÃüÂö×ó¼ç + 4;
		ÃüÂö[1][3] = Bones::ÃüÂö×ó¼ç + 2;
		ÃüÂö[1][4] = Bones::ÃüÂö×ó¼ç + 3;
		ÃüÂö[1][5] = Bones::ÃüÂö×ó¼ç + 1;
		ÃüÂö[1][6] = Bones::ÃüÂö×ó¼ç;
		ÃüÂö[1][7] = Bones::ÃüÂö×ó¼ç - 2;

		ÃüÂö[1][8] = Bones::ÃüÂöÓÒ¼ç - 2;
		ÃüÂö[1][9] = Bones::ÃüÂöÓÒ¼ç;
		ÃüÂö[1][10] = Bones::ÃüÂöÓÒ¼ç + 1;
		ÃüÂö[1][11] = Bones::ÃüÂöÓÒ¼ç + 3;
		ÃüÂö[1][12] = Bones::ÃüÂöÓÒ¼ç + 2;
		ÃüÂö[1][13] = Bones::ÃüÂöÓÒ¼ç + 4;
		ÃüÂö[1][14] = Bones::ÃüÂöÓÒ¼ç + 5;
		ÃüÂö[1][15] = Bones::ÃüÂöÓÒ¼ç + 7;

		ÃüÂö[2][0] = Bones::ÃüÂö×óÏ¥ + 2;
		ÃüÂö[2][1] = Bones::ÃüÂö×óÏ¥ + 1;
		ÃüÂö[2][2] = Bones::ÃüÂö×óÏ¥;
		ÃüÂö[2][3] = Bones::ŒÅ;
		ÃüÂö[2][4] = Bones::ÃüÂöÓÒÏ¥;
		ÃüÂö[2][5] = Bones::ÃüÂöÓÒÏ¥ + 1;
		ÃüÂö[2][6] = Bones::ÃüÂöÓÒÏ¥ + 2;
	}
	//Ì½Â·
	{
		Ì½Â·[0][0] = Bones::head;
		Ì½Â·[0][1] = Bones::neck;
		Ì½Â·[0][2] = Bones::ÐØ;
		Ì½Â·[0][3] = Bones::¸¹;
		Ì½Â·[0][4] = Bones::ŒÅ;

		Ì½Â·[1][0] = Bones::Ì½Â·×ó¼ç + 6;
		Ì½Â·[1][1] = Bones::Ì½Â·×ó¼ç + 5;
		Ì½Â·[1][2] = Bones::Ì½Â·×ó¼ç + 4;
		Ì½Â·[1][3] = Bones::Ì½Â·×ó¼ç + 3;
		Ì½Â·[1][4] = Bones::Ì½Â·×ó¼ç + 2;
		Ì½Â·[1][5] = Bones::Ì½Â·×ó¼ç + 1;
		Ì½Â·[1][6] = Bones::Ì½Â·×ó¼ç;
		Ì½Â·[1][7] = Bones::Ì½Â·×ó¼ç - 1;

		Ì½Â·[1][8] = Bones::Ì½Â·ÓÒ¼ç - 1;
		Ì½Â·[1][9] = Bones::Ì½Â·ÓÒ¼ç;
		Ì½Â·[1][10] = Bones::Ì½Â·ÓÒ¼ç + 1;
		Ì½Â·[1][11] = Bones::Ì½Â·ÓÒ¼ç + 2;
		Ì½Â·[1][12] = Bones::Ì½Â·ÓÒ¼ç + 3;
		Ì½Â·[1][13] = Bones::Ì½Â·ÓÒ¼ç + 4;
		Ì½Â·[1][14] = Bones::Ì½Â·ÓÒ¼ç + 5;
		Ì½Â·[1][15] = Bones::Ì½Â·ÓÒ¼ç + 6;

		Ì½Â·[2][0] = Bones::Ì½Â·×óÏ¥ + 2;
		Ì½Â·[2][1] = Bones::Ì½Â·×óÏ¥ + 1;
		Ì½Â·[2][2] = Bones::Ì½Â·×óÏ¥;
		Ì½Â·[2][3] = Bones::ŒÅ;
		Ì½Â·[2][4] = Bones::Ì½Â·ÓÒÏ¥;
		Ì½Â·[2][5] = Bones::Ì½Â·ÓÒÏ¥ + 1;
		Ì½Â·[2][6] = Bones::Ì½Â·ÓÒÏ¥ + 2;
	}
	//¶¯Á¦
	{
		¶¯Á¦[0][0] = Bones::head;
		¶¯Á¦[0][1] = Bones::neck;
		¶¯Á¦[0][2] = Bones::ÐØ;
		¶¯Á¦[0][3] = Bones::¸¹;
		¶¯Á¦[0][4] = Bones::ŒÅ;

		¶¯Á¦[1][0] = Bones::¶¯Á¦×ó¼ç + 7;
		¶¯Á¦[1][1] = Bones::¶¯Á¦×ó¼ç + 5;
		¶¯Á¦[1][2] = Bones::¶¯Á¦×ó¼ç + 4;
		¶¯Á¦[1][3] = Bones::¶¯Á¦×ó¼ç + 2;
		¶¯Á¦[1][4] = Bones::¶¯Á¦×ó¼ç + 3;
		¶¯Á¦[1][5] = Bones::¶¯Á¦×ó¼ç + 1;
		¶¯Á¦[1][6] = Bones::¶¯Á¦×ó¼ç;
		¶¯Á¦[1][7] = Bones::¶¯Á¦×ó¼ç - 2;

		¶¯Á¦[1][8] = Bones::¶¯Á¦ÓÒ¼ç - 2;
		¶¯Á¦[1][9] = Bones::¶¯Á¦ÓÒ¼ç;
		¶¯Á¦[1][10] = Bones::¶¯Á¦ÓÒ¼ç + 1;
		¶¯Á¦[1][11] = Bones::¶¯Á¦ÓÒ¼ç + 3;
		¶¯Á¦[1][12] = Bones::¶¯Á¦ÓÒ¼ç + 2;
		¶¯Á¦[1][13] = Bones::¶¯Á¦ÓÒ¼ç + 4;
		¶¯Á¦[1][14] = Bones::¶¯Á¦ÓÒ¼ç + 5;
		¶¯Á¦[1][15] = Bones::¶¯Á¦ÓÒ¼ç + 7;

		¶¯Á¦[2][0] = Bones::¶¯Á¦×óÏ¥ - 3;
		¶¯Á¦[2][1] = Bones::¶¯Á¦×óÏ¥ - 2;
		¶¯Á¦[2][2] = Bones::¶¯Á¦×óÏ¥;
		¶¯Á¦[2][3] = Bones::ŒÅ;
		¶¯Á¦[2][4] = Bones::¶¯Á¦ÓÒÏ¥;
		¶¯Á¦[2][5] = Bones::¶¯Á¦ÓÒÏ¥ + 1;
		¶¯Á¦[2][6] = Bones::¶¯Á¦ÓÒÏ¥ + 2;
	}
	//¶ñÁé
	{
		¶ñÁé[0][0] = Bones::head;
		¶ñÁé[0][1] = Bones::neck;
		¶ñÁé[0][2] = Bones::ÐØ;
		¶ñÁé[0][3] = Bones::¸¹;
		¶ñÁé[0][4] = Bones::ŒÅ;

		¶ñÁé[1][0] = Bones::¶ñÁé×ó¼ç + 7;
		¶ñÁé[1][1] = Bones::¶ñÁé×ó¼ç + 5;
		¶ñÁé[1][2] = Bones::¶ñÁé×ó¼ç + 4;
		¶ñÁé[1][3] = Bones::¶ñÁé×ó¼ç + 2;
		¶ñÁé[1][4] = Bones::¶ñÁé×ó¼ç + 3;
		¶ñÁé[1][5] = Bones::¶ñÁé×ó¼ç + 1;
		¶ñÁé[1][6] = Bones::¶ñÁé×ó¼ç;
		¶ñÁé[1][7] = Bones::¶ñÁé×ó¼ç - 2;

		¶ñÁé[1][8] = Bones::¶ñÁéÓÒ¼ç - 2;
		¶ñÁé[1][9] = Bones::¶ñÁéÓÒ¼ç;
		¶ñÁé[1][10] = Bones::¶ñÁéÓÒ¼ç + 1;
		¶ñÁé[1][11] = Bones::¶ñÁéÓÒ¼ç + 3;
		¶ñÁé[1][12] = Bones::¶ñÁéÓÒ¼ç + 2;
		¶ñÁé[1][13] = Bones::¶ñÁéÓÒ¼ç + 4;
		¶ñÁé[1][14] = Bones::¶ñÁéÓÒ¼ç + 5;
		¶ñÁé[1][15] = Bones::¶ñÁéÓÒ¼ç + 7;

		¶ñÁé[2][0] = Bones::¶ñÁé×óÏ¥ + 2;
		¶ñÁé[2][1] = Bones::¶ñÁé×óÏ¥ + 1;
		¶ñÁé[2][2] = Bones::¶ñÁé×óÏ¥;
		¶ñÁé[2][3] = Bones::ŒÅ;
		¶ñÁé[2][4] = Bones::¶ñÁéÓÒÏ¥;
		¶ñÁé[2][5] = Bones::¶ñÁéÓÒÏ¥ + 1;
		¶ñÁé[2][6] = Bones::¶ñÁéÓÒÏ¥ + 2;
	}
	//°à¼Ó
	{
		°à¼Ó[0][0] = Bones::head;
		°à¼Ó[0][1] = Bones::neck;
		°à¼Ó[0][2] = Bones::ÐØ;
		°à¼Ó[0][3] = Bones::¸¹;
		°à¼Ó[0][4] = Bones::ŒÅ;

		°à¼Ó[1][0] = Bones::°à¼Ó×ó¼ç + 7;
		°à¼Ó[1][1] = Bones::°à¼Ó×ó¼ç + 5;
		°à¼Ó[1][2] = Bones::°à¼Ó×ó¼ç + 4;
		°à¼Ó[1][3] = Bones::°à¼Ó×ó¼ç + 2;
		°à¼Ó[1][4] = Bones::°à¼Ó×ó¼ç + 3;
		°à¼Ó[1][5] = Bones::°à¼Ó×ó¼ç + 1;
		°à¼Ó[1][6] = Bones::°à¼Ó×ó¼ç;
		°à¼Ó[1][7] = Bones::°à¼Ó×ó¼ç - 2;

		°à¼Ó[1][8] = Bones::°à¼ÓÓÒ¼ç - 2;
		°à¼Ó[1][9] = Bones::°à¼ÓÓÒ¼ç;
		°à¼Ó[1][10] = Bones::°à¼ÓÓÒ¼ç + 1;
		°à¼Ó[1][11] = Bones::°à¼ÓÓÒ¼ç + 3;
		°à¼Ó[1][12] = Bones::°à¼ÓÓÒ¼ç + 2;
		°à¼Ó[1][13] = Bones::°à¼ÓÓÒ¼ç + 4;
		°à¼Ó[1][14] = Bones::°à¼ÓÓÒ¼ç + 5;
		°à¼Ó[1][15] = Bones::°à¼ÓÓÒ¼ç + 7;

		°à¼Ó[2][0] = Bones::°à¼Ó×óÏ¥ + 2;
		°à¼Ó[2][1] = Bones::°à¼Ó×óÏ¥ + 1;
		°à¼Ó[2][2] = Bones::°à¼Ó×óÏ¥;
		°à¼Ó[2][3] = Bones::ŒÅ;
		°à¼Ó[2][4] = Bones::°à¼ÓÓÒÏ¥;
		°à¼Ó[2][5] = Bones::°à¼ÓÓÒÏ¥ + 1;
		°à¼Ó[2][6] = Bones::°à¼ÓÓÒÏ¥ + 2;
	}
	//ÇÖÊ´
	{
		ÇÖÊ´[0][0] = Bones::head;
		ÇÖÊ´[0][1] = Bones::neck;
		ÇÖÊ´[0][2] = Bones::ÐØ;
		ÇÖÊ´[0][3] = Bones::¸¹;
		ÇÖÊ´[0][4] = Bones::ŒÅ;

		ÇÖÊ´[1][0] = Bones::ÇÖÊ´×ó¼ç + 7;
		ÇÖÊ´[1][1] = Bones::ÇÖÊ´×ó¼ç + 5;
		ÇÖÊ´[1][2] = Bones::ÇÖÊ´×ó¼ç + 4;
		ÇÖÊ´[1][3] = Bones::ÇÖÊ´×ó¼ç + 2;
		ÇÖÊ´[1][4] = Bones::ÇÖÊ´×ó¼ç + 3;
		ÇÖÊ´[1][5] = Bones::ÇÖÊ´×ó¼ç + 1;
		ÇÖÊ´[1][6] = Bones::ÇÖÊ´×ó¼ç;
		ÇÖÊ´[1][7] = Bones::ÇÖÊ´×ó¼ç - 2;

		ÇÖÊ´[1][8] = Bones::ÇÖÊ´ÓÒ¼ç - 2;
		ÇÖÊ´[1][9] = Bones::ÇÖÊ´ÓÒ¼ç;
		ÇÖÊ´[1][10] = Bones::ÇÖÊ´ÓÒ¼ç + 1;
		ÇÖÊ´[1][11] = Bones::ÇÖÊ´ÓÒ¼ç + 3;
		ÇÖÊ´[1][12] = Bones::ÇÖÊ´ÓÒ¼ç + 2;
		ÇÖÊ´[1][13] = Bones::ÇÖÊ´ÓÒ¼ç + 4;
		ÇÖÊ´[1][14] = Bones::ÇÖÊ´ÓÒ¼ç + 5;
		ÇÖÊ´[1][15] = Bones::ÇÖÊ´ÓÒ¼ç + 7;

		ÇÖÊ´[2][0] = Bones::ÇÖÊ´×óÏ¥ + 2;
		ÇÖÊ´[2][1] = Bones::ÇÖÊ´×óÏ¥ + 1;
		ÇÖÊ´[2][2] = Bones::ÇÖÊ´×óÏ¥;
		ÇÖÊ´[2][3] = Bones::ŒÅ;
		ÇÖÊ´[2][4] = Bones::ÇÖÊ´ÓÒÏ¥;
		ÇÖÊ´[2][5] = Bones::ÇÖÊ´ÓÒÏ¥ + 1;
		ÇÖÊ´[2][6] = Bones::ÇÖÊ´ÓÒÏ¥ + 2;
	}
	//»ÃÏó
	{
		»ÃÏó[0][0] = Bones::head;
		»ÃÏó[0][1] = Bones::neck;
		»ÃÏó[0][2] = Bones::ÐØ;
		»ÃÏó[0][3] = Bones::¸¹;
		»ÃÏó[0][4] = Bones::ŒÅ;

		»ÃÏó[1][0] = Bones::»ÃÏó×ó¼ç + 7;
		»ÃÏó[1][1] = Bones::»ÃÏó×ó¼ç + 5;
		»ÃÏó[1][2] = Bones::»ÃÏó×ó¼ç + 4;
		»ÃÏó[1][3] = Bones::»ÃÏó×ó¼ç + 2;
		»ÃÏó[1][4] = Bones::»ÃÏó×ó¼ç + 3;
		»ÃÏó[1][5] = Bones::»ÃÏó×ó¼ç + 1;
		»ÃÏó[1][6] = Bones::»ÃÏó×ó¼ç;
		»ÃÏó[1][7] = Bones::»ÃÏó×ó¼ç - 1;

		»ÃÏó[1][8] = Bones::»ÃÏóÓÒ¼ç - 1;
		»ÃÏó[1][9] = Bones::»ÃÏóÓÒ¼ç;
		»ÃÏó[1][10] = Bones::»ÃÏóÓÒ¼ç + 1;
		»ÃÏó[1][11] = Bones::»ÃÏóÓÒ¼ç + 3;
		»ÃÏó[1][12] = Bones::»ÃÏóÓÒ¼ç + 2;
		»ÃÏó[1][13] = Bones::»ÃÏóÓÒ¼ç + 4;
		»ÃÏó[1][14] = Bones::»ÃÏóÓÒ¼ç + 5;
		»ÃÏó[1][15] = Bones::»ÃÏóÓÒ¼ç + 7;

		»ÃÏó[2][0] = Bones::»ÃÏó×óÏ¥ + 2;
		»ÃÏó[2][1] = Bones::»ÃÏó×óÏ¥ + 1;
		»ÃÏó[2][2] = Bones::»ÃÏó×óÏ¥;
		»ÃÏó[2][3] = Bones::ŒÅ;
		»ÃÏó[2][4] = Bones::»ÃÏóÓÒÏ¥;
		»ÃÏó[2][5] = Bones::»ÃÏóÓÒÏ¥ + 1;
		»ÃÏó[2][6] = Bones::»ÃÏóÓÒÏ¥ + 2;
	}
	//»ªÉ­
	{
		»ªÉ­[0][0] = Bones::head;
		»ªÉ­[0][1] = Bones::neck;
		»ªÉ­[0][2] = Bones::ÐØ;
		»ªÉ­[0][3] = Bones::¸¹;
		»ªÉ­[0][4] = Bones::ŒÅ;

		»ªÉ­[1][0] = Bones::»ªÉ­×ó¼ç + 7;
		»ªÉ­[1][1] = Bones::»ªÉ­×ó¼ç + 5;
		»ªÉ­[1][2] = Bones::»ªÉ­×ó¼ç + 4;
		»ªÉ­[1][3] = Bones::»ªÉ­×ó¼ç + 2;
		»ªÉ­[1][4] = Bones::»ªÉ­×ó¼ç + 3;
		»ªÉ­[1][5] = Bones::»ªÉ­×ó¼ç + 1;
		»ªÉ­[1][6] = Bones::»ªÉ­×ó¼ç;
		»ªÉ­[1][7] = Bones::»ªÉ­×ó¼ç - 2;

		»ªÉ­[1][8] = Bones::»ªÉ­ÓÒ¼ç - 2;
		»ªÉ­[1][9] = Bones::»ªÉ­ÓÒ¼ç;
		»ªÉ­[1][10] = Bones::»ªÉ­ÓÒ¼ç + 1;
		»ªÉ­[1][11] = Bones::»ªÉ­ÓÒ¼ç + 3;
		»ªÉ­[1][12] = Bones::»ªÉ­ÓÒ¼ç + 2;
		»ªÉ­[1][13] = Bones::»ªÉ­ÓÒ¼ç + 4;
		»ªÉ­[1][14] = Bones::»ªÉ­ÓÒ¼ç + 5;
		»ªÉ­[1][15] = Bones::»ªÉ­ÓÒ¼ç + 7;

		»ªÉ­[2][0] = Bones::»ªÉ­×óÏ¥ + 2;
		»ªÉ­[2][1] = Bones::»ªÉ­×óÏ¥ + 1;
		»ªÉ­[2][2] = Bones::»ªÉ­×óÏ¥;
		»ªÉ­[2][3] = Bones::ŒÅ;
		»ªÉ­[2][4] = Bones::»ªÉ­ÓÒÏ¥;
		»ªÉ­[2][5] = Bones::»ªÉ­ÓÒÏ¥ + 1;
		»ªÉ­[2][6] = Bones::»ªÉ­ÓÒÏ¥ + 2;
	}
}
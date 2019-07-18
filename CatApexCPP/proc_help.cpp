#include "proc_help.h"
#include <time.h>
#include <iostream>
#include <stdlib.h>
#include <string>
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

char *rand_str(char *str, const int len) {
	srand(time(NULL));
	int i;
	for (i = 0; i < len; ++i) {
		switch ((rand() % 3)) {
		case 1:
			str[i] = 'A' + rand() % 26;
			break;
		case 2:
			str[i] = 'a' + rand() % 26;
			break;
		default:
			str[i] = '0' + rand() % 10;
			break;
		}
	}
	str[++i] = '\0';
	return str;
}

int getRandom(int a, int b) {
	srand(time(NULL));
	return (rand() % (b - a)) + a;
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
		MessageBoxA(NULL, "安装键盘钩子失败", NULL, 0);
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
			appConfigs.DanwuSanShe = !appConfigs.DanwuSanShe;
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
	menu_kuan = ju + fontSize * 2.65 + 10;
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

void initEntityNames() {
	entityNames[1] = { u8"AWM", colors.ShiShi };
	entityNames[2] = { u8"空投喷子", colors.ShiShi };
	entityNames[3] = { u8"空投狗杂", colors.ShiShi };
	entityNames[4] = { u8"哈沃克", colors.NengLiang };
	entityNames[5] = { u8"哈沃克[金]", colors.ShiShi };
	entityNames[6] = { u8"轻机枪", colors.NengLiang };
	entityNames[8] = { u8"三重狙", colors.NengLiang };
	entityNames[10] = { u8"VK47", colors.ZhongXing };
	entityNames[12] = { u8"SL三连发", colors.ZhongXing };
	entityNames[13] = { u8"G7狙击枪", colors.QingXing };
	entityNames[15] = { u8"转换者冲锋枪", colors.QingXing };
	entityNames[17] = { u8"R99冲锋枪", colors.QingXing };
	entityNames[18] = { u8"猎兽冲锋枪", colors.ZhongXing };
	entityNames[19] = { u8"潜袭冲锋枪[金]", colors.ShiShi };
	entityNames[20] = { u8"长弓狙击枪", colors.ZhongXing };
	entityNames[22] = { u8"M600", colors.ShiShi };
	entityNames[23] = { u8"R301", colors.QingXing };
	entityNames[28] = { u8"莫桑比克", colors.SanDan };
	entityNames[26] = { u8"和平捍卫者", colors.SanDan };
	entityNames[25] = { u8"S12k", colors.SanDan };
	entityNames[30] = { u8"无敌左轮", colors.ZhongXing };
	entityNames[31] = { u8"无敌左轮[金]", colors.ShiShi };
	entityNames[34] = { u8"R45", colors.QingXing };
	entityNames[32] = { u8"p2020", colors.QingXing };

	entityNames[36] = { u8"轻型弹药", colors.QingXing };
	entityNames[37] = { u8"能量弹药", colors.NengLiang };
	entityNames[38] = { u8"霰弹枪弹药", colors.SanDan };
	entityNames[39] = { u8"重型弹药", colors.ZhongXing };

	entityNames[41] = { u8"凤凰", colors.ShengQi };
	entityNames[42] = { u8"大电脑", colors.ShengQi };
	entityNames[43] = { u8"注射器", colors.ShengQi };
	entityNames[44] = { u8"大电池", colors.ShengQi };
	entityNames[45] = { u8"小电池", colors.ShengQi };

	entityNames[46] = { u8"一级头", colors.PuTong };
	entityNames[47] = { u8"二级头", colors.GaoJi };
	entityNames[48] = { u8"三级头", colors.ShengQi };
	entityNames[49] = { u8"四级头", colors.ShiShi };

	entityNames[50] = { u8"一级甲", colors.PuTong };
	entityNames[51] = { u8"二级甲", colors.GaoJi };
	entityNames[52] = { u8"三级甲", colors.ShengQi };
	entityNames[53] = { u8"四级甲", colors.ShiShi };

	entityNames[54] = { u8"一级盾", colors.PuTong };
	entityNames[55] = { u8"二级盾", colors.GaoJi };
	entityNames[56] = { u8"三级盾", colors.ShengQi };
	entityNames[57] = { u8"四级盾", colors.ShiShi };

	entityNames[58] = { u8"一级包", colors.PuTong };
	entityNames[59] = { u8"二级包", colors.GaoJi };
	entityNames[60] = { u8"三级包", colors.ShengQi };
	entityNames[61] = { u8"四级包", colors.ShiShi };

	entityNames[62] = { u8"铝热剂", colors.GaoJi };
	entityNames[63] = { u8"雷", colors.GaoJi };
	entityNames[64] = { u8"电", colors.GaoJi };
	entityNames[65] = { u8"红点", colors.GaoJi };
	entityNames[66] = { u8"2x", colors.GaoJi };
	entityNames[67] = { u8"全息", colors.GaoJi };

	entityNames[68] = { u8"1-2x", colors.ShengQi };
	entityNames[69] = { u8"1-2x", colors.ShengQi };
	entityNames[70] = { u8"3x", colors.GaoJi };
	entityNames[71] = { u8"2-4x", colors.GaoJi };
	entityNames[72] = { u8"6x", colors.ShengQi };
	entityNames[73] = { u8"4-8x", colors.ShengQi };
	entityNames[74] = { u8"4-10x", colors.ShiShi };
	entityNames[75] = { u8"枪管稳定", colors.GaoJi };
	entityNames[76] = { u8"枪管稳定Lv.2", colors.GaoJi };
	entityNames[77] = { u8"枪管稳定Lv.3", colors.ShengQi };
	entityNames[78] = { u8"枪管稳定Lv.4", colors.ShiShi };
	entityNames[79] = { u8"轻型扩容", colors.GaoJi };
	entityNames[80] = { u8"轻型扩容Lv.2", colors.GaoJi };
	entityNames[81] = { u8"轻型扩容Lv.3", colors.ShengQi };

	entityNames[82] = { u8"重型扩容", colors.GaoJi };
	entityNames[83] = { u8"重型扩容Lv.2", colors.GaoJi };
	entityNames[84] = { u8"重型扩容Lv.3", colors.ShengQi };

	entityNames[85] = { u8"能量扩容", colors.GaoJi };
	entityNames[86] = { u8"能量扩容Lv.2", colors.GaoJi };
	entityNames[87] = { u8"能量扩容Lv.3", colors.ShengQi };

	entityNames[88] = { u8"霰弹枪头", colors.GaoJi };
	entityNames[89] = { u8"霰弹枪头Lv.2", colors.GaoJi };
	entityNames[90] = { u8"霰弹枪头Lv.3", colors.ShengQi };

	entityNames[91] = { u8"枪屁股", colors.GaoJi };
	entityNames[92] = { u8"枪屁股Lv.2", colors.GaoJi };
	entityNames[93] = { u8"枪屁股Lv.3", colors.ShengQi };

	entityNames[94] = { u8"狙屁股", colors.GaoJi };
	entityNames[95] = { u8"狙屁股Lv.2", colors.GaoJi };
	entityNames[96] = { u8"狙屁股Lv.3", colors.ShengQi };

	entityNames[97] = { u8"涡轮增压器", colors.ShengQi };
	entityNames[98] = { u8"射击模式选择器", colors.ShengQi };
	entityNames[99] = { u8"精准收束器", colors.ShengQi };
	entityNames[100] = { u8"爆头伤害配件", colors.ShengQi };
	entityNames[101] = { u8"干扰者弹药", colors.ShiShi };
	entityNames[102] = { u8"锤点弹药", colors.ShiShi };

}
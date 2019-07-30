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


int 寻血[3][16] = {};
int 直布[3][16] = {};
int 命脉[3][16] = {};
int 探路[3][16] = {};
int 动力[3][16] = {};

int 恶灵[3][16] = {};
int 班加[3][16] = {};
int 侵蚀[3][16] = {};
int 幻象[3][16] = {};
int 华森[3][16] = {};


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

void initBones() {
	//寻血
	{
		寻血[0][0] = Bones::头;
		寻血[0][1] = Bones::颈;
		寻血[0][2] = Bones::胸;
		寻血[0][3] = Bones::腹;
		寻血[0][4] = Bones::屌;

		寻血[1][0] = Bones::寻血左肩 + 7;
		寻血[1][1] = Bones::寻血左肩 + 5;
		寻血[1][2] = Bones::寻血左肩 + 4;
		寻血[1][3] = Bones::寻血左肩 + 2;
		寻血[1][4] = Bones::寻血左肩 + 3;
		寻血[1][5] = Bones::寻血左肩 + 1;
		寻血[1][6] = Bones::寻血左肩;
		寻血[1][7] = Bones::寻血左肩 - 2;

		寻血[1][8] = Bones::寻血右肩 - 2;
		寻血[1][9] = Bones::寻血右肩;
		寻血[1][10] = Bones::寻血右肩 + 1;
		寻血[1][11] = Bones::寻血右肩 + 3;
		寻血[1][12] = Bones::寻血右肩 + 2;
		寻血[1][13] = Bones::寻血右肩 + 4;
		寻血[1][14] = Bones::寻血右肩 + 5;
		寻血[1][15] = Bones::寻血右肩 + 7;

		寻血[2][0] = Bones::寻血左膝 + 2;
		寻血[2][1] = Bones::寻血左膝 + 1;
		寻血[2][2] = Bones::寻血左膝;
		寻血[2][3] = Bones::屌;
		寻血[2][4] = Bones::寻血右膝;
		寻血[2][5] = Bones::寻血右膝 + 1;
		寻血[2][6] = Bones::寻血右膝 + 2;
	}
	//直布
	{
		直布[0][0] = Bones::头;
		直布[0][1] = Bones::颈;
		直布[0][2] = Bones::胸;
		直布[0][3] = Bones::腹;
		直布[0][4] = Bones::屌;

		直布[1][0] = Bones::直布左肩 + 7;
		直布[1][1] = Bones::直布左肩 + 5;
		直布[1][2] = Bones::直布左肩 + 4;
		直布[1][3] = Bones::直布左肩 + 2;
		直布[1][4] = Bones::直布左肩 + 3;
		直布[1][5] = Bones::直布左肩 + 1;
		直布[1][6] = Bones::直布左肩;
		直布[1][7] = Bones::直布左肩 - 2;

		直布[1][8] = Bones::直布右肩 - 2;
		直布[1][9] = Bones::直布右肩;
		直布[1][10] = Bones::直布右肩 + 1;
		直布[1][11] = Bones::直布右肩 + 3;
		直布[1][12] = Bones::直布右肩 + 2;
		直布[1][13] = Bones::直布右肩 + 4;
		直布[1][14] = Bones::直布右肩 + 5;
		直布[1][15] = Bones::直布右肩 + 7;

		直布[2][0] = Bones::直布左膝 + 2;
		直布[2][1] = Bones::直布左膝 + 1;
		直布[2][2] = Bones::直布左膝;
		直布[2][3] = Bones::屌;
		直布[2][4] = Bones::直布右膝;
		直布[2][5] = Bones::直布右膝 + 1;
		直布[2][6] = Bones::直布右膝 + 2;
	}
	//命脉
	{
		命脉[0][0] = Bones::头;
		命脉[0][1] = Bones::颈;
		命脉[0][2] = Bones::胸;
		命脉[0][3] = Bones::腹;
		命脉[0][4] = Bones::屌;

		命脉[1][0] = Bones::命脉左肩 + 7;
		命脉[1][1] = Bones::命脉左肩 + 5;
		命脉[1][2] = Bones::命脉左肩 + 4;
		命脉[1][3] = Bones::命脉左肩 + 2;
		命脉[1][4] = Bones::命脉左肩 + 3;
		命脉[1][5] = Bones::命脉左肩 + 1;
		命脉[1][6] = Bones::命脉左肩;
		命脉[1][7] = Bones::命脉左肩 - 2;

		命脉[1][8] = Bones::命脉右肩 - 2;
		命脉[1][9] = Bones::命脉右肩;
		命脉[1][10] = Bones::命脉右肩 + 1;
		命脉[1][11] = Bones::命脉右肩 + 3;
		命脉[1][12] = Bones::命脉右肩 + 2;
		命脉[1][13] = Bones::命脉右肩 + 4;
		命脉[1][14] = Bones::命脉右肩 + 5;
		命脉[1][15] = Bones::命脉右肩 + 7;

		命脉[2][0] = Bones::命脉左膝 + 2;
		命脉[2][1] = Bones::命脉左膝 + 1;
		命脉[2][2] = Bones::命脉左膝;
		命脉[2][3] = Bones::屌;
		命脉[2][4] = Bones::命脉右膝;
		命脉[2][5] = Bones::命脉右膝 + 1;
		命脉[2][6] = Bones::命脉右膝 + 2;
	}
	//探路
	{
		探路[0][0] = Bones::头;
		探路[0][1] = Bones::颈;
		探路[0][2] = Bones::胸;
		探路[0][3] = Bones::腹;
		探路[0][4] = Bones::屌;

		探路[1][0] = Bones::探路左肩 + 6;
		探路[1][1] = Bones::探路左肩 + 5;
		探路[1][2] = Bones::探路左肩 + 4;
		探路[1][3] = Bones::探路左肩 + 3;
		探路[1][4] = Bones::探路左肩 + 2;
		探路[1][5] = Bones::探路左肩 + 1;
		探路[1][6] = Bones::探路左肩;
		探路[1][7] = Bones::探路左肩 - 1;

		探路[1][8] = Bones::探路右肩 - 1;
		探路[1][9] = Bones::探路右肩;
		探路[1][10] = Bones::探路右肩 + 1;
		探路[1][11] = Bones::探路右肩 + 2;
		探路[1][12] = Bones::探路右肩 + 3;
		探路[1][13] = Bones::探路右肩 + 4;
		探路[1][14] = Bones::探路右肩 + 5;
		探路[1][15] = Bones::探路右肩 + 6;

		探路[2][0] = Bones::探路左膝 + 2;
		探路[2][1] = Bones::探路左膝 + 1;
		探路[2][2] = Bones::探路左膝;
		探路[2][3] = Bones::屌;
		探路[2][4] = Bones::探路右膝;
		探路[2][5] = Bones::探路右膝 + 1;
		探路[2][6] = Bones::探路右膝 + 2;
	}
	//动力
	{
		动力[0][0] = Bones::头;
		动力[0][1] = Bones::颈;
		动力[0][2] = Bones::胸;
		动力[0][3] = Bones::腹;
		动力[0][4] = Bones::屌;

		动力[1][0] = Bones::动力左肩 + 7;
		动力[1][1] = Bones::动力左肩 + 5;
		动力[1][2] = Bones::动力左肩 + 4;
		动力[1][3] = Bones::动力左肩 + 2;
		动力[1][4] = Bones::动力左肩 + 3;
		动力[1][5] = Bones::动力左肩 + 1;
		动力[1][6] = Bones::动力左肩;
		动力[1][7] = Bones::动力左肩 - 2;

		动力[1][8] = Bones::动力右肩 - 2;
		动力[1][9] = Bones::动力右肩;
		动力[1][10] = Bones::动力右肩 + 1;
		动力[1][11] = Bones::动力右肩 + 3;
		动力[1][12] = Bones::动力右肩 + 2;
		动力[1][13] = Bones::动力右肩 + 4;
		动力[1][14] = Bones::动力右肩 + 5;
		动力[1][15] = Bones::动力右肩 + 7;

		动力[2][0] = Bones::动力左膝 - 3;
		动力[2][1] = Bones::动力左膝 - 2;
		动力[2][2] = Bones::动力左膝;
		动力[2][3] = Bones::屌;
		动力[2][4] = Bones::动力右膝;
		动力[2][5] = Bones::动力右膝 + 1;
		动力[2][6] = Bones::动力右膝 + 2;
	}
	//恶灵
	{
		恶灵[0][0] = Bones::头;
		恶灵[0][1] = Bones::颈;
		恶灵[0][2] = Bones::胸;
		恶灵[0][3] = Bones::腹;
		恶灵[0][4] = Bones::屌;

		恶灵[1][0] = Bones::恶灵左肩 + 7;
		恶灵[1][1] = Bones::恶灵左肩 + 5;
		恶灵[1][2] = Bones::恶灵左肩 + 4;
		恶灵[1][3] = Bones::恶灵左肩 + 2;
		恶灵[1][4] = Bones::恶灵左肩 + 3;
		恶灵[1][5] = Bones::恶灵左肩 + 1;
		恶灵[1][6] = Bones::恶灵左肩;
		恶灵[1][7] = Bones::恶灵左肩 - 2;

		恶灵[1][8] = Bones::恶灵右肩 - 2;
		恶灵[1][9] = Bones::恶灵右肩;
		恶灵[1][10] = Bones::恶灵右肩 + 1;
		恶灵[1][11] = Bones::恶灵右肩 + 3;
		恶灵[1][12] = Bones::恶灵右肩 + 2;
		恶灵[1][13] = Bones::恶灵右肩 + 4;
		恶灵[1][14] = Bones::恶灵右肩 + 5;
		恶灵[1][15] = Bones::恶灵右肩 + 7;

		恶灵[2][0] = Bones::恶灵左膝 + 2;
		恶灵[2][1] = Bones::恶灵左膝 + 1;
		恶灵[2][2] = Bones::恶灵左膝;
		恶灵[2][3] = Bones::屌;
		恶灵[2][4] = Bones::恶灵右膝;
		恶灵[2][5] = Bones::恶灵右膝 + 1;
		恶灵[2][6] = Bones::恶灵右膝 + 2;
	}
	//班加
	{
		班加[0][0] = Bones::头;
		班加[0][1] = Bones::颈;
		班加[0][2] = Bones::胸;
		班加[0][3] = Bones::腹;
		班加[0][4] = Bones::屌;

		班加[1][0] = Bones::班加左肩 + 7;
		班加[1][1] = Bones::班加左肩 + 5;
		班加[1][2] = Bones::班加左肩 + 4;
		班加[1][3] = Bones::班加左肩 + 2;
		班加[1][4] = Bones::班加左肩 + 3;
		班加[1][5] = Bones::班加左肩 + 1;
		班加[1][6] = Bones::班加左肩;
		班加[1][7] = Bones::班加左肩 - 2;

		班加[1][8] = Bones::班加右肩 - 2;
		班加[1][9] = Bones::班加右肩;
		班加[1][10] = Bones::班加右肩 + 1;
		班加[1][11] = Bones::班加右肩 + 3;
		班加[1][12] = Bones::班加右肩 + 2;
		班加[1][13] = Bones::班加右肩 + 4;
		班加[1][14] = Bones::班加右肩 + 5;
		班加[1][15] = Bones::班加右肩 + 7;

		班加[2][0] = Bones::班加左膝 + 2;
		班加[2][1] = Bones::班加左膝 + 1;
		班加[2][2] = Bones::班加左膝;
		班加[2][3] = Bones::屌;
		班加[2][4] = Bones::班加右膝;
		班加[2][5] = Bones::班加右膝 + 1;
		班加[2][6] = Bones::班加右膝 + 2;
	}
	//侵蚀
	{
		侵蚀[0][0] = Bones::头;
		侵蚀[0][1] = Bones::颈;
		侵蚀[0][2] = Bones::胸;
		侵蚀[0][3] = Bones::腹;
		侵蚀[0][4] = Bones::屌;

		侵蚀[1][0] = Bones::侵蚀左肩 + 7;
		侵蚀[1][1] = Bones::侵蚀左肩 + 5;
		侵蚀[1][2] = Bones::侵蚀左肩 + 4;
		侵蚀[1][3] = Bones::侵蚀左肩 + 2;
		侵蚀[1][4] = Bones::侵蚀左肩 + 3;
		侵蚀[1][5] = Bones::侵蚀左肩 + 1;
		侵蚀[1][6] = Bones::侵蚀左肩;
		侵蚀[1][7] = Bones::侵蚀左肩 - 2;

		侵蚀[1][8] = Bones::侵蚀右肩 - 2;
		侵蚀[1][9] = Bones::侵蚀右肩;
		侵蚀[1][10] = Bones::侵蚀右肩 + 1;
		侵蚀[1][11] = Bones::侵蚀右肩 + 3;
		侵蚀[1][12] = Bones::侵蚀右肩 + 2;
		侵蚀[1][13] = Bones::侵蚀右肩 + 4;
		侵蚀[1][14] = Bones::侵蚀右肩 + 5;
		侵蚀[1][15] = Bones::侵蚀右肩 + 7;

		侵蚀[2][0] = Bones::侵蚀左膝 + 2;
		侵蚀[2][1] = Bones::侵蚀左膝 + 1;
		侵蚀[2][2] = Bones::侵蚀左膝;
		侵蚀[2][3] = Bones::屌;
		侵蚀[2][4] = Bones::侵蚀右膝;
		侵蚀[2][5] = Bones::侵蚀右膝 + 1;
		侵蚀[2][6] = Bones::侵蚀右膝 + 2;
	}
	//幻象
	{
		幻象[0][0] = Bones::头;
		幻象[0][1] = Bones::颈;
		幻象[0][2] = Bones::胸;
		幻象[0][3] = Bones::腹;
		幻象[0][4] = Bones::屌;

		幻象[1][0] = Bones::幻象左肩 + 7;
		幻象[1][1] = Bones::幻象左肩 + 5;
		幻象[1][2] = Bones::幻象左肩 + 4;
		幻象[1][3] = Bones::幻象左肩 + 2;
		幻象[1][4] = Bones::幻象左肩 + 3;
		幻象[1][5] = Bones::幻象左肩 + 1;
		幻象[1][6] = Bones::幻象左肩;
		幻象[1][7] = Bones::幻象左肩 - 2;

		幻象[1][8] = Bones::幻象右肩 - 2;
		幻象[1][9] = Bones::幻象右肩;
		幻象[1][10] = Bones::幻象右肩 + 1;
		幻象[1][11] = Bones::幻象右肩 + 3;
		幻象[1][12] = Bones::幻象右肩 + 2;
		幻象[1][13] = Bones::幻象右肩 + 4;
		幻象[1][14] = Bones::幻象右肩 + 5;
		幻象[1][15] = Bones::幻象右肩 + 7;

		幻象[2][0] = Bones::幻象左膝 + 2;
		幻象[2][1] = Bones::幻象左膝 + 1;
		幻象[2][2] = Bones::幻象左膝;
		幻象[2][3] = Bones::屌;
		幻象[2][4] = Bones::幻象右膝;
		幻象[2][5] = Bones::幻象右膝 + 1;
		幻象[2][6] = Bones::幻象右膝 + 2;
	}
	//华森
	{
		华森[0][0] = Bones::头;
		华森[0][1] = Bones::颈;
		华森[0][2] = Bones::胸;
		华森[0][3] = Bones::腹;
		华森[0][4] = Bones::屌;

		华森[1][0] = Bones::华森左肩 + 7;
		华森[1][1] = Bones::华森左肩 + 5;
		华森[1][2] = Bones::华森左肩 + 4;
		华森[1][3] = Bones::华森左肩 + 2;
		华森[1][4] = Bones::华森左肩 + 3;
		华森[1][5] = Bones::华森左肩 + 1;
		华森[1][6] = Bones::华森左肩;
		华森[1][7] = Bones::华森左肩 - 2;

		华森[1][8] = Bones::华森右肩 - 2;
		华森[1][9] = Bones::华森右肩;
		华森[1][10] = Bones::华森右肩 + 1;
		华森[1][11] = Bones::华森右肩 + 3;
		华森[1][12] = Bones::华森右肩 + 2;
		华森[1][13] = Bones::华森右肩 + 4;
		华森[1][14] = Bones::华森右肩 + 5;
		华森[1][15] = Bones::华森右肩 + 7;

		华森[2][0] = Bones::华森左膝 + 2;
		华森[2][1] = Bones::华森左膝 + 1;
		华森[2][2] = Bones::华森左膝;
		华森[2][3] = Bones::屌;
		华森[2][4] = Bones::华森右膝;
		华森[2][5] = Bones::华森右膝 + 1;
		华森[2][6] = Bones::华森右膝 + 2;
	}
}
#include "DrawCallBack.h"
#include "DrawEng.h"
#include "proc_help.h"
#include "apexUtils.h"
#include "imgui/imgui.h"
#include "DriverHelp.h"
#include <cstdlib>
#include <sstream>
#include <string>
#include <iostream>

std::vector<void *> needFrees;

void drawMenu();
void drawEntity();
using namespace std;

__int64 lastPlayer = 0;
float losDistance = 0;
std::vector<ApexEntity> insidePlayer;
std::vector<ImVec2> itemLocals;

void draw() {
	drawMenu();
	drawEntity();
}

void drawMenu() {
	if (!appConfigs.MenuStatus) {
		return;
	}
	drawStrockText(ImGui::GetOverlayDrawList(), font, myFontSize, { 1, 1 }, { 0, 255, 255 }, u8"CatApex-CPPVersion");
	ImDrawList *drawList = ImGui::GetOverlayDrawList();
	int menuTop = (gameRect.bottom - 100) / 2;
	int menuIndex = 0;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop }, { 0, 255, 255 },
		u8"CatApex C++ 64位版本 [Home]键显隐");
	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.FangKuang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"方框透视[F1]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.FangKuang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.FangKuang ? u8"开" : u8"关");

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 30, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.FangKuang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"透视范围[F2]");
	drawStrockText(drawList, font, myFontSize, { 140, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.FangKuang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		to_string((int)appConfigs.TouShiFanWei).c_str());

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.WuPingTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"物品透视[F3]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.WuPingTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.WuPingTouShi ? u8"开" : u8"关");

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 30, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.WuPingTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"物品范围[F4]");
	drawStrockText(drawList, font, myFontSize, { 140, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.WuPingTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		to_string((int)appConfigs.WuPingFanWei).c_str());

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.PeiJianTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"隐藏喷子和配件[F5]");
	drawStrockText(drawList, font, myFontSize, { 150, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.PeiJianTouShi ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.PeiJianTouShi ? u8"开" : u8"关");

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.ZiDongMiaoZhun ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"右键自瞄[F6]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.ZiDongMiaoZhun ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.ZiDongMiaoZhun ? u8"开" : u8"关");

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.DanwuSanShe ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"弹无散射[F7]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.DanwuSanShe ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.DanwuSanShe ? u8"开" : u8"关");

}

void drawEntity() {
	insidePlayer.clear();
	itemLocals.clear();
	readWorldArray(&worldArray);
	ImDrawList *drawList = ImGui::GetOverlayDrawList();
	drawList->AddCircle({ (float)CentWindow.x, (float)CentWindow.y }, appConfigs.ZiMiaoFanWei, ImColor({ 14, 184, 58 }),
		50, 2.0f);
	for (ApexEntity entity : apexEntityList) {
		if (appConfigs.PeiJianTouShi &&
			(entity.flag >= 54 || entity.flag == 20 || entity.flag == 23 || entity.flag == 30)) {
			continue;
		}
		Vec3D entityLocal = {};
		readVec3D(entity.point + m_location, &entityLocal);
		float ViewW =
			worldArray[3][0] * entityLocal.x + worldArray[3][1] * entityLocal.y + worldArray[3][2] * entityLocal.z +
			worldArray[3][3];
		if (ViewW < 0.01)
		{
			continue;
		}
		float distance = ViewW / 30;
		ViewW = 1 / ViewW;
		if (entity.type == 0 && appConfigs.WuPingTouShi) {
			if (distance > appConfigs.WuPingFanWei) {
				continue;
			}
			float BoxX = CentWindow.x +
				(worldArray[0][0] * entityLocal.x + worldArray[0][1] * entityLocal.y +
					worldArray[0][2] * entityLocal.z +
					worldArray[0][3]) * ViewW * CentWindow.x;
			float BoxY = CentWindow.y -
				(worldArray[1][0] * entityLocal.x + worldArray[1][1] * entityLocal.y +
					worldArray[1][2] * entityLocal.z +
					worldArray[1][3]) * ViewW * CentWindow.y;
			if (BoxX > windowW || BoxX < -70 || BoxY > windowH || BoxY < 0) {
				continue;
			}
			const char *fNormal = "%s| %d";
			char *buff = (char *)calloc(128, 128);
			sprintf(buff, fNormal, entity.name, (int)distance);
		startFor:
			for (ImVec2 local : itemLocals) {
				if (local.x == BoxX && local.y == BoxY) {
					BoxY -= 15;
					goto startFor;
				}
			}
			itemLocals.emplace_back(BoxX, BoxY);
			drawStrockText(drawList, font, myFontSize, { BoxX, BoxY }, entity.color, buff);
			needFrees.emplace_back(buff);
		}
		else if (entity.type == 1) {
			float BoxX = CentWindow.x +
				(worldArray[0][0] * entityLocal.x + worldArray[0][1] * entityLocal.y +
					worldArray[0][2] * entityLocal.z +
					worldArray[0][3]) * ViewW * CentWindow.x;
			float BoxY1 = CentWindow.y -
				(worldArray[1][0] * entityLocal.x + worldArray[1][1] * entityLocal.y +
					worldArray[1][2] * (entityLocal.z + 70) +
					worldArray[1][3]) * ViewW * CentWindow.y;
			float BoxY = CentWindow.y -
				(worldArray[1][0] * entityLocal.x + worldArray[1][1] * entityLocal.y +
					worldArray[1][2] * (entityLocal.z - 10) +
					worldArray[1][3]) * ViewW * CentWindow.y;
			if (BoxX - (BoxY1 - BoxY) / 4 > windowW || BoxX - (BoxY1 - BoxY) / 4 < -70 || BoxY1 > windowH ||
				BoxY1 < 0) {
				continue;
			}
			ImColor playerColor;
			if (lastPlayer == entity.point) {
				playerColor = ImColor({ 244, 121, 131 });
			}
			else {
				playerColor = ImColor({ 14, 184, 58 });
			}
			int blood = 0;
			int armor = 0;
			int status = 0;
			readMem((HANDLE)gamePid, entity.point + m_iHealth, 4, &blood);
			readMem((HANDLE)gamePid, entity.point + m_shieldHealth, 4, &armor);
			readMem((HANDLE)gamePid, entity.point + m_bleedoutState, 4, &status);
			if (status != 0) {
				playerColor = ImColor({ 141, 75, 187 });
			}
			if (blood <= 0 || blood > 100 || distance > appConfigs.TouShiFanWei) {
				continue;
			}
			if (appConfigs.FangKuang) {


				const char *fNormal = u8"[%d]甲:%d 血:%d";
				const char *fName = u8"[%d]甲:%d 血:%d [%s]";


				char *buff = (char *)calloc(512, 512);

				if (entity.point == lastPlayer) {
					char *pName = readPlayerName(entity.zc);
					sprintf(buff, fName, (int)distance, armor, blood, pName);
					free(pName);
				}
				else {
					sprintf(buff, fNormal, (int)distance, armor, blood);
				}
				drawStrockText(drawList, font, myFontSize, { (BoxX - (BoxY1 - BoxY) / 4) + (BoxY1 - BoxY) / 2, BoxY },
					playerColor, buff);
				drawFrame(drawList, { BoxX - (BoxY1 - BoxY) / 4, BoxY, (BoxY1 - BoxY) / 2, BoxY1 - BoxY }, 2.1f,
					playerColor);
				needFrees.emplace_back(buff);
			}

			if (status != 0) {
				continue;
			}

			if (aimThreadStop && appConfigs.ZiDongMiaoZhun) {
				ImVec2 tmpPiont;
				tmpPiont.x = CentWindow.x - BoxX;
				tmpPiont.y = CentWindow.y - BoxY;
				float showDistance = sqrt(tmpPiont.x * tmpPiont.x + tmpPiont.y * tmpPiont.y);
				if (distance < 12) {
					insidePlayer.emplace_back(entity);
				}
				if (showDistance < appConfigs.ZiMiaoFanWei && distance < appConfigs.TouShiFanWei) {
					if (showDistance < losDistance || losDistance == 0) {
						losDistance = showDistance;
						aimEntity = entity.point;
					}
				}
			}
		}
	}

	readWorldArray(&worldArray);
	float losInside = 0;

	for (ApexEntity et1 : insidePlayer) {
		Vec3D entityLocal = {};
		readVec3D(et1.point + m_location, &entityLocal);

		float ViewW =
			worldArray[3][0] * entityLocal.x + worldArray[3][1] * entityLocal.y + worldArray[3][2] * entityLocal.z +
			worldArray[3][3];
		if (ViewW < 0.01f) {
			continue;
		}
		float distance = ViewW / 30;
		if (losInside == 0 || distance < losInside) {
			losInside = distance;
			aimEntity = et1.point;
			losDistance = 1;
		}
	}


	lastPlayer = aimEntity;

	if (losDistance == 0 && aimThreadStop) {
		aimEntity = 0;
	}

	if (appConfigs.ZiDongMiaoZhun) {
		if (GetAsyncKeyState(appConfigs.MiaoZhunReJian) != 0) {
			if (aimEntity > 0) {
				aim = true;
				if (aimThreadStop) {
					aimThreadStop = false;
					ResumeThread(hAimThread);
				}
			}
			else {
				aim = false;
				losDistance = 0;
			}
		}
		else {
			aim = false;
			losDistance = 0;
		}
	}
}
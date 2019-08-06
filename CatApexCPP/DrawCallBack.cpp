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
char * playerData = 0;
bool playerDataInit = false;

void draw() {
	if (!playerDataInit)
	{
		playerData = (char *) malloc(m_bleedoutState + 8);
		playerDataInit = true;
	}
	drawMenu();
	drawEntity();
}

void drawMenu() {
	if (!appConfigs.MenuStatus) {
		return;
	}
	ImDrawList *drawList = ImGui::GetBackgroundDrawList();
	const char * fps =  u8"每帧耗时: %.3f ms / 帧率: %.2f";
	char * drawStr = (char *) malloc(500);
	memset(drawStr, 0, 500);
	sprintf(drawStr, fps, 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	drawStrockText(drawList, font, 16, { 20, 20 }, ImColor(0, 255, 0), drawStr);
	needFrees.emplace_back(drawStr);
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
	__int64 tempAim = 0;
	insidePlayer.clear();
	itemLocals.clear();
	readWorldArray(&worldArray);
	ImDrawList *drawList = ImGui::GetBackgroundDrawList();
	drawList->AddCircle({ (float)CentWindow.x, (float)CentWindow.y }, appConfigs.ZiMiaoFanWei, ImColor({ 0x00, 0xff, 0xff }),
		50, 1.2f);
	int aimEntityStatus = 0;
	if (aimEntity > 0)
	{
		readMem((HANDLE)gamePid, aimEntityStatus + m_bleedoutState, 4, &aimEntityStatus);
	}
	//
	for (ApexEntity entity : apexEntityList) {
		if (appConfigs.PeiJianTouShi && (entity.flag >= 62 || entity.flag == 38 || entity.flag == 25 || entity.flag == 28)) continue;
		Vec3D entityLocal = {};
		readVec3D(entity.point + m_location, &entityLocal);
		float ViewW =
			worldArray[3][0] * entityLocal.x + worldArray[3][1] * entityLocal.y + worldArray[3][2] * entityLocal.z +
			worldArray[3][3];
		if (ViewW < 0.01) continue;
		ViewW = 1 / ViewW;
		if (entity.type == 0) {
			float BoxX = gamePoint.x + CentWindow.x +(
				worldArray[0][0] * entityLocal.x + 
				worldArray[0][1] * entityLocal.y +
				worldArray[0][2] * entityLocal.z +
				worldArray[0][3]
				) * ViewW * CentWindow.x;
			float BoxY = gamePoint.y + CentWindow.y -(
				worldArray[1][0] * entityLocal.x + 
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * entityLocal.z +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			if (BoxX > windowW || BoxX < -70 || BoxY > windowH || BoxY < 0) {
				continue;
			}
			const char *fNormal = "%s| %d";
			char *buff = (char *)malloc(128);
			memset(buff, 0, 128);
			sprintf(buff, fNormal, entity.name, entity.distance);
		startFor:
			for (ImVec2 local : itemLocals) {
				if (local.x == BoxX && local.y == BoxY) {
					BoxY -= 17;
					goto startFor;
				}
			}
			itemLocals.emplace_back(BoxX, BoxY);
			drawStrockText(drawList, font, myFontSize, { BoxX, BoxY }, entity.color, buff);
			needFrees.emplace_back(buff);
		}
		else if (entity.type == 1)
		{
			float BoxX = gamePoint.x + CentWindow.x +(
				worldArray[0][0] * entityLocal.x + 
				worldArray[0][1] * entityLocal.y +
				worldArray[0][2] * entityLocal.z +
				worldArray[0][3]
				) * ViewW * CentWindow.x;
			float BoxY1 = gamePoint.y + CentWindow.y -(
				worldArray[1][0] * entityLocal.x + 
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * (entityLocal.z + 70) +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			float BoxY = gamePoint.y + CentWindow.y -(
				worldArray[1][0] * entityLocal.x +
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * (entityLocal.z - 10) +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			if (BoxX - (BoxY1 - BoxY) / 4 > windowW || BoxX - (BoxY1 - BoxY) / 4 < -70 || BoxY1 > windowH || BoxY1 < 0) continue;
		
			ImColor playerColor;
			if (aimEntity == entity.point) {
				playerColor = ImColor({ 0xff, 0x50, 0x80 });
			}
			else {
				playerColor = ImColor({ 0x00, 0xff, 0xff });
			}
			readMem((HANDLE)gamePid, entity.point, m_bleedoutState + 8, playerData);
			int blood = *(int *) &playerData[m_iHealth];
			int armor = *(int *) &playerData[m_shieldHealth - 4];
			int status = *(int *) &playerData[m_bleedoutState];
			if (blood <= 0 || blood > 100 || (blood == 50 && armor == 0)) continue;
			if (status != 0) {
				playerColor = ImColor({ 0x90, 0x00, 0x255 });
			}
			if (appConfigs.FangKuang) {
				const char *fNormal = u8"[%d] 甲:%d 血:%d ";
				const char *fName = u8"[%d] 甲:%d 血:%d 【%s】";
				char *buff = (char *)malloc(512);
				memset(buff, 0, 512);
				if (entity.point == aimEntity) {
					char *pName = readPlayerName(entity.zc);
					sprintf(buff, fName, entity.distance, armor, blood, pName);
					free(pName);
				}
				else {
					sprintf(buff, fNormal, entity.distance, armor, blood);
				}
				drawStrockText(drawList, font, myFontSize, { (BoxX - (BoxY1 - BoxY) / 4) + (BoxY1 - BoxY) / 2, BoxY },
					playerColor, buff);
				drawFrame(drawList, { BoxX - (BoxY1 - BoxY) / 4, BoxY, (BoxY1 - BoxY) / 2, BoxY1 - BoxY }, 2.f,
					playerColor);
				ImVec2 tmpPiont;
				tmpPiont.x = CentWindow.x - BoxX;
				tmpPiont.y = CentWindow.y - BoxY;
				float showDistance = sqrt(tmpPiont.x * tmpPiont.x + tmpPiont.y * tmpPiont.y);
				if (entity.distance < 200)
				{
					DrawBone(drawList, entity.point, entityLocal, font, myFontSize, ImColor({ 0x00, 0xff, 0xff }));
				}
				needFrees.emplace_back(buff);
			}

			if (status != 0) continue;

			if ((aimThreadStop || aimEntityStatus != 0) && appConfigs.ZiDongMiaoZhun) {
				ImVec2 tmpPiont;
				tmpPiont.x = CentWindow.x - BoxX;
				tmpPiont.y = CentWindow.y - BoxY;
				float showDistance = sqrt(tmpPiont.x * tmpPiont.x + tmpPiont.y * tmpPiont.y);
				if (entity.distance < 20) {
					insidePlayer.emplace_back(entity);
				}
				if (showDistance < appConfigs.ZiMiaoFanWei && entity.distance < appConfigs.TouShiFanWei) {
					if (showDistance < losDistance || losDistance == 0) {
						losDistance = showDistance;
						tempAim = entity.point;
					}
				}
			}
		}
	}
	float losInside = 0;
	Vec3D myLocal = {};
	readVec3D(MouseAddr - 28, &myLocal);
	bool insideSubmit = false;
	for (ApexEntity et1 : insidePlayer) {
		Vec3D entityLocal = {};
		readVec3D(et1.point + m_location, &entityLocal);
		float ViewW =
			worldArray[3][0] * entityLocal.x + worldArray[3][1] * entityLocal.y + worldArray[3][2] * entityLocal.z +
			worldArray[3][3];
		if (ViewW < 0.01f) continue;
		ViewW = 1 / ViewW;
		float BoxX = gamePoint.x + CentWindow.x + (
			worldArray[0][0] * entityLocal.x +
			worldArray[0][1] * entityLocal.y +
			worldArray[0][2] * entityLocal.z +
			worldArray[0][3]
			) * ViewW * CentWindow.x;
		float BoxY1 = gamePoint.y + CentWindow.y - (
			worldArray[1][0] * entityLocal.x +
			worldArray[1][1] * entityLocal.y +
			worldArray[1][2] * (entityLocal.z + 70) +
			worldArray[1][3]
			) * ViewW * CentWindow.y;
		float BoxY = gamePoint.y + CentWindow.y - (
			worldArray[1][0] * entityLocal.x +
			worldArray[1][1] * entityLocal.y +
			worldArray[1][2] * (entityLocal.z - 10) +
			worldArray[1][3]
			) * ViewW * CentWindow.y;
		ImVec4 pRect = { BoxX - (BoxY1 - BoxY) / 4, BoxY, (BoxY1 - BoxY) / 2, BoxY1 - BoxY };
		pRect.z += pRect.x;
		pRect.w += pRect.y;
		//大数在前 小数在后
		// && (pRect.y > CentWindow.y && pRect.w < CentWindow.y)
		if ( (pRect.x > CentWindow.x && pRect.z < CentWindow.x) )
		{
			float xx = entityLocal.x - myLocal.x;
			float yy = entityLocal.y - myLocal.y;
			float zz = entityLocal.z - myLocal.z;
			float pDistance = sqrt(xx * xx + yy * yy + zz * zz);
			if (pDistance < losInside || losInside == 0)
			{
				insideSubmit = true;
				losInside = pDistance;
				tempAim = et1.point;
			}
		}
	}

	if (!insideSubmit && insidePlayer.size() > 0)
	{
		for (ApexEntity et1 : insidePlayer) {
			Vec3D entityLocal = {};
			readVec3D(et1.point + m_location, &entityLocal);
			float xx = entityLocal.x - myLocal.x;
			float yy = entityLocal.y - myLocal.y;
			float zz = entityLocal.z - myLocal.z;
			float pDistance = sqrt(xx * xx + yy * yy + zz * zz);
			if (pDistance < losInside || losInside == 0)
			{
				insideSubmit = true;
				losInside = pDistance;
				tempAim = et1.point;
			}
		}
	}

	lastPlayer = tempAim;

	if (losDistance == 0 && losInside == 0 && aimThreadStop) {
		tempAim = 0;
	}
	aimEntity = tempAim;
	if (appConfigs.ZiDongMiaoZhun) {
		if (GetAsyncKeyState(appConfigs.MiaoZhunReJian) != 0 || GetAsyncKeyState(VK_CONTROL)) {
			if (aimEntity > 0) {
				aim = true;
				if (aimThreadStop) {
					aimThreadStop = false;
					ResumeThread(hAimThread);
				}
			}
			else
			{
				aim = false;
				losDistance = 0;
			}
		}
		else
		{
			aim = false;
			losDistance = 0;
		}
	}
}
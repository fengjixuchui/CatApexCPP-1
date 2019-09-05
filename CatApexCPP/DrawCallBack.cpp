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

void drawMenu();
void drawEntity();
using namespace std;
float losDistance = 0;
std::vector<ApexEntity> insidePlayer;
std::vector<ImVec2> itemLocals;
char * entityData = 0;

char * pNameBuff = 0;
bool playerDataInit = false;
char * renderBuff = 0;
__int64 tempAim = 0;
char * aimBuff = 0;
INT64 lastTime = 0;

void draw() {
	if (!playerDataInit)
	{
		entityData = (char *)malloc(m_bleedoutState + 8);
		renderBuff = (char *)malloc(512);
		pNameBuff = (char *)malloc(512);
		aimBuff = (char *)malloc(512);
		playerDataInit = true;
		QueryPerformanceCounter((LARGE_INTEGER *)&lastTime);
	}
	drawMenu();
	drawEntity();
}

void drawMenu() {
	if (!appConfigs.MenuStatus) {
		return;
	}
	INT64 nowTime = 0;
	QueryPerformanceCounter((LARGE_INTEGER *)&nowTime);
	ImDrawList *drawList = ImGui::GetForegroundDrawList();
	const char * fps = u8"每帧耗时: %lld ms / 帧率: %.1f\0";
	INT64 usedTime = (nowTime - lastTime) / 10000;
	sprintf(renderBuff, fps, usedTime, usedTime == 0 ? 9999.f : (float) (1000.f / usedTime));
	lastTime = nowTime;
	drawStrockText(drawList, font, 16, { 20, 20 }, ImColor(0, 255, 0), renderBuff);
	int menuTop = (gameRect.bottom + 150) / 2;
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
		appConfigs.XianShiZhaZhu ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"显示炸珠[F7]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.XianShiZhaZhu ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.XianShiZhaZhu ? u8"开" : u8"关");

	++menuIndex;
	drawStrockText(drawList, font, myFontSize, { 10, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.KaiFaZheXuanXiang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 }, u8"开发者选项[F12]");
	drawStrockText(drawList, font, myFontSize, { 123, (float)menuTop + (menuIndex + 1) * 20 - 2 },
		appConfigs.KaiFaZheXuanXiang ? ImColor{ 0, 255, 255 } : ImColor{ 255, 255, 255 },
		appConfigs.KaiFaZheXuanXiang ? u8"开" : u8"关");

}

void drawEntity() {
	insidePlayer.clear();
	itemLocals.clear();
	readWorldArray(&worldArray);
	ImDrawList *drawList = ImGui::GetForegroundDrawList();
	drawList->AddCircle({ (float)CentWindow.x, (float)CentWindow.y }, appConfigs.ZiMiaoFanWei, ImColor({ 0x00, 0xff, 0xff }),
		50, 1.2f);
	int aimEntityStatus = 0;
	int _50Players = 0;
	if (aimEntity > 0)
	{
		readMem(gamePID, aimEntity + m_bleedoutState, 4, &aimEntityStatus);
	}
	AimEntityDrawData aimDraw = {0, 0};
	//
	for (ApexEntity entity : apexEntityList) {
		if (appConfigs.PeiJianTouShi && (entity.flag >= 62 || entity.flag == 38 || entity.flag == 25 || entity.flag == 28)) continue;
		readMem(gamePID, entity.point, m_bleedoutState + 8, entityData);
		Vec3D entityLocal = *(Vec3D *)&entityData[m_location];
		float ViewW =
			worldArray[3][0] * entityLocal.x + worldArray[3][1] * entityLocal.y + worldArray[3][2] * entityLocal.z +
			worldArray[3][3];
		if (entity.type == 1)
		{
			int status = *(int*)& entityData[m_bleedoutState];
			if (status == 0 && (int)entity.distance <= 50)
			{
				int blood = *(int*)& entityData[m_iHealth];
				int armor = *(int*)& entityData[m_shieldHealthMax - 4];
				if (blood <= 0 || blood > 100 || (blood == 50 && armor == 0)) continue;
				_50Players++;
			}
		}
		if (ViewW < 0.01) {
			continue;
		}
		ViewW = 1 / ViewW;
		if (entity.type == 0) {
			//
			float BoxX = gamePoint.x + CentWindow.x + (
				worldArray[0][0] * entityLocal.x +
				worldArray[0][1] * entityLocal.y +
				worldArray[0][2] * entityLocal.z +
				worldArray[0][3]
				) * ViewW * CentWindow.x;
			float BoxY = gamePoint.y + CentWindow.y - (
				worldArray[1][0] * entityLocal.x +
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * entityLocal.z +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			if (BoxX > windowW || BoxX < -70 || BoxY > windowH || BoxY < 0) {
				continue;
			}
			const char *fNormal = "%s| %d\0";
			sprintf(renderBuff, fNormal, entity.name, entity.distance);
		startFor:
			for (ImVec2 local : itemLocals) {
				if (local.x == BoxX && local.y == BoxY) {
					BoxY -= 17;
					goto startFor;
				}
			}
			itemLocals.emplace_back(BoxX, BoxY);
			drawStrockText(drawList, font, myFontSize, { BoxX, BoxY }, entity.color, renderBuff);
		}
		else if (entity.type == 1)
		{
			//
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
			if (BoxX - (BoxY1 - BoxY) / 4 > windowW || BoxX - (BoxY1 - BoxY) / 4 < -70 || BoxY1 > windowH || BoxY1 < 0) continue;

			ImColor playerColor;
			if (aimEntity == entity.point)
			{
				playerColor = ImColor({ 0xff, 0x50, 0x80 });
			}
			else
			{
				playerColor = ImColor({ 0x00, 0xff, 0xff });
			}
			int blood = *(int*)& entityData[m_iHealth];
			int armor = *(int*)& entityData[m_shieldHealthMax - 4];
			int status = *(int*)& entityData[m_bleedoutState];
			if (blood <= 0 || blood > 100 || (blood == 50 && armor == 0)) continue;
			if (status != 0) {
				playerColor = ImColor({ 0x90, 0x00, 0x255 });
			}
			
			if (appConfigs.FangKuang) {


				const char* fNormal = u8"[%d] 甲:%d 血:%d   %s\0";
				const char* fName = u8"[%d] 甲:%d 血:%d【%s】  %s\0";

				if (entity.point == aimEntity) {
					readPlayerName(entity.zc, pNameBuff);
					sprintf(aimBuff, fName, entity.distance, armor, blood, pNameBuff, entity.WeaponName);
					aimDraw = { BoxX, BoxY, BoxY1, playerColor, aimBuff };
				}
				else
				{
					sprintf(renderBuff, fNormal, entity.distance, armor, blood, entity.WeaponName);
					drawStrockText(drawList, font, myFontSize, { (BoxX - (BoxY1 - BoxY) / 4) + (BoxY1 - BoxY) / 2, BoxY },
						playerColor, renderBuff);
					drawFrame(drawList, { BoxX - (BoxY1 - BoxY) / 4, BoxY, (BoxY1 - BoxY) / 2, BoxY1 - BoxY }, 2.f,
						playerColor);
				}

				ImVec2 tmpPiont;
				tmpPiont.x = CentWindow.x - BoxX;
				tmpPiont.y = CentWindow.y - BoxY;
				float showDistance = sqrt(tmpPiont.x * tmpPiont.x + tmpPiont.y * tmpPiont.y);
				if (entity.distance < 120)
				{
					DrawBone(drawList, entity.point, entityLocal, font, myFontSize, ImColor({ 0x00, 0xff, 0xff }));
				}
			}

			if (status != 0) continue;

			if ((aimThreadStop || aimEntityStatus != 0) && appConfigs.ZiDongMiaoZhun) {
				ImVec2 tmpPiont;
				tmpPiont.x = CentWindow.x - BoxX;
				tmpPiont.y = CentWindow.y - BoxY;
				float showDistance = sqrt(tmpPiont.x * tmpPiont.x + tmpPiont.y * tmpPiont.y);
				if (entity.distance < 35 && GetAsyncKeyState(VK_CONTROL)) {
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

		else if (entity.type == 2)
		{
			//
			char * name = (char *)u8"npc";
			float BoxX = gamePoint.x + CentWindow.x + (
				worldArray[0][0] * entityLocal.x +
				worldArray[0][1] * entityLocal.y +
				worldArray[0][2] * entityLocal.z +
				worldArray[0][3]
				) * ViewW * CentWindow.x;
			float BoxY = gamePoint.y + CentWindow.y - (
				worldArray[1][0] * entityLocal.x +
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * entityLocal.z +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			if (BoxX > windowW || BoxX < -70 || BoxY > windowH || BoxY < 0) {
				continue;
			}
			const char *fNormal = "%s| %d\0";
			sprintf(renderBuff, fNormal, name, entity.distance);
			itemLocals.emplace_back(BoxX, BoxY);
			drawStrockText(drawList, font, myFontSize, { BoxX, BoxY }, entity.color, renderBuff);
		}
		else if (entity.type == 3)
		{
			//
			char * name = (char *)u8"大臭屁";
			float BoxX = gamePoint.x + CentWindow.x + (
				worldArray[0][0] * entityLocal.x +
				worldArray[0][1] * entityLocal.y +
				worldArray[0][2] * entityLocal.z +
				worldArray[0][3]
				) * ViewW * CentWindow.x;
			float BoxY = gamePoint.y + CentWindow.y - (
				worldArray[1][0] * entityLocal.x +
				worldArray[1][1] * entityLocal.y +
				worldArray[1][2] * entityLocal.z +
				worldArray[1][3]
				) * ViewW * CentWindow.y;
			if (BoxX > windowW || BoxX < -70 || BoxY > windowH || BoxY < 0) {
				continue;
			}
			const char *fNormal = "%s| %d\0";
			sprintf(renderBuff, fNormal, name, entity.distance);
			itemLocals.emplace_back(BoxX, BoxY);
			drawStrockText(drawList, font, myFontSize, { BoxX, BoxY }, entity.color, renderBuff);
		}
	}
	if (aimDraw.BoxY1)
	{
		drawStrockText(drawList, font, myFontSize, { (aimDraw.BoxX - (aimDraw.BoxY1 - aimDraw.BoxY) / 4) + (aimDraw.BoxY1 - aimDraw.BoxY) / 2, aimDraw.BoxY },
			aimDraw.color, aimDraw.text);
		drawFrame(drawList, { aimDraw.BoxX - (aimDraw.BoxY1 - aimDraw.BoxY) / 4, aimDraw.BoxY, (aimDraw.BoxY1 - aimDraw.BoxY) / 2, aimDraw.BoxY1 - aimDraw.BoxY }, 2.f,
			aimDraw.color);
	}

	sprintf(renderBuff, u8"50米内未倒地敌人: %d\0", _50Players);
	drawStrockText(drawList, font, myFontSize, { (float)CentWindow.x - 100, 170.f }, _50Players == 0 ? ImColor{ 0, 255, 0 } : ImColor{ 255, 0, 0 }, renderBuff);

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
		if ((pRect.x > CentWindow.x && pRect.z < CentWindow.x))
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

	if (losDistance == 0 && losInside == 0 && aimThreadStop) {
		tempAim = 0;
	}
	aimEntity = tempAim;
	if (appConfigs.ZiDongMiaoZhun) {
		if (GetAsyncKeyState(appConfigs.MiaoZhunReJian) || GetAsyncKeyState(VK_CONTROL)) {
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
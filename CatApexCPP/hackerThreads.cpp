#include <Windows.h>
#include "proc_help.h"
#include "hackerThreads.h"
#include "DriverHelp.h"
#include "apexUtils.h"
#include <vector>
#include <cmath>
#include "DrawEng.h"

using namespace std;

POINT gamePoint = {};
RECT gameRect = {};
__int64 TempPoint = 0;
__int64 ArrayPoint = 0;
__int64 MySelfPoint = 0;
__int64 EntityListPoint = 0;
float worldArray[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
};
int MyTeam;
__int64 MouseAddr = 0;
std::vector<ApexEntity> apexEntityList;
bool aim;
bool aimThreadStop;
__int64 aimEntity;
int windowW;
int windowH;
POINT CentWindow;


DWORD WINAPI InfoThread(LPVOID lpParam) {
	CoInitialize(0);
	int a = false;
	CentWindow = { 0, 0 };
	while (!a) {
		DWORD gameID = GetProcessIDByName("r5apex.exe");
		if (gameID == 0) exit(0);
		POINT tempGamePoint = {};
		RECT tempGameRect = {};
		ClientToScreen(hGameWind, &tempGamePoint);
		GetClientRect(hGameWind, &tempGameRect);
		if (tempGamePoint.x != gamePoint.x || tempGamePoint.y != gamePoint.y || tempGameRect.right != gameRect.right || tempGameRect.bottom != gameRect.bottom)
		{
			MoveWindow(myHWND, tempGamePoint.x, tempGamePoint.y, tempGameRect.right, tempGameRect.bottom, true);
			printf("Checked the game window moved. ReDraw\n");
		}
		gamePoint = tempGamePoint;
		gameRect = tempGameRect;
		windowW = gameRect.right;
		windowH = gameRect.bottom;
		CentWindow.x = windowW / 2;
		CentWindow.y = windowH / 2;

		readMem((HANDLE)gamePid, hGameModule + ViewRender, 8, &TempPoint);
		readMem((HANDLE)gamePid, TempPoint + m_renderArray, 8, &ArrayPoint);
		readMem((HANDLE)gamePid, hGameModule + CLocalEntity, 8, &MySelfPoint);
		readMem((HANDLE)gamePid, MySelfPoint + m_iTeamNum, 4, &MyTeam);
		MouseAddr = MySelfPoint + m_mouse;
		//printf("My: %lld \n", MySelfPoint);
		Sleep(2000);
	}
	return 0;
}

DWORD WINAPI EntityManager(LPVOID lpParam) {
	CoInitialize(0);
	initEntityNames();
	initBones();
	EntityListPoint = hGameModule + CEntityList;
	bool a = false;
	char *apexName = (char *)calloc(32, 32);
	__int64 cuPoint;
	int len = 65536 << 5;
	char * EntityListMem = (char *)malloc(len + 1);
	char * EntityMemCached = (char *)malloc(0x4230);
	while (!a) {
		Sleep(200);
		memset(EntityListMem, 0, len + 1);
		vector<ApexEntity> tempEntityList;
		readMem((HANDLE)gamePid, EntityListPoint, len, EntityListMem);
		Vec3D myLocal = {};
		readVec3D(MySelfPoint + m_location, &myLocal);
		for (int i = 0; i < 65535; ++i) {
			cuPoint = *(__int64 *)&EntityListMem[i << 5];
			if (cuPoint < 1000000) continue;
			__int64 apexNamePoint = 0;
			memset(EntityMemCached, 0, 0x2048);

			readMem((HANDLE)gamePid, cuPoint, 0x4230, EntityMemCached);
			apexNamePoint = *(__int64 *)&EntityMemCached[m_iSignifierName];

			if (apexNamePoint < 1000000) continue;
			readMem((HANDLE)gamePid, apexNamePoint, 32, apexName);
			if (!memcmp(apexName, "prop_survival", 13)) {
				if (!appConfigs.WuPingTouShi) continue;
				int flag = *(int *)&EntityMemCached[m_customScriptInt];
				ItemInfo item = entityNames[flag];
				if (item.color == 0) continue;
				Vec3D local = *(Vec3D *)&EntityMemCached[m_location];
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				if (distance > appConfigs.WuPingFanWei && (item.color != colors.ShiShi || distance > 350)) continue;
				if (item.color == colors.ShiShi)
				{
					item.color = ImColor(rand() % (255 - 1 + 1) + 1, rand() % (255 - 1 + 1) + 1, rand() % (255 - 1 + 1) + 1);
				}
				ApexEntity entity = { cuPoint, 0, flag, (char *)item.name, (char *)"", item.color, 0, distance, item };

				tempEntityList.emplace_back(entity);
				continue;
			}
			else if (!memcmp(apexName, "player", 6))
			{
				Vec3D local = *(Vec3D *)&EntityMemCached[m_EyePosition];
				if (local.x == 0 || local.y == 0 || local.z == 0) continue;
				int team = *(int *)&EntityMemCached[m_iTeamNum];
				if (team == MyTeam) continue;
				if (cuPoint == MySelfPoint) continue;
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				if (distance > appConfigs.TouShiFanWei) continue;
				ApexEntity entity = { cuPoint, 1, 0, NULL, apexName, 255, i, distance };
				tempEntityList.emplace_back(entity);
				continue;
			}
			else if (appConfigs.KaiFaZheXuanXiang)
			{
				int flag = *(int *)&EntityMemCached[m_customScriptInt];
				Vec3D local = *(Vec3D *)&EntityMemCached[m_location];
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				char * qwq = (char *)malloc(32);
				memcpy(qwq, apexName, 32);
				ApexEntity entity = { cuPoint, 0, flag, qwq, (char *)"", ImColor(255, 0, 0), 0, distance, NULL };
				GetEntityType(cuPoint);
				tempEntityList.emplace_back(entity);
				continue;
			}
			else if (!memcmp(apexName, "npc_frag", 8))
			{
				if (! appConfigs.XianShiZhaZhu)
				{
					continue;
				}
				int flag = *(int *)&EntityMemCached[m_customScriptInt];
				Vec3D local = *(Vec3D *)&EntityMemCached[m_location];
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				ApexEntity entity = { cuPoint, 2, flag, NULL, (char *)"", ImColor(255, 0, 0), 0, distance, NULL };
				tempEntityList.emplace_back(entity);
				//GetEntityType(cuPoint);
				continue;
			}
			
		}
		apexEntityList.clear();
		apexEntityList = tempEntityList;
	}
	return 0;
}

DWORD WINAPI SuperAim(LPVOID lpParam) {
	CoInitialize(0);
	bool a = false;
	aim = false;
	aimEntity = 0;
	int i = 0;
	char * weaponData = (char *) malloc(m_flBulletSpeed + 16);
	char * aimPlayerData = (char *)malloc(m_vecVelocity + 160);
	char * mySelfData = (char *)malloc(m_vecAimPunch + 160);
	while (!a) {
		int weaponEntityid = 0;
		__int64 weaponEntityPoint = 0; 
		float bulletSpeed = 0;
		float bullet_gv = 0;

		if (!aim) {
			aimEntity = 0;
			aimThreadStop = true;
			i = 0;
			SuspendThread(hAimThread);
		}
		readMem(gamePid, MySelfPoint, m_vecAimPunch + 160, mySelfData);
		weaponEntityid = *(int *)&mySelfData[m_latestPrimaryWeapons];
		weaponEntityid &= 0xFFFF;
		if (weaponEntityid > 0 && weaponEntityid < 65535) {
			readMem((HANDLE)gamePid, EntityListPoint + (weaponEntityid << 5), 8, &weaponEntityPoint);
		}
		readMem(gamePid, weaponEntityPoint, m_flBulletSpeed + 16, weaponData);
		bulletSpeed = *(float *)&weaponData[m_flBulletSpeed];
		if (bulletSpeed == 0) {
			bulletSpeed = 15000;
		}
		bullet_gv = *(float *)&weaponData[m_flBulletSpeed + 8];
		readMem(gamePid, aimEntity, m_vecVelocity + 160, aimPlayerData);
		Vec3D entityLocal = *(Vec3D *) &aimPlayerData[m_location];
		Vec3D aimLocal = GetBonePos(aimEntity, Bones::head, entityLocal);
		Vec3D myLocal = {};
		Vec3D VectorVec3D = *(Vec3D *) &aimPlayerData[m_vecVelocity];
		if (aimLocal.x == 0 || aimLocal.y == 0 || aimLocal.z == 0) continue;
		readVec3D(MouseAddr - 28, &myLocal);
		float xx = aimLocal.x - myLocal.x;
		float yy = aimLocal.y - myLocal.y;
		float zz = aimLocal.z - myLocal.z;
		float distance = sqrt(xx * xx + yy * yy + zz * zz);
		float flTime = distance / bulletSpeed;
		if (bulletSpeed > 10 && distance  * 0.01905f > 25) {
			float js = distance * 0.01905f / 100;
			if (js > 1.f) js = 1.f;
			aimLocal.x += ((VectorVec3D.x * flTime) * js);
			aimLocal.y += ((VectorVec3D.y * flTime) * js);
			aimLocal.z += ((VectorVec3D.z * flTime) * js) * 0.90f;
			aimLocal.z += 700.f * bullet_gv * (flTime * flTime);
		}
		int random = getRandomInt(-350, 500);
		aimLocal.z -= (float)(random / 100);
		aimLocal.x -= (float)(random / 320);
		aimLocal.y -= (float)(random / 320);
		xx = aimLocal.x - myLocal.x;
		yy = aimLocal.y - myLocal.y;
		zz = aimLocal.z - myLocal.z;
		float lf = atan2f(yy, xx) * rotation;
		float tb = 0 - ((atan2f(zz, sqrt(xx * xx + yy * yy))) * rotation);

		if (!(lf >= 0 || lf <= 0) || !(tb >= 0 || tb <= 0)) continue;
		Vec3D angle = { tb, lf, 0.f };
		Vec3D punch = *(Vec3D *) &mySelfData[m_vecAimPunch];
		angle.x -= punch.x;
		angle.y -= punch.y;
		angle.z -= punch.z;
		writeVec3D(MouseAddr, &angle);
		usleep(100);
	}
	return 0;
}

DWORD WINAPI HentaiThread(LPVOID lpParam) {
	bool a = false;
	while (true)
	{
		Sleep(1);
	}
	return 0;
}


DWORD WINAPI StartDraw(LPVOID lpParam) {
	gamePoint.x = 0;
	gamePoint.y = 0;
	gameRect.left = 0;
	gameRect.top = 0;
	gameRect.right = 0;
	gameRect.bottom = 0;
	ClientToScreen(hGameWind, &gamePoint);
	GetClientRect(hGameWind, &gameRect);
	startDraw();
	return 0;
}
#include <Windows.h>
#include "proc_help.h"
#include "hackerThreads.h"
#include "DriverHelp.h"
#include "apexUtils.h"
#include <vector>
#include <cmath>
#include "DrawEng.h"

using namespace std;

POINT gamePoint;
RECT gameRect;
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

int weaponEntityid = 0;
__int64 weaponEntityPoint = 0;
float bulletSpeed = 0;
bool bulledSpeedGeted = false;
float bullet_gv = 0;


DWORD WINAPI InfoThread(LPVOID lpParam) {
	CoInitialize(0);
	int a = false;
	CentWindow = { 0, 0 };
	while (!a) {
		DWORD gameID = GetProcessIDByName("r5apex.exe");
		if (gameID == 0) {
			exit(0);
		}
		gamePoint.x = 0;
		gamePoint.y = 0;
		gameRect.left = 0;
		gameRect.top = 0;
		gameRect.right = 0;
		gameRect.bottom = 0;
		ClientToScreen(hGameWind, &gamePoint);
		GetClientRect(hGameWind, &gameRect);
		MoveWindow(myHWND, gamePoint.x, gamePoint.y, gameRect.right, gameRect.bottom, true);

		windowW = gameRect.right;
		windowH = gameRect.bottom;
		CentWindow.x = windowW / 2;
		CentWindow.y = windowH / 2;

		readMem((HANDLE)gamePid, hGameModule + ViewRender, 8, &TempPoint);
		readMem((HANDLE)gamePid, TempPoint + m_renderArray, 8, &ArrayPoint);
		readMem((HANDLE)gamePid, hGameModule + CLocalEntity, 8, &MySelfPoint);
		readMem((HANDLE)gamePid, MySelfPoint + m_iTeamNum, 4, &MyTeam);
		MouseAddr = MySelfPoint + m_mouse;
		Sleep(2000);
	}
	return 0;
}

DWORD WINAPI EntityManager(LPVOID lpParam) {
	CoInitialize(0);
	initEntityNames();
	EntityListPoint = hGameModule + CEntityList;
	bool a = false;
	char *apexName = (char *)calloc(32, 32);
	__int64 cuPoint;
	int len = 65536 << 5;
	char * EntityListMem = (char *)malloc(len + 1);
	char * EntityMemCached = (char *)malloc(0x4230);
	while (!a) {
		Sleep(500);
		memset(EntityListMem, 0, len + 1);
		vector<ApexEntity> tempEntityList;
 		readMem((HANDLE)gamePid, EntityListPoint, len, EntityListMem);
		Vec3D myLocal = {};
		readVec3D(MouseAddr - 28, &myLocal);
		for (int i = 0; i < 65535; ++i) {
			memcpy(&cuPoint, &EntityListMem[i << 5], sizeof(cuPoint));
			if (cuPoint < 1000000) {
				continue;
			}
			__int64 apexNamePoint = 0;
			memset(EntityMemCached, 0, 0x2048);

			readMem((HANDLE)gamePid, cuPoint, 0x4230, EntityMemCached);
			memcpy(&apexNamePoint, &EntityMemCached[m_iSignifierName], sizeof(apexNamePoint));

			if (apexNamePoint < 1000000) {
				continue;
			}
			readMem((HANDLE)gamePid, apexNamePoint, 32, apexName);
			if (apexName[0] == 'p') {
				if (!memcmp(apexName, "prop_survival", 13)) {
					if (! appConfigs.WuPingTouShi)
					{
						continue;
					}
					int flag = 0;
					memcpy(&flag, &EntityMemCached[m_customScriptInt], sizeof(flag));
					ItemInfo item = entityNames[flag];
					if (item.color == 0) {
						continue;
					}
					Vec3D local = {};
					memcpy(&local, &EntityMemCached[m_location], sizeof(local));
					float xx = local.x - myLocal.x;
					float yy = local.y - myLocal.y;
					float zz = local.z - myLocal.z;
					float distance = sqrt(xx * xx + yy * yy + zz * zz);
					distance *= 0.01905f;
					if (distance > appConfigs.WuPingFanWei)
					{
						continue;
					}
					ApexEntity entity = { cuPoint, 0, flag, (char *)item.name, (char *)"", item.color, 0, distance, item };

					tempEntityList.emplace_back(entity);
					continue;
				}
				else if (!memcmp(apexName, "player", 6)) {
					Vec3D local = {};
					memcpy(&local, &EntityMemCached[m_EyePosition], sizeof(local));
					if (local.x == 0 || local.y == 0 || local.z == 0) {
						continue;
					}
					int team = 0;
					memcpy(&team, &EntityMemCached[m_iTeamNum], sizeof(team));
					if (team == MyTeam || cuPoint == MySelfPoint) {
						continue;
					}
					int blood = 0;
					memcpy(&blood, &EntityMemCached[m_iHealth], sizeof(blood));
					if (blood <= 0 || blood > 100) {
						continue;
					}
					float xx = local.x - myLocal.x;
					float yy = local.y - myLocal.y;
					float zz = local.z - myLocal.z;
					float distance = sqrt(xx * xx + yy * yy + zz * zz);
					distance *= 0.01905f;
					if (distance > appConfigs.TouShiFanWei)
					{
						continue;
					}
					ApexEntity entity = { cuPoint, 1, 0, NULL, apexName, 255, i, distance };
					tempEntityList.emplace_back(entity);
					continue;
				}
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
	while (!a) {
		if (!aim) {
			aimThreadStop = true;
			bulledSpeedGeted = false;
			i = 0;
			SuspendThread(hAimThread);
		}

		if (!bulledSpeedGeted)
		{
			readMem((HANDLE)gamePid, MySelfPoint + m_latestPrimaryWeapons, 4, &weaponEntityid);
			weaponEntityid &= 0xFFFF;
			if (weaponEntityid > 0 && weaponEntityid < 65535) {
				readMem((HANDLE)gamePid, EntityListPoint + weaponEntityid << 5, 8, &weaponEntityPoint);
			}

			readMem((HANDLE)gamePid, weaponEntityPoint + m_flBulletSpeed, 4, &bulletSpeed);
			if (bulletSpeed == 0) {
				bulletSpeed = 15000;
			}
			readMem((HANDLE)gamePid, weaponEntityPoint + m_flBulletSpeed + 8, 4, &bullet_gv);
			bulledSpeedGeted = true;
		}
		Vec3D aimLocal = {};
		Vec3D myLocal = {};
		Vec3D VectorVec3D = {};
		readVec3D(aimEntity + m_EyePosition, &aimLocal);
		if (aimLocal.x == 0 || aimLocal.y == 0 || aimLocal.z == 0)
		{
			continue;
		}
		readVec3D(MouseAddr - 28, &myLocal);
		readVec3D(aimEntity + m_vecVelocity, &VectorVec3D);
		float xx = aimLocal.x - myLocal.x;
		float yy = aimLocal.y - myLocal.y;
		float zz = aimLocal.z - myLocal.z;
		float distance = sqrt(xx * xx + yy*yy + zz*zz);
		float flTime = (distance - 15) / bulletSpeed ;
		if (bulletSpeed > 10 && distance > 15) {
			aimLocal.z += 1.f;
			aimLocal.x += (VectorVec3D.x * flTime) * 0.65f;
			aimLocal.y += (VectorVec3D.y * flTime) * 0.65f;
			aimLocal.z += (VectorVec3D.z * flTime) * 0.65f;
			float flyTime2 = flTime * ((distance - 15) / distance);
			float xx2 = 375 * bullet_gv * (flyTime2 * flyTime2);
			aimLocal.z += xx2;
		}
		xx = aimLocal.x - myLocal.x;
		yy = aimLocal.y - myLocal.y;
		zz = aimLocal.z - myLocal.z;
		float lf = atan2f(yy, xx) * 57.29f;
		float tb = 0 - ((atan2f(zz, sqrt(xx * xx + yy * yy))) * 57.29f);

		if (!(lf >= 0 || lf <= 0) || !(tb >= 0 || tb <= 0))
		{
			continue;
		}

		writeMem(gamePid, MouseAddr, 4, &tb);
		writeMem(gamePid, MouseAddr + 4, 4, &lf);
		if (++i % 20 == 0)
		{
			float AIMArray[4][4] = {
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0},
		{0, 0, 0, 0}
			};
			readWorldArray(&AIMArray);
			float ViewW = AIMArray[3][0] * aimLocal.x + AIMArray[3][1] * aimLocal.y + AIMArray[3][2] * aimLocal.z + AIMArray[3][3];
			if (ViewW < 0.01f)
			{
				continue;
			}
			ViewW = 1 / ViewW;
			float BoxX = CentWindow.x + (AIMArray[0][0] * aimLocal.x + AIMArray[0][1] * aimLocal.y + AIMArray[0][2] * aimLocal.z + AIMArray[0][3]) * ViewW * CentWindow.x;
			float BoxY = CentWindow.y - (AIMArray[1][0] * aimLocal.x + AIMArray[1][1] * aimLocal.y + AIMArray[1][2] * aimLocal.z + AIMArray[1][3]) * ViewW * CentWindow.y;
			mouse_event(1, BoxX - CentWindow.x, BoxY - CentWindow.y, 0, 0);
		}
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI HentaiThread(LPVOID lpParam) {
	bool a = false;
	while (!a)
	{
		Sleep(1);
		//if (appConfigs.DanwuSanShe)
		//{
		//	int weaponEntityid = 0;
		//	__int64 weaponEntityPoint = 0;
		//	readMem((HANDLE)gamePid, MySelfPoint + m_latestPrimaryWeapons, 4, &weaponEntityid);
		//	weaponEntityid &= 0xFFFF;
		//	if (weaponEntityid > 0 && weaponEntityid < 65535) {
		//		readMem((HANDLE)gamePid, EntityListPoint + weaponEntityid * 32, 8, &weaponEntityPoint);
		//	}
		//	if (weaponEntityPoint > 0)
		//	{
		//		float NoSp = 0.0f;
		//		writeMem(gamePid, weaponEntityPoint + m_flWeaponSpread1, 4, &NoSp);
		//		writeMem(gamePid, weaponEntityPoint + m_flWeaponSpread2, 4, &NoSp);
		//	}
		//}
	}
	return 0;
}


DWORD WINAPI StartDraw(LPVOID lpParam) {
	startDraw();
	return 0;
}
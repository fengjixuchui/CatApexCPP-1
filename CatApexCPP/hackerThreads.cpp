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
	char * EntityMemCached = (char *)malloc(0x2048);
	while (!a) {
		Sleep(500);
		memset(EntityListMem, 0, len + 1);
		vector<ApexEntity> tempEntityList;
 		readMem((HANDLE)gamePid, EntityListPoint, len, EntityListMem);
		for (int i = 0; i < 65535; ++i) {
			memcpy(&cuPoint, &EntityListMem[i << 5], sizeof(cuPoint));
			if (cuPoint < 1000000) {
				continue;
			}
			__int64 apexNamePoint = 0;
			memset(EntityMemCached, 0, 0x2048);

			readMem((HANDLE)gamePid, cuPoint, 0x2048, EntityMemCached);
			memcpy(&apexNamePoint, &EntityMemCached[m_iSignifierName], sizeof(apexNamePoint));

			if (apexNamePoint < 1000000) {
				continue;
			}
			readMem((HANDLE)gamePid, apexNamePoint, 32, apexName);
			if (apexName[0] == 'p') {
				if (!memcmp(apexName, "prop_survival", 13)) {
					int flag = 0;
					memcpy(&flag, &EntityMemCached[m_customScriptInt], sizeof(flag));
					ItemInfo item = entityNames[flag];
					if (item.color == 0) {
						continue;
					}
					ApexEntity entity = { cuPoint, 0, flag, (char *)item.name, (char *)"", item.color, 0, item };

					tempEntityList.emplace_back(entity);
					continue;
				}
				else if (!memcmp(apexName, "player", 6)) {
					Vec3D local = {};
					readVec3D(cuPoint + m_location, &local);
					if (local.x == 0 || local.y == 0 || local.z == 0) {
						continue;
					}
					int team = 0;
					memcpy(&team, &EntityMemCached[m_iTeamNum], sizeof(team));
					if (team == MyTeam) {
						continue;
					}
					if (cuPoint == MySelfPoint) {
						continue;
					}
					int blood = 0;
					memcpy(&blood, &EntityMemCached[m_iHealth], sizeof(blood));
					if (blood == 0) {
						continue;
					}
					ApexEntity entity = { cuPoint, 1, 0, NULL, apexName, 255, i };
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
	while (!a) {
		if (!aim) {
			aimThreadStop = true;
			SuspendThread(hAimThread);
		}
		int weaponEntityid = 0;
		__int64 weaponEntityPoint = 0;
		readMem((HANDLE)gamePid, MySelfPoint + m_latestPrimaryWeapons, 4, &weaponEntityid);
		weaponEntityid &= 0xFFFF;
		if (weaponEntityid > 0 && weaponEntityid < 65535) {
			readMem((HANDLE)gamePid, EntityListPoint + weaponEntityid * 32, 8, &weaponEntityPoint);
		}
		float bulletSpeed = 0;
		readMem((HANDLE)gamePid, weaponEntityPoint + m_flBulletSpeed, 4, &bulletSpeed);
		if (bulletSpeed == 0) {
			bulletSpeed = 15000;
		}
		Vec3D aimLocal = {};
		Vec3D myLocal = {};
		Vec3D VectorVec3D = {};
		VectorVec3D.x *= 0.65f;
		VectorVec3D.y *= 0.65f;
		VectorVec3D.z *= 0.40f;
		aimLocal.z += 4.00f;
		readVec3D(aimEntity + m_EyePosition, &aimLocal);
		readVec3D(MouseAddr - 28, &myLocal);
		readVec3D(aimEntity + m_vecVelocity, &VectorVec3D);

		if (aimLocal.x == 0 && aimLocal.y == 0 && aimLocal.z == 0) {
			continue;
		}
		if (myLocal.x == 0 && myLocal.y == 0 && myLocal.z == 0) {
			continue;
		}
		float xx = aimLocal.x - myLocal.x;
		float yy = aimLocal.y - myLocal.y;
		float zz = aimLocal.z - myLocal.z;
		float distance = sqrt(xx*xx + yy*yy + zz*zz);
		float flTime = (distance - 15) / bulletSpeed ;
		if (bulletSpeed > 10) {
			aimLocal.x += VectorVec3D.x * flTime;
			aimLocal.y += VectorVec3D.y * flTime;
			aimLocal.z += VectorVec3D.z * flTime;
			if (distance > 15)
			{
				float bullet_gv = 0;
				readMem((HANDLE)gamePid, weaponEntityPoint + m_flBulletSpeed + 8, 4, &bullet_gv);
				float flyTime2 = flTime * ((distance - 15) / distance);
				float xx2 = 375 * bullet_gv * (flyTime2 * flyTime2);
				aimLocal.z += xx2;
			}
		}
		xx = aimLocal.x - myLocal.x;
		yy = aimLocal.y - myLocal.y;
		zz = aimLocal.z - myLocal.z;
		float lf = atan2f(yy, xx) * 57.29f;
		float tb = 0 - ((atan2f(zz, sqrt(xx * xx + yy * yy))) * 57.29f);
		writeMem(gamePid, MouseAddr, 4, &tb);
		writeMem(gamePid, MouseAddr + 4, 4, &lf);
		if (GetAsyncKeyState(VK_LBUTTON) != 0)
		{
			mouse_event(1, 0, 1.8f, 0, 0);
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
		if (appConfigs.DanwuSanShe)
		{
			int weaponEntityid = 0;
			__int64 weaponEntityPoint = 0;
			readMem((HANDLE)gamePid, MySelfPoint + m_latestPrimaryWeapons, 4, &weaponEntityid);
			weaponEntityid &= 0xFFFF;
			if (weaponEntityid > 0 && weaponEntityid < 65535) {
				readMem((HANDLE)gamePid, EntityListPoint + weaponEntityid * 32, 8, &weaponEntityPoint);
			}
			if (weaponEntityPoint > 0)
			{
				float NoSp = 0.0f;
				writeMem(gamePid, weaponEntityPoint + m_flWeaponSpread1, 4, &NoSp);
				writeMem(gamePid, weaponEntityPoint + m_flWeaponSpread2, 4, &NoSp);
			}
		}
	}
	return 0;
}


DWORD WINAPI StartDraw(LPVOID lpParam) {
	startDraw();
	return 0;
}
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

void Print_Memory(const unsigned char* start, unsigned int length)
{
	//create row, col, and i.  Set i to 0

	int row, col, i = 0;

	//iterate through the rows, which will be 16 bytes of memory wide
	for (row = 0; (i + 1) < length; row++)
	{
		//print hex representation
		for (col = 0; col < 16; col++)
		{
			//calculate the current index

			i = row * 16 + col;

			//divides a row of 16 into two columns of 8
			if (col == 8)
			{
				printf(" ");
			}

			//print the hex value if the current index is in range.

			if (i < length)
			{
				printf("%02X", start[i]);
			}
			//print a blank if the current index is past the end
			else

			{
				printf("  ");
			}

			//print a space to keep the values separate
			printf(" ");
		}

		//create a vertial seperator between hex and ascii representations

		printf(" ");

		//print ascii representation
		for (col = 0; col < 16; col++)
		{
			//calculate the current index
			i = row * 16 + col;

			//divides a row of 16 into two coumns of 8

			if (col == 8)
			{
				printf("  ");
			}

			//print the value if it is in range
			if (i < length)
			{

				//print the ascii value if applicable
				if (start[i] > 0x20 && start[i] < 0x7F)  //A-Z
				{
					printf("%c", start[i]);
				}
				//print a period if the value is not printable

				else
				{
					printf(".");
				}
			}
			//nothing else to print, so break out of this for loop
			else

			{
				break;
			}
		}

		//create a new row
		printf("\n");
	}
}


DWORD WINAPI InfoThread(LPVOID lpParam) {
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

		readMem(gameHandle, hGameModule + ViewRender, 8, &TempPoint);
		readMem(gameHandle, TempPoint + m_renderArray, 8, &ArrayPoint);
		readMem(gameHandle, hGameModule + CLocalEntity, 8, &MySelfPoint);
		readMem(gameHandle, MySelfPoint + m_iTeamNum, 4, &MyTeam);
		MouseAddr = MySelfPoint + m_mouse;
		char* arrayData = (char*) malloc(64);
		ZeroMemory(arrayData, 64);
		free(arrayData);
		Sleep(1000);
	}
	return 0;
}

DWORD WINAPI EntityManager(LPVOID lpParam) {
	initEntityNames();
	initBones();
	EntityListPoint = hGameModule + CEntityList;
	bool a = false;
	char *apexName = (char *)calloc(32, 32);
	__int64 cuPoint;
	int len = 65536 << 5;
	char * EntityListMem = (char *)malloc(len + 1);
	char * EntityMemCached = (char *)malloc(0x10240);
	char * prop_scriptBuff = (char *)malloc(512);
	char * weaponData = (char *)malloc(m_flBulletSpeed + 16);
	char * weaponModelStr = (char *)malloc(50);
	char * entityInfoNameStr = (char *)malloc(80);
	while (!a) {
		Sleep(300);
		vector<ApexEntity> tempEntityList;
		readMem(gameHandle, EntityListPoint, len, EntityListMem);
		Vec3D myLocal = {};
		readVec3D(MySelfPoint + m_location, &myLocal);
		for (int i = 0; i < 65535; ++i) {
			cuPoint = *(__int64 *)&EntityListMem[i << 5];
			if (cuPoint < 1000000) continue;
			__int64 apexNamePoint = 0;
			readMem(gameHandle, cuPoint, 0x10240, EntityMemCached);
			apexNamePoint = *(__int64 *)&EntityMemCached[m_iSignifierName];
			if (apexNamePoint < 1000000) continue;
			readMem(gameHandle, apexNamePoint, 32, apexName);
			if (!memcmp(apexName, "prop_survival", 14)) {
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
			else if (!memcmp(apexName, "player", 7))
			{
				Vec3D local = *(Vec3D *)&EntityMemCached[m_location];
				if (local.x == 0 || local.y == 0 || local.z == 0 || (int)local.z == 23440) continue;
				int team = *(int *)&EntityMemCached[m_iTeamNum];
				if (team == MyTeam) continue;
				if (cuPoint == MySelfPoint) continue;
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				if (distance > appConfigs.TouShiFanWei) continue;
				int weaponEntityid = 0;
				__int64 weaponEntityPoint = 0;
				weaponEntityid = *(int *)&EntityMemCached[m_latestPrimaryWeapons + m_allWeapons];
				weaponEntityid &= 0xFFFF;
				if (weaponEntityid > 0 && weaponEntityid < 65535) {
					readMem(gameHandle, EntityListPoint + (weaponEntityid << 5), 8, &weaponEntityPoint);
				}
				GetEntityTypeStr(weaponEntityPoint, entityInfoNameStr);
				if (!memcmp("mdl/weapons/", entityInfoNameStr, 12) || !memcmp("mdl/Weapons/", entityInfoNameStr, 12))
				{
					char * newModelName = &entityInfoNameStr[12];
					for (size_t i = 0; i < 50; i++)
					{
						if (newModelName[i] == '/') {
							weaponModelStr[i] = '\0';
							break;
						}
						weaponModelStr[i] = newModelName[i];
					}
				}
				ApexEntity entity = { cuPoint, 1, 0, NULL, apexName, 255, i, distance, {}, GetWeaponName(weaponModelStr) };
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
				char * qwq = (char *)malloc(80);
				GetEntityTypeStr(cuPoint, qwq);
				ApexEntity entity = { cuPoint, 0, flag, qwq, (char *)"", ImColor(255, 0, 0), 0, distance, {} };
				//GetEntityType(cuPoint);
				tempEntityList.emplace_back(entity);
				continue;
			}
			else if (!memcmp(apexName, "npc_frag", 8))
			{
				if (!appConfigs.XianShiZhaZhu)
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
				ApexEntity entity = { cuPoint, 2, flag, NULL, (char *)"", ImColor(255, 0, 0), 0, distance, {} };
				tempEntityList.emplace_back(entity);
				continue;
			}
			else if (!memcmp(apexName, "prop_script", 12))
			{
				int flag = *(int *)&EntityMemCached[m_customScriptInt];
				Vec3D local = *(Vec3D *)&EntityMemCached[m_location];
				float xx = local.x - myLocal.x;
				float yy = local.y - myLocal.y;
				float zz = local.z - myLocal.z;
				float distance = sqrt(xx * xx + yy * yy + zz * zz);
				distance *= 0.01905f;
				if (distance > 200)
				{
					continue;
				}

				GetEntityTypeStr(cuPoint, prop_scriptBuff);
				if (memcmp(prop_scriptBuff, "mdl/props/caustic_gas_tank", 26))
				{
					continue;
				}
				ApexEntity entity = { cuPoint, 3, flag, NULL, (char *)"", ImColor(255, 255, 0), 0, distance, {} };
				tempEntityList.emplace_back(entity);
				GetEntityType(cuPoint);
				continue;
			}
		}
		apexEntityList.clear();
		apexEntityList = tempEntityList;
	}
	return 0;
}

DWORD WINAPI SuperAim(LPVOID lpParam) {
	aim = false;
	aimEntity = 0;
	char * weaponData = (char *)malloc(m_flBulletSpeed + 16);
	char * aimPlayerData = (char *)malloc(m_vecVelocity + 160);
	char * mySelfData = (char *)malloc(m_vecAimPunch + 160);
	while (true) {
		int weaponEntityid = 0;
		__int64 weaponEntityPoint = 0;
		float bulletSpeed = 0;
		float bullet_gv = 0;

		if (!aim) {
			aimEntity = 0;
			aimThreadStop = true;
			SuspendThread(hAimThread);
		}
		readMem(gameHandle, MySelfPoint, m_vecAimPunch + 160, mySelfData);
		weaponEntityid = *(int*)& mySelfData[m_latestPrimaryWeapons + m_allWeapons];
		weaponEntityid &= 0xFFFF;
		if (weaponEntityid > 0 && weaponEntityid < 65535) {
			readMem(gameHandle, EntityListPoint + (weaponEntityid << 5), 8, &weaponEntityPoint);
		}
		readMem(gameHandle, weaponEntityPoint, m_flBulletSpeed + 16, weaponData);
		bulletSpeed = *(float*)& weaponData[m_flBulletSpeed];
		if (bulletSpeed == 0) {
			bulletSpeed = 15000;
		}
		bullet_gv = *(float*)& weaponData[m_flBulletSpeed + 8];
		readMem(gameHandle, aimEntity, m_vecVelocity + 160, aimPlayerData);
		Vec3D entityLocal = *(Vec3D *)&aimPlayerData[m_location];
		float matrix[128][3][4];
		GetBoneArray(aimEntity, &matrix);
		Vec3D headLocal = CalcBonePos(matrix, Bones::head, entityLocal);
		Vec3D neckLocal = CalcBonePos(matrix, Bones::neck, entityLocal);
		Vec3D calcLocal = { (headLocal.x - neckLocal.x) / 3.2f, (headLocal.y - neckLocal.y) / 3.5f, (headLocal.z - neckLocal.z) / 3.5f };
		Vec3D aimLocal = { headLocal.x - calcLocal.x, headLocal.y - calcLocal.y, headLocal.z - calcLocal.z };
		Vec3D myLocal = {};
		Vec3D VectorVec3D = *(Vec3D *)&aimPlayerData[m_vecVelocity];
		if (aimLocal.x == 0 || aimLocal.y == 0 || aimLocal.z == 0) continue;
		readVec3D(MouseAddr - 28, &myLocal);
		float xx = aimLocal.x - myLocal.x;
		float yy = aimLocal.y - myLocal.y;
		float zz = aimLocal.z - myLocal.z;
		float distance = sqrt(xx * xx + yy * yy + zz * zz);
		float flTime = distance / bulletSpeed;
		if (bulletSpeed > 10 && distance  * 0.01905f > 25) {
			float js = distance * 0.01905f / 88;
			if (js > 1.f) js = 1.f;
			aimLocal.x += ((VectorVec3D.x * flTime) * js);
			aimLocal.y += ((VectorVec3D.y * flTime) * js);
			aimLocal.z += ((VectorVec3D.z * flTime) * js);
			aimLocal.z += 690.f * bullet_gv * (flTime * flTime);
		}
		xx = aimLocal.x - myLocal.x;
		yy = aimLocal.y - myLocal.y;
		zz = aimLocal.z - myLocal.z;
		float lf = atan2f(yy, xx) * rotation;
		float tb = 0 - ((atan2f(zz, sqrt(xx * xx + yy * yy))) * rotation);

		if (!(lf >= 0 || lf <= 0) || !(tb >= 0 || tb <= 0)) continue;
		Vec3D angle = { tb, lf, 0.f };
		Vec3D punch = *(Vec3D *)&mySelfData[m_vecAimPunch];
		angle.x -= punch.x;
		angle.y -= punch.y;
		writeVec3D(MouseAddr, &angle);
		usleep(5);
	}
	return 0;
}

DWORD WINAPI HentaiThread(LPVOID lpParam) {

	while (true)
	{
		int weaponEntityid = 0;
		__int64 weaponEntityPoint = 0;
		readMem(gameHandle, MySelfPoint + m_latestPrimaryWeapons, 4, &weaponEntityid);
		weaponEntityid &= 0xFFFF;
		if (weaponEntityid > 0 && weaponEntityid < 65535) {
			readMem(gameHandle, EntityListPoint + (weaponEntityid << 5), 8, &weaponEntityPoint);
		}
		float w = 0.f;
		writeMem(gameHandle, weaponEntityPoint + m_flWeaponSpread1, 4, &w);
		writeMem(gameHandle, weaponEntityPoint + m_flWeaponSpread2, 4, &w);
		//printf("WEAPON: %I64X\n", weaponEntityPoint);
		Sleep(1);
	}
	return 0;
}

DWORD WINAPI MouseThread(LPVOID lpParam) {

	while (true)
	{
		
		Sleep(100);
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
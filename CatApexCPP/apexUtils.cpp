#include "apexUtils.h"
#include "DriverHelp.h"
#include "proc_help.h"
#include "DrawEng.h"

char * entityInfoName = 0;
bool infoInit = false;

void readWorldArray(void *array) {
	readMem((HANDLE)gamePid, ArrayPoint, 64, array);
}

void readVec3D(__int64 addr, Vec3D * vec3D) {
	readMem((HANDLE)gamePid, addr, 12, vec3D);
}

void writeVec3D(__int64 addr, Vec3D * vec3D) {
	writeMem((HANDLE)gamePid, addr, 12, vec3D);
}

char *readPlayerName(int index) {
	__int64 nameArray = 0;
	readMem((HANDLE)gamePid, hGameModule + NameList, 8, &nameArray);
	nameArray = nameArray + index * 8;
	__int64 namePoint = 0;
	readMem((HANDLE)gamePid, nameArray + 2560, 8, &namePoint);
	char *name = (char *)calloc(256, 256);
	readMem((HANDLE)gamePid, namePoint, 256, name);
	return name;
}

Vec3D GetBonePos(__int64 entity, int ID, Vec3D entityLocal) {
	float matrix[128][3][4];
	__int64 pBoneMatrix = 0;
	readMem(gamePid, entity + m_bone, sizeof(pBoneMatrix), &pBoneMatrix);

	readMem(gamePid, pBoneMatrix, sizeof(matrix), &matrix);


	Vec3D bone_pos = { matrix[ID][0][3], matrix[ID][1][3], matrix[ID][2][3] };
	bone_pos.x += entityLocal.x;
	bone_pos.y += entityLocal.y;
	bone_pos.z += entityLocal.z;

	return bone_pos;
}

void Vec3DBoneToScreen(Vec3D local, ImVec2 * point) {
	float ViewW =
		worldArray[3][0] * local.x + worldArray[3][1] * local.y + worldArray[3][2] * local.z +
		worldArray[3][3];
	if (ViewW < 0.01) return;
	ViewW = 1 / ViewW;
	point->x = gamePoint.x + CentWindow.x +(
		worldArray[0][0] * local.x + 
		worldArray[0][1] * local.y +
		worldArray[0][2] * local.z +
		worldArray[0][3]
		) * ViewW * CentWindow.x;

	point->y = gamePoint.y + CentWindow.y - (
		worldArray[1][0] * local.x + 
		worldArray[1][1] * local.y +
		worldArray[1][2] * local.z +
		worldArray[1][3]
		) * ViewW * CentWindow.y;
}

void DrawBone(ImDrawList * drawList, __int64 entity, Vec3D entityLocal, ImFont *font, float size, ImColor col) {
	int entityType = GetEntityType(entity);
	if (entityType == 0)
	{
		drawBones(drawList, entity, entityLocal, col, 寻血);
		return;
	}
	else if (entityType == 1)
	{
		drawBones(drawList, entity, entityLocal, col, 直布);
	}
	else if (entityType == 2)
	{
		drawBones(drawList, entity, entityLocal, col, 命脉);
	}
	else if (entityType == 3)
	{
		drawBones(drawList, entity, entityLocal, col, 探路);
	}
	else if (entityType == 4)
	{
		drawBones(drawList, entity, entityLocal, col, 动力);
	}
	else if (entityType == 5)
	{
		drawBones(drawList, entity, entityLocal, col, 恶灵);
	}
	else if (entityType == 6)
	{
		drawBones(drawList, entity, entityLocal, col, 班加);
	}
	else if (entityType == 7)
	{
		drawBones(drawList, entity, entityLocal, col, 侵蚀);
	}
	else if (entityType == 8)
	{
		drawBones(drawList, entity, entityLocal, col, 幻象);
	}
	else if (entityType == 9)
	{
		drawBones(drawList, entity, entityLocal, col, 华森);
	}
}

int GetEntityType(__int64 entityPoint) {
	__int64 entityInfos = 0;
	readMem(gamePid, entityPoint + m_entityTypeInfo, sizeof(entityInfos), &entityInfos);
	if (!infoInit)
	{
		infoInit = true;
		entityInfoName = (char *)malloc(80);
		memset(entityInfoName, 0, 80);
	}
	readMem(gamePid, entityInfos + 4, 80, entityInfoName);
	string infoName = string(entityInfoName, 80);
	if (infoName.find("bloodhound", 0) != string::npos)
	{
		return 0; // 寻血
	}
	else if (infoName.find("gibraltar", 0) != string::npos)
	{
		return 1; // 直布
	}
	else if (infoName.find("support", 0) != string::npos)
	{
		return 2; // 命脉
	}
	else if (infoName.find("pathfinder", 0) != string::npos)
	{
		return 3; // 探路
	}
	else if (infoName.find("stim", 0) != string::npos)
	{
		return 4; // 动力
	}
	else if (infoName.find("wraith", 0) != string::npos)
	{
		return 5; // 恶灵
	}
	else if (infoName.find("bangalore", 0) != string::npos)
	{
		return 6; // 班加
	}
	else if (infoName.find("caustic", 0) != string::npos)
	{
		return 7; //侵蚀
	}
	else if (infoName.find("holo", 0) != string::npos)
	{
		return 8; // 幻象
	}
	else if (infoName.find("wattson", 0) != string::npos)
	{
		return 9; // 华森
	}
	else 
	{
		printf("%s\n", entityInfoName);
		return -1;
	}
}

void drawBones(ImDrawList * drawList, __int64 entity, Vec3D local, ImColor col, int boneIndex[3][16]) {
	ImVec2 lasteIndex = {};
	ImVec2 cuIndex = {};
	for (size_t i = 1; i < 5; i++)
	{
		if (i == 1) Vec3DBoneToScreen(GetBonePos(entity, boneIndex[0][0], local), &lasteIndex);
		Vec3DBoneToScreen(GetBonePos(entity, boneIndex[0][i], local), &cuIndex);
		drawList->AddLine(lasteIndex, cuIndex, col);
		lasteIndex = cuIndex;
	}

	for (size_t i = 1; i < 16; i++)
	{
		if (i == 1) Vec3DBoneToScreen(GetBonePos(entity, boneIndex[1][0], local), &lasteIndex);
		Vec3DBoneToScreen(GetBonePos(entity, boneIndex[1][i], local), &cuIndex);
		drawList->AddLine(lasteIndex, cuIndex, col);
		lasteIndex = cuIndex;
	}

	for (size_t i = 1; i < 7; i++)
	{
		if (i == 1) Vec3DBoneToScreen(GetBonePos(entity, boneIndex[2][0], local), &lasteIndex);
		Vec3DBoneToScreen(GetBonePos(entity, boneIndex[2][i], local), &cuIndex);
		drawList->AddLine(lasteIndex, cuIndex, col);
		lasteIndex = cuIndex;
	}
}
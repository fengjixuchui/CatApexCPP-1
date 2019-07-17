#include "apexUtils.h"
#include "DriverHelp.h"
#include "proc_help.h"

void readWorldArray(void *array) {
	readMem((HANDLE)gamePid, ArrayPoint, 64, array);
}

void readVec3D(__int64 addr, Vec3D * vec3D) {
	readMem((HANDLE)gamePid, addr, 12, vec3D);
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
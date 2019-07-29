#include <string>
#include "proc_help.h"

#ifndef CATAPEXCPP_APEXUTILS_H
#define CATAPEXCPP_APEXUTILS_H

using std::string;
using std::wstring;

#endif //CATAPEXCPP_APEXUTILS_H

void readWorldArray(void *array);

void readVec3D(__int64 addr, Vec3D * vec3D);
void writeVec3D(__int64 addr, Vec3D * vec3D);

char *readPlayerName(int index);

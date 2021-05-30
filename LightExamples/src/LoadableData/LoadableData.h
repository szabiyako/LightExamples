#pragma once

#include "ObjData/ObjData.h"

struct LoadableData
{
	ObjData objData;

	LoadableData();
	LoadableData(const ObjData &objData);
};


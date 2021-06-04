#pragma once

#include "ObjData/ObjData.h"
#include "Image/Image.h"

struct LoadableData
{
	struct Default {
		Image diffuse;
		Image specular;
		Image normal;

		Image reflection;
	};
	struct PBR {
		Image albedo;
		Image metallic;
		Image roughness;
		Image normal;
		Image ao;

		Image reflection;
	};

	ObjData objData;
	Default defaultData;
	PBR pbrData;


	LoadableData();
	LoadableData(const ObjData &objData);
};


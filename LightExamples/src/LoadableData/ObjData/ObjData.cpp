#include "ObjData.h"

bool operator==(const ObjData& objLeft, const ObjData& objRight)
{
	return objLeft.data.vertices == objRight.data.vertices
		   && objLeft.data.normals == objRight.data.normals
		   && objLeft.data.uv == objRight.data.uv
		   && objLeft.indices.polygonsStarts == objRight.indices.polygonsStarts
		   && objLeft.indices.polygons == objRight.indices.polygons
		   && objLeft.indices.normals == objRight.indices.normals
		   && objLeft.indices.uv == objRight.indices.uv;
}

void ObjData::clear()
{
	data.vertices.clear();
	data.normals.clear();
	data.uv.clear();

	indices.polygonsStarts.clear();
	indices.polygons.clear();
	indices.normals.clear();
	indices.uv.clear();
}

size_t ObjData::getPolygonsCount() const
{
	if (indices.polygonsStarts.empty())
		return 0;
	return indices.polygonsStarts.size() - 1;
}

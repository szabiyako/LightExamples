#pragma once

#include <vector>

#include "glm/glm.hpp"

class ObjData
{
public:
	struct Data
	{
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uv;
	};
	struct Indices
	{
		std::vector<int> polygonsStarts;
		std::vector<int> polygons;
		std::vector<int> normals;
		std::vector<int> uv;
	};
	Data data;
	Indices indices;

	void clear();
	size_t getPolygonsCount();
};

bool operator== (const ObjData& objLeft, const ObjData& objRight);

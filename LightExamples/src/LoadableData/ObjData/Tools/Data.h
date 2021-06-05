#pragma once

#include <vector>
#include "glm/glm.hpp"

class ObjData;

namespace ObjDataTools {
namespace Data {

std::vector<glm::vec3> computeNormals(
    const std::vector<int>& triangleVertexIndices,
    const std::vector<glm::vec3>& vertices);
std::vector<int> buildTriangleVertexIndices(const std::vector<int>& polygonIndices, const std::vector<int>& polygonStart);
std::vector<float> packTriangleVertexCoords(const std::vector<glm::vec3>& vertices, const std::vector<int>& triangleVertexIndices);
std::vector<float> packUVs(const std::vector<glm::vec2>& uvs, const std::vector<int>& triangleUVsIndices);

std::vector<float> buildRawData(const ObjData& objData);
}
}


#include "Default.h"

using namespace DrawableData;

#include <vector>

#include "glm/glm.hpp"

#include "Utils/Console.h"
#include "LoadableData/ObjData/Tools/Data.h"

Default::Default(const ObjData& objData)
{
    loadFromObjData(objData);
}

void Default::loadFromObjData(const ObjData& objData)
{
    const std::vector<int> triangleVertexIndices = ObjDataTools::Data::buildTriangleVertexIndices(objData.indices.polygons, objData.indices.polygonsStarts);
    const std::vector<float> triangleVertexCoords = ObjDataTools::Data::packTriangleVertexCoords(objData.data.vertices, triangleVertexIndices);
    const size_t nVertices = triangleVertexCoords.size() / 3;

    const bool hasNormals = (objData.indices.normals.size() == objData.indices.polygons.size());

    std::vector<float> triangleNormalCoords;
    if (hasNormals) {
        const std::vector<glm::vec3> normals = objData.data.normals;
        const std::vector<int> normalsIndices = objData.indices.normals;
        const std::vector<int> triangleNormalIndices = ObjDataTools::Data::buildTriangleVertexIndices(normalsIndices, objData.indices.polygonsStarts);
        triangleNormalCoords = ObjDataTools::Data::packTriangleVertexCoords(normals, triangleNormalIndices);
    }
    else {
        const std::vector<glm::vec3> normals = ObjDataTools::Data::computeNormals(triangleVertexIndices, objData.data.vertices);
        triangleNormalCoords = ObjDataTools::Data::packTriangleVertexCoords(normals, triangleVertexIndices);
    }

    m_hasUVs = !objData.indices.uv.empty();
    Console::print("Model " + (m_hasUVs ? std::string("has UVs") : std::string("doesn't have UVs")) + "\n");

    std::vector<float> triangleUVs;
    if (m_hasUVs) {
        const std::vector<glm::vec2> uvs = objData.data.uv;
        const std::vector<int> uvsIndices = objData.indices.uv;
        const std::vector<int> triangleUVsIndices = ObjDataTools::Data::buildTriangleVertexIndices(uvsIndices, objData.indices.polygonsStarts);
        triangleUVs = ObjDataTools::Data::packUVs(uvs, triangleUVsIndices);
    }

    std::vector<float> data(nVertices * 8);
    std::vector<unsigned int> indices(nVertices);
    for (size_t i = 0; i < nVertices; ++i) {
        data[(i * 8) + 0] = triangleVertexCoords[(i * 3) + 0];
        data[(i * 8) + 1] = triangleVertexCoords[(i * 3) + 1];
        data[(i * 8) + 2] = triangleVertexCoords[(i * 3) + 2];

        if (m_hasUVs) {
            data[(i * 8) + 3] = triangleUVs[(i * 2) + 0];
            data[(i * 8) + 4] = triangleUVs[(i * 2) + 1];
        }
        else {
            data[(i * 8) + 3] = 0.f;
            data[(i * 8) + 4] = 0.f;
        }

        data[(i * 8) + 5] = triangleNormalCoords[(i * 3) + 0];
        data[(i * 8) + 6] = triangleNormalCoords[(i * 3) + 1];
        data[(i * 8) + 7] = triangleNormalCoords[(i * 3) + 2];

        indices[i] = unsigned int(i);
    }
    m_vertexBuffer.create(data.data(), nVertices * 8 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3); // vertices
    layout.Push<float>(2); // uvs
    layout.Push<float>(3); // normals
    m_vertexArray.addBuffer(m_vertexBuffer, layout);
    m_indexBuffer.create(indices.data(), nVertices);
}

Default::~Default()
{
}

bool Default::hasUVs() const
{
    return m_hasUVs;
}


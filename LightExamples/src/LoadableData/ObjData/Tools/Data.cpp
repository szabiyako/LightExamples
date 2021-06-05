#include "Data.h"

#include "LoadableData/ObjData/ObjData.h"

std::vector<glm::vec3> ObjDataTools::Data::computeNormals(
    const std::vector<int>& triangleVertexIndices,
    const std::vector<glm::vec3>& vertices)
{
    const size_t nVerticesInTriangle = 3;
    const size_t nTriangleVertexIndices = triangleVertexIndices.size() / 3;
    const float edgeKoeff = 1000.f;
    std::vector<glm::vec3> res(vertices.size());
    for (size_t triangleInd = 0; triangleInd < nTriangleVertexIndices; ++triangleInd)
    {
        for (size_t vertexInd = 0; vertexInd < nVerticesInTriangle; ++vertexInd)
        {
            const size_t prevVertexInd = triangleVertexIndices[3 * triangleInd + vertexInd % nVerticesInTriangle];
            const size_t originVertexInd = triangleVertexIndices[3 * triangleInd + (vertexInd + 1) % nVerticesInTriangle];
            const size_t nextVertexInd = triangleVertexIndices[3 * triangleInd + (vertexInd + 2) % nVerticesInTriangle];

            const glm::vec3 edge1 = vertices[prevVertexInd] - vertices[originVertexInd];
            const glm::vec3 edge2 = vertices[nextVertexInd] - vertices[originVertexInd];

            const glm::vec3 normal = glm::normalize(glm::cross(edge2 * edgeKoeff, edge1 * edgeKoeff));

            res[triangleVertexIndices[3 * triangleInd + (vertexInd + 1) % nVerticesInTriangle]] += normal;
        }
    }
    for (int vertexInd = 0; vertexInd < res.size(); ++vertexInd)
        res[vertexInd] = glm::normalize(res[vertexInd]);
    return res;

}

std::vector<int> ObjDataTools::Data::buildTriangleVertexIndices(const std::vector<int>& polygonIndices, const std::vector<int>& polygonStart)
{
    //ASSERT(polygonStart.size() > 0);
    if (polygonStart.size() == 0)
        return std::vector<int>();

    std::vector<int> triangleIndices;
    int polygonCount = polygonStart.size() - 1;
    for (int polygonInd = 0; polygonInd < polygonCount; ++polygonInd) {
        int polygonStartInd = polygonStart[polygonInd];
        int originInd = polygonIndices[polygonStartInd];
        int vertexCount = polygonStart[polygonInd + 1] - polygonStart[polygonInd];
        for (int triangleInd = 0; triangleInd < vertexCount - 2; ++triangleInd) {
            int firstInd = polygonIndices[polygonStartInd + triangleInd + 1];
            int secondInd = polygonIndices[polygonStartInd + triangleInd + 2];
            triangleIndices.push_back(originInd);
            triangleIndices.push_back(firstInd);
            triangleIndices.push_back(secondInd);
        }
    }
    return triangleIndices;
}

std::vector<float> ObjDataTools::Data::packTriangleVertexCoords(const std::vector<glm::vec3>& vertices, const std::vector<int>& triangleVertexIndices)
{
    //ASSERT(triangleVertexIndices.size() % 3 == 0);
    const size_t nVertices = triangleVertexIndices.size();
    std::vector<float> triangleVertexCoords;
    triangleVertexCoords.reserve(nVertices * 3);
    for (int vertexInd : triangleVertexIndices) {
        glm::vec3 triangleVertex = vertices[vertexInd];
        for (int coordInd = 0; coordInd < 3; ++coordInd)
            triangleVertexCoords.push_back(triangleVertex[coordInd]);
    }
    return triangleVertexCoords;
}

std::vector<float> ObjDataTools::Data::packUVs(const std::vector<glm::vec2>& uvs, const std::vector<int>& triangleUVsIndices)
{
    //ASSERT(triangleVertexIndices.size() % 2 == 0);
    const size_t nVertices = triangleUVsIndices.size();
    std::vector<float> triangleUvs;
    triangleUvs.reserve(nVertices * 2);
    for (int vertexInd : triangleUVsIndices) {
        glm::vec2 triangleVertex = uvs[vertexInd];
        for (int coordInd = 0; coordInd < 2; ++coordInd)
            triangleUvs.push_back(triangleVertex[coordInd]);
    }
    return triangleUvs;
}

std::vector<float> ObjDataTools::Data::buildRawData(const ObjData& objData)
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

    const bool hasUVs = !objData.indices.uv.empty();

    std::vector<float> triangleUVs;
    if (hasUVs) {
        const std::vector<glm::vec2> uvs = objData.data.uv;
        const std::vector<int> uvsIndices = objData.indices.uv;
        const std::vector<int> triangleUVsIndices = ObjDataTools::Data::buildTriangleVertexIndices(uvsIndices, objData.indices.polygonsStarts);
        triangleUVs = ObjDataTools::Data::packUVs(uvs, triangleUVsIndices);
    }

    std::vector<float> data(nVertices * 8);
    for (size_t i = 0; i < nVertices; ++i) {
        data[(i * 8) + 0] = triangleVertexCoords[(i * 3) + 0];
        data[(i * 8) + 1] = triangleVertexCoords[(i * 3) + 1];
        data[(i * 8) + 2] = triangleVertexCoords[(i * 3) + 2];

        if (hasUVs) {
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
    }
    return data;
}

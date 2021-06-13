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
    std::vector<float> data = ObjDataTools::Data::buildRawData(objData);
    m_hasUVs = !objData.indices.uv.empty();
    const size_t nVertices = data.size() / 8;
    std::vector<unsigned int> indices(nVertices);
    for (size_t i = 0; i < nVertices; ++i)
        indices[i] = unsigned int(i);

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


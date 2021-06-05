#include "Utils.h"

#include "LoadableData/ObjData/Tools/Data.h"

void Utils::BVHNodesToTexture(Texture::VertexTexture& bvhTexture, BVH::BVHBuilder& bvh)
{
	float const* texNodeData = (float*)(bvh.bvhToTexture());
	int texWidthNode = bvh.getNodesSize();
	bvhTexture.loadFromData(texNodeData, texWidthNode, texWidthNode);
}

void Utils::loadModelsToTexture(Texture::VertexTexture& vertexTexture, BVH::BVHBuilder& bvh, std::vector<Model>& models)
{
	const size_t nModels = models.size();
	std::vector<float> vertexData;
	for (size_t i = 0; i < nModels; ++i) {
		const ObjData& objData = models[i].getLoadableDataRef().objData;
		const std::vector<int> triangleVertexIndices = ObjDataTools::Data::buildTriangleVertexIndices(objData.indices.polygons, objData.indices.polygonsStarts);
		const std::vector<float> triangleVertexCoords = ObjDataTools::Data::packTriangleVertexCoords(objData.data.vertices, triangleVertexIndices);
		vertexData.insert(vertexData.end(), triangleVertexCoords.begin(), triangleVertexCoords.end());
	}
	bvh.build(vertexData);

	uint32_t vertexCount = vertexData.size() / 3; // 3 vertex component x,y,z
	int sqrtVertexCount = ceil(sqrt(vertexCount)); // for sqrt demension 
	int texWidthPos = Utils::powerOfTwo(sqrtVertexCount); // texture demension sqrt
	vertexData.resize(texWidthPos * texWidthPos * 3, 0.0); // for pack x,y,z to  r,g,b

	vertexTexture.loadFromData(vertexData.data(), texWidthPos, texWidthPos);
}
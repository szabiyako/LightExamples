#include "Utils.h"

#include "LoadableData/ObjData/Tools/Data.h"

void Utils::BVHNodesToTexture(Texture::VertexTexture& bvhTexture, BVH::BVHBuilder& bvh)
{
	float const* texNodeData = (float*)(bvh.bvhToTexture());
	int texWidthNode = bvh.getNodesSize();
	bvhTexture.loadFromData(texNodeData, texWidthNode, texWidthNode);
}

void Utils::loadModelsToTexture(
	Texture::VertexTexture& vertexTexture,
	Texture::VertexTexture& normalTexture,
	BVH::BVHBuilder& bvh,
	std::vector<Model>& models)
{
	const size_t nModels = models.size();
	std::vector<float> vertexData;
	std::vector<float> normalData;
	for (size_t i = 0; i < nModels; ++i) {
		const ObjData& baseObjData = models[i].getLoadableDataRef().objData;
		ObjData objData = baseObjData;
		const size_t nVertices = baseObjData.data.vertices.size();
		const glm::mat4x4 transform = models[i].getModelMatrix();
		if (transform != glm::mat4(1.f)) {
			for (size_t vIndex = 0; vIndex < nVertices; ++vIndex) {
				objData.data.vertices[vIndex] = transform * glm::vec4(objData.data.vertices[vIndex], 1);
			}
		}
		//const ObjData& objData = models[i].getLoadableDataRef().objData;
		const std::vector<int> triangleVertexIndices = ObjDataTools::Data::buildTriangleVertexIndices(objData.indices.polygons, objData.indices.polygonsStarts);
		const std::vector<float> triangleVertexCoords = ObjDataTools::Data::packTriangleVertexCoords(objData.data.vertices, triangleVertexIndices);
		vertexData.insert(vertexData.end(), triangleVertexCoords.begin(), triangleVertexCoords.end());

		// Normals
		const bool hasNormals = (objData.indices.normals.size() == objData.indices.polygons.size());
		std::vector<float> triangleNormalCoords;
		if (hasNormals) {
			// TODO ROTATE NORMALS USING MODEL MATRIX
			const std::vector<glm::vec3> normals = objData.data.normals;
			const std::vector<int> normalsIndices = objData.indices.normals;
			const std::vector<int> triangleNormalIndices = ObjDataTools::Data::buildTriangleVertexIndices(normalsIndices, objData.indices.polygonsStarts);
			triangleNormalCoords = ObjDataTools::Data::packTriangleVertexCoords(normals, triangleNormalIndices);
		}
		else {
			const std::vector<glm::vec3> normals = ObjDataTools::Data::computeNormals(triangleVertexIndices, objData.data.vertices);
			triangleNormalCoords = ObjDataTools::Data::packTriangleVertexCoords(normals, triangleVertexIndices);
		}
		normalData.insert(normalData.end(), triangleNormalCoords.begin(), triangleNormalCoords.end());
	}
	bvh.build(vertexData);

	uint32_t vertexCount = vertexData.size() / 3; // 3 vertex component x,y,z
	int sqrtVertexCount = ceil(sqrt(vertexCount)); // for sqrt demension 
	int texWidthPos = Utils::powerOfTwo(sqrtVertexCount); // texture demension sqrt
	vertexData.resize(texWidthPos * texWidthPos * 3, 0.0); // for pack x,y,z to  r,g,b
	normalData.resize(texWidthPos * texWidthPos * 3, 0.0); // for pack x,y,z to  r,g,b

	vertexTexture.loadFromData(vertexData.data(), texWidthPos, texWidthPos);
	normalTexture.loadFromData(normalData.data(), texWidthPos, texWidthPos);
}

void Utils::fillBVH(
	Texture::VertexTexture& vertexTexture,
	Texture::VertexTexture& normalTexture,
	Texture::VertexTexture& bvhTexture,
	BVH::BVHBuilder& bvh,
	std::vector<Model>& models)
{
	loadModelsToTexture(vertexTexture, normalTexture, bvh, models);
	BVHNodesToTexture(bvhTexture, bvh);
}

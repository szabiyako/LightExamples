#pragma once

#include "OpenGL/Texture/VertexTexture/VertexTexture.h"
#include "BVHBuilder.h"
#include "Model/Model.h"
#include <vector>

namespace Utils
{
	inline uint32_t powerOfTwo(uint32_t value)
	{
		value--;
		value |= value >> 1;
		value |= value >> 2;
		value |= value >> 4;
		value |= value >> 8;
		value |= value >> 16;
		value++;
		return value;
	}

	void BVHNodesToTexture(Texture::VertexTexture& bvhTexture, BVH::BVHBuilder& bvh);

	void loadModelsToTexture(Texture::VertexTexture& vertexTexture, BVH::BVHBuilder& bvh, std::vector<Model>& models);
}


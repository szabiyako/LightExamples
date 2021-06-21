#pragma once

#include "Utils/NonCopyable.h"

#include "BVHBuilder.h"
#include "OpenGL/Texture/VertexTexture/VertexTexture.h"
#include "OpenGL/Texture/Texture2D/Texture2D.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/FrameBuffer.h"
#include <random>

#include "Camera.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"
#include "LightSrc/LightSrc.h"
#include "Model/Model.h"
#include <vector>

class RayTracer: public NonCopyable
{
public:
	enum class Type {
		SIMPLE,
		SMART,
		GROUND_TRUTH
	};

	RayTracer();
	~RayTracer();

	void draw(
		Camera &camera,
		const std::vector<LightSrc> &lightSources,
		const Texture::CubeMap &skyboxCubeMap);
	void updateData(std::vector<Model> &models);
	void setWindowSize(const int width, const int height);
	void setInvalid();

	int getMaxRays() const;
	void setMaxRays(const int value);
	float getResolutionScale() const;
	void setResolutionScale(const float scale);

	bool getInterpolateTextures() const;
	void setInterpolateTextures(const bool value);

	int getMaxFrames() const;
	void setMaxFrames(const int value);

	void updateTextures();
	void resetFrames();

	int getCurrentFrame() const;
private:
	//Shader getCurrentShaderPtr();

	void drawFrame(
		Camera& camera,
		const std::vector<LightSrc>& lightSources,
		const Texture::CubeMap& skyboxCubeMap);

	BVH::BVHBuilder* m_bvh = nullptr;
	Texture::VertexTexture m_bvhTexture;
	Texture::VertexTexture m_vertexTexture;
	Texture::VertexTexture m_uvTexture;
	Texture::VertexTexture m_normalTexture;

	Type m_type = Type::SMART;
	//Type m_type = Type::GROUND_TRUTH;

	bool m_isBVHvalid = false;
	int m_nRaysMax = 1;

	Shader m_simpleShader;
	Shader m_smartShader;
	Shader m_groundTruthShader;
	//Shader m_groundTruthShader;
	//Shader m_smartShader;
	//Shader m_simpleShader;
	Shader m_screenShader;
	VertexArray m_vertexArray;
	FrameBuffer m_frameBuffer0;
	FrameBuffer m_frameBuffer1;
	Texture::VertexTexture m_windowTexture0;
	Texture::VertexTexture m_windowTexture1;
	int m_width = 0;
	int m_height = 0;
	float m_resolutionScale = 1.f;
	bool m_interpolateTextures = true;

	int m_currentFrame = 1;
	int m_maxFrames = 100;

	glm::mat4 m_prevViewProj = glm::mat4(1.f);

	std::mt19937 m_randomEngine;
	std::uniform_real_distribution<> m_distribution;
};


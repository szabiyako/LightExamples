#pragma once

#include "Utils/NonCopyable.h"

#include "BVHBuilder.h"
#include "OpenGL/Texture/VertexTexture/VertexTexture.h"
#include "OpenGL/Texture/Texture2D/Texture2D.h"
#include "OpenGL/Shader.h"
#include "OpenGL/VertexArray.h"
#include "OpenGL/FrameBuffer.h"

#include "Camera.h"
#include "OpenGL/Texture/CubeMap/CubeMap.h"
#include "LightSrc/LightSrc.h"
#include "Model/Model.h"
#include <vector>

class RayTracer: public NonCopyable
{
public:
	RayTracer();
	~RayTracer();

	void draw(
		Camera &camera,
		const std::vector<LightSrc> &lightSources,
		const Texture::CubeMap &skyboxCubeMap);
	void updateData(std::vector<Model> &models);
	void setWindowSize(const int width, const int height);
	void setInvalid();

	int& getMaxRaysRef();
	float getResolutionScale() const;
	void setResolutionScale(const float scale);

	void resetFrames();
private:
	BVH::BVHBuilder* m_bvh = nullptr;
	Texture::VertexTexture m_bvhTexture;
	Texture::VertexTexture m_vertexTexture;
	Texture::VertexTexture m_uvTexture;
	Texture::VertexTexture m_normalTexture;

	bool m_isBVHvalid = false;
	int m_nRaysMax = 1;

	Shader m_shader;
	Shader m_screenShader;
	VertexArray m_vertexArray;
	FrameBuffer m_frameBuffer0;
	FrameBuffer m_frameBuffer1;
	Texture::Texture2D m_windowTexture0;
	Texture::Texture2D m_windowTexture1;
	int m_width = 0;
	int m_height = 0;
	float m_resolutionScale = 1.f;

	int m_currentFrame = 1;

	glm::mat4 m_prevViewProj = glm::mat4(1.f);
};


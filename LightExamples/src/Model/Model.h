#pragma once

#include "LoadableData/LoadableData.h"
#include "Drawable/Drawable.h"
#include "glm/glm.hpp"
#include "LightSrc/LightSrc.h"

class Model
{
public:
	enum class RenderType
	{
		DEFAULT,
		GOURAUD,
		PHONG,
		BLINN_PHONG,
		PBR,
		RAYTRACING
	};

	Model(std::vector<LightSrc> *lightSources = nullptr);
	~Model();
	void deleteDrawable();

	Drawable getDrawable() const;
	void setRenderType(const RenderType& type);
	RenderType getRenderType() const;
	glm::mat4x4 getModelMatrix() const;

	glm::vec3& getPosRef();
	glm::vec3& getRotationRef();
	float& getScaleRef();

	void updateModelMatrix();

	glm::vec3 getColor() const;
	void setColor(const glm::vec3& color);

	bool isVisible() const;
	void setVisible(const bool value);

	LoadableData& getLoadableDataRef();
	Drawable& getDrawableRef();
private:
	LoadableData m_loadableData;
	Drawable m_drawable;

	std::vector<LightSrc> *m_lightSources;

	RenderType m_renderType = RenderType::DEFAULT;

	bool m_cullFaces = true;
	glm::vec3 m_color = { 1.f, 1.f, 1.f };

	glm::vec3 m_position = { 0.f, 0.f, 0.f };
	glm::vec3 m_rotation = { 0.f, 0.f, 0.f };
	float m_scale = 1.f;
	glm::mat4x4 m_modelMatrix = glm::mat4(1.f);
};


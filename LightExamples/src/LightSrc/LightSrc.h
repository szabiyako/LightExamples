#pragma once

#include "Drawable/Drawable.h"
#include "glm/glm.hpp"
#include <vector>

class Model;

class LightSrc
{
public:
	enum class Type {
		DIRECTIONAL,
		POINT,
		SPOTLIGHT
	};

	LightSrc();
	~LightSrc();
	void deleteDrawable();

	Drawable getDrawable() const;
	void setType(const Type &type);
	void setEnabled(const bool value);
	void setShadowsEnabled(const bool value);
	Type getType() const;
	bool isEnabled() const;
	bool isShadowsEnabled() const;
	glm::mat4x4 getModelMatrix() const;

	bool& getEnabledRef();
	bool& getShadowsEnabledRef();

	glm::vec3& getPosRef();
	glm::vec3& getRotationRef();
	float& getScaleRef();

	void updateModelMatrix();
	void updateDirection();

	glm::vec3 getPos() const;
	glm::vec3 getDir() const;

	glm::vec3 getLightColor() const;
	void setLightColor(const glm::vec3 &color);

	glm::vec3 getModelColor() const;
	void setModelColor(const glm::vec3& color);

	bool isVisible() const;
	void setVisible(const bool value);
private:
	Drawable m_drawable;
	glm::mat4x4 m_modelMatrix = glm::mat4(1.f);
	glm::vec3 m_modelColor = { 1.f, 1.f, 1.f };
	glm::vec3 m_lightColor = { 1.f, 1.f, 1.f };

	// Shader data
	Type m_type = Type::DIRECTIONAL;
	bool m_isEnabled = true;
	bool m_isShadowsEnabled = false;
	glm::vec3 m_position = { 0.f, 0.f, 0.f };
	glm::vec3 m_rotation = { 0.f, 0.f, 0.f };
	glm::vec3 m_dir = { 0.f, 0.f, -1.f };
	float m_scale = 1.f;
};


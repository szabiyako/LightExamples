#pragma once

#include "Drawable/Drawable.h"
#include "glm/glm.hpp"

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

	void setType(const Type &type);
	void setEnabled(const bool value);
	void setShadowsEnabled(const bool value);
	Type getType() const;
	bool isEnabled() const;
	bool isShadowsEnabled() const;
private:
	Drawable m_drawable;
	glm::mat4x4 m_modelMatrix = glm::mat4(1.f);

	// Shader data
	Type m_type = Type::DIRECTIONAL;
	bool m_isEnabled = true;
	bool m_isShadowsEnabled = false;
	glm::vec3 m_lightPos = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 m_lightDir = glm::vec3(0.f, 0.f, -1.f);
};


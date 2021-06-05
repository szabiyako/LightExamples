#pragma once

#include "OpenGL/Texture/Texture.h"
#include "LoadableData/Image/Image.h"
#include <GL/glew.h>
#include "glm/glm.hpp"
#include <vector>

namespace Texture {
class CubeMap : public Texture
{
public:
	CubeMap();

	virtual ~CubeMap() override;

	virtual void bind(unsigned int slot = 0) const override;
	virtual void unbind() const override;

	virtual void setAnisotropicLevel(float level) override;

	void loadFromColor(const glm::vec3 &color);
	/*
	* Images sequence:
	* ----------------
	* right
	* left
	* bottom
	* top
	* front
	* back
	*/
	void loadFromImages(const std::vector<Image>& images, const GLint textureFromat = GL_RGBA8);
private:
	virtual void setupParams();
};
}

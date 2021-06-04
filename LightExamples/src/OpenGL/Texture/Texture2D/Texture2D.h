#pragma once

#include "OpenGL/Texture/Texture.h"
#include "LoadableData/Image/Image.h"
#include <GL/glew.h>

namespace Texture {
class Texture2D : public Texture
{
public:
	Texture2D();

	virtual ~Texture2D() override;

	virtual void bind(unsigned int slot = 0) const override;
	virtual void unbind() const override;

	virtual void setAnisotropicLevel(float level) override;

	void loadFromData(const void* data, const int width, const int height, const GLint dataFromat = GL_RGBA, const GLint textureFromat = GL_RGBA8);
	void loadFromImage(const Image &image, const GLint textureFromat = GL_RGBA8);
};
}

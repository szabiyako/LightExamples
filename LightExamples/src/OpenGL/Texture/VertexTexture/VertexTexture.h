#pragma once

#include "OpenGL/Texture/Texture.h"
#include <GL/glew.h>

namespace Texture {
class VertexTexture : public Texture
{
public:
	VertexTexture();

	virtual ~VertexTexture() override;

	virtual void bind(unsigned int slot = 0) const override;
	virtual void unbind() const override;

	virtual void setAnisotropicLevel(float level) override;

	void loadFromData(const void* data, const int width, const int height);
private:
	virtual void setupParams();
};
}

#pragma once

#include "OpenGL/Texture/Texture.h"

#include "LoadableData/Image/Image.h"

namespace Texture {
class RGBA8_2D : public Texture
{
public:
	RGBA8_2D();

	virtual ~RGBA8_2D() override;

	virtual void bind(unsigned int slot = 0) const override;
	virtual void unbind() const override;

	virtual void setAnisotropicLevel(float level) override;

	void loadFromImage(const Image &image);
};
}

#pragma once

#include "Utils/NonCopyable.h"

namespace Texture {
class Texture : public NonCopyable
{
public:
	Texture();
	virtual ~Texture();

	virtual void bind(unsigned int slot = 0) const = 0;
	virtual void unbind() const = 0;

	virtual void setAnisotropicLevel(float level) = 0;

	unsigned int getID() const;
	unsigned int getWidth() const;
	unsigned int getHeight() const;

	static float getMaxAnisotropicLevel();
protected:
	unsigned int m_id = 0;
	int m_width = 0;
	int m_height = 0;
};
}

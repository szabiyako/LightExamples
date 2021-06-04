#include "Texture.h"

#include <GL/glew.h>
#include "OpenGl/ErrorHandler.h"


Texture::Texture::Texture()
{

}

Texture::Texture::~Texture()
{

}

unsigned int Texture::Texture::getID() const
{
	return m_id;
}
unsigned int Texture::Texture::getWidth() const
{
	return m_width;
}
unsigned int Texture::Texture::getHeight() const
{
	return m_height;
}

float Texture::Texture::getMaxAnisotropicLevel()
{
	float fLargest;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &fLargest);
	return fLargest;
}
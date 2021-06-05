#include "Texture2D.h"

#include "LoadableData/Image/Image.h"

#include "OpenGL/ErrorHandler.h"

using namespace Texture;

Texture2D::Texture2D()
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	setupParams();

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_id);
}

void Texture2D::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::setAnisotropicLevel(float level)
{
	bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}

void Texture2D::loadFromData(const void* data, const int width, const int height, const GLint dataFromat, const GLint textureFromat)
{
	bind();
	m_width = width;
	m_height = height;
	//                               | Type output                          | Type input   | Data size per channel
	//                          | level of detail 0 - max            | pixels in Border
	glTexImage2D(GL_TEXTURE_2D, 0, textureFromat, m_width, m_height, 0, dataFromat, GL_UNSIGNED_BYTE, data);
	setupParams();
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture2D::loadFromImage(const Image& image, const GLint textureFromat)
{
	loadFromData(image.data.data(), image.width, image.height, GL_RGBA, textureFromat);
}

void Texture2D::setupParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	setAnisotropicLevel(getMaxAnisotropicLevel());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

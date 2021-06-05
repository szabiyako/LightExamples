#include "VertexTexture.h"

#include "LoadableData/Image/Image.h"

#include "OpenGL/ErrorHandler.h"

using namespace Texture;

VertexTexture::VertexTexture()
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	setupParams();

	glBindTexture(GL_TEXTURE_2D, 0);
}

VertexTexture::~VertexTexture()
{
	glDeleteTextures(1, &m_id);
}

void VertexTexture::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void VertexTexture::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void VertexTexture::setAnisotropicLevel(float level)
{
	bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}

void VertexTexture::loadFromData(const void* data, const int width, const int height)
{
	bind();
	m_width = width;
	m_height = height;
	//                               | Type output                          | Type input   | Data size per channel
	//                          | level of detail 0 - max            | pixels in Border
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data);

	setupParams();
}

void VertexTexture::setupParams()
{
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

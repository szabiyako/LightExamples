#include "VertexTexture.h"

#include "LoadableData/Image/Image.h"

#include "OpenGL/ErrorHandler.h"

using namespace Texture;

VertexTexture::VertexTexture()
{
	GLCall(glGenTextures(1, &m_id));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

VertexTexture::~VertexTexture()
{
	GLCall(glDeleteTextures(1, &m_id));
}

void VertexTexture::bind(unsigned int slot /*= 0*/) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_id));
}

void VertexTexture::unbind() const
{
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void VertexTexture::setAnisotropicLevel(float level)
{
	bind();
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level));
}

void VertexTexture::loadFromData(const void* data, const int width, const int height)
{
	bind();
	m_width = width;
	m_height = height;
	//                               | Type output                          | Type input   | Data size per channel
	//                          | level of detail 0 - max            | pixels in Border
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, m_width, m_height, 0, GL_RGB, GL_FLOAT, data));

	setupParams();
}

void VertexTexture::setupParams()
{
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
}

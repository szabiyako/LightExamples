#include "RGBA8_2D.h"

#include "LoadableData/Image/Image.h"
#include "LoadableData/Image/Tools/Import/Import.h"

#include <GL/glew.h>
#include "OpenGL/ErrorHandler.h"

using namespace Texture;

RGBA8_2D::RGBA8_2D()
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_2D, m_id);

	//for pixilisation
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	//Default
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//For no repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	//For repeat
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//float borderColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

	//16xAnisfiltr
	setAnisotropicLevel(getMaxAnisotropicLevel());

	////                               | Type output                   | Type input   | Data size per channel
	////                          | level of detail 0 - max       | pixels in Border
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data.data());
	//glGenerateMipmap(GL_TEXTURE_2D);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
}

//OnecoloredTexture
//Texture::Texture(const unsigned char& r /*= 255*/, const unsigned char& g /*= 255*/, const unsigned char& b /*= 255*/)
//	: m_id(0), m_filePath("None"), /*m_localBuffer(nullptr),*/ m_width(1), m_height(1)//, m_bitsPerPixel(8)
//{
//	unsigned char tmp[4] = { r, g, b, 255 };
//
//	glGenTextures(1, &m_id);
//	glBindTexture(GL_TEXTURE_2D, m_id);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//
//	setAnisotropicLevel(getMaxAnisotropicLevel());
//
//	//                          | level                         | pixels in Border
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tmp);
//	glGenerateMipmap(GL_TEXTURE_2D);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	glBindTexture(GL_TEXTURE_2D, 0);
//}

RGBA8_2D::~RGBA8_2D()
{
	glDeleteTextures(1, &m_id);
}

void RGBA8_2D::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_id);
}

void RGBA8_2D::unbind() const
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

void RGBA8_2D::setAnisotropicLevel(float level)
{
	bind();
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}

void RGBA8_2D::loadFromImage(const Image& image)
{
	bind();
	m_width = image.width;
	m_height = image.height;
	//                               | Type output                   | Type input   | Data size per channel
	//                          | level of detail 0 - max       | pixels in Border
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data.data());
	glGenerateMipmap(GL_TEXTURE_2D);
}

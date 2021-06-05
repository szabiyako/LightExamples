#include "CubeMap.h"

#include "LoadableData/Image/Image.h"

#include "OpenGL/ErrorHandler.h"

using namespace Texture;

CubeMap::CubeMap()
{
	glGenTextures(1, &m_id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	setAnisotropicLevel(getMaxAnisotropicLevel());

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
	glDeleteTextures(1, &m_id);
}

void CubeMap::bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

void CubeMap::unbind() const
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void CubeMap::setAnisotropicLevel(float level)
{
	bind();
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAX_ANISOTROPY_EXT, level);
}

void CubeMap::loadFromColor(const glm::vec3& color)
{
	bind();
	unsigned int width = 1, height = 1;
	unsigned char colorData[4] = { 
		unsigned char(glm::clamp(color.x, 0.f, 1.f) * 255),
		unsigned char(glm::clamp(color.y, 0.f, 1.f) * 255),
		unsigned char(glm::clamp(color.z, 0.f, 1.f) * 255), 0xFF
	};
	m_width = width;
	m_height = height;
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, GL_RGBA8, (GLsizei)m_width, (GLsizei)m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, colorData
		);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

void CubeMap::loadFromImages(const std::vector<Image>& images, const GLint textureFromat)
{
	bind();
	const size_t nImages = images.size();
	ASSERT(nImages == 6);
	m_width = images[0].width;
	m_height = images[0].height;
	for (GLuint i = 0; i < nImages; ++i) {
		ASSERT((m_width == images[i].width) && (m_height == images[i].height)); // Images has different sizes!
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0, textureFromat, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, images[i].data.data()
		);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}
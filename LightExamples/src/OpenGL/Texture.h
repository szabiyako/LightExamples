#pragma once

#include <string>

//
#include "glm/glm.hpp"

#include "Utils/NonCopyable.h"

class Texture : public NonCopyable
{
private:
	unsigned int m_id;
	std::string m_filePath;
	unsigned char *m_localBuffer;
	int m_width;
	int m_height;
	int m_bitsPerPixel; //bits per pixel
public:
	Texture(const std::string& filepath);

	//OnecoloredTexture
	Texture(const unsigned char& r = 255, const unsigned char& g = 255, const unsigned char& b = 255);
	~Texture();

	void bind(unsigned int slot = 0) const;
	void unbind() const;

	void setAnisotropicLevel(float level);
	static float getMaxAnisotropicLevel();

	//temp
	inline unsigned int getID() const { return m_id; }
	inline unsigned int getWidth() const { return m_width; }
	inline unsigned int getHeight() const { return m_height; }
};


#pragma once

#include <vector>

#include <GL/glew.h>
#include "ErrorHandler.h"

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;

	static unsigned int getSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:				return 4;
			case GL_UNSIGNED_INT:		return 2;
			case GL_UNSIGNED_BYTE:		return 1;
		}
		ASSERT(false);
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_elements;
	unsigned int m_stride;
public:
	VertexBufferLayout()
		: m_stride(0) {}

	template <typename T>
	void Push(unsigned int count, bool normalized = false)
	{
		static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count, bool normalized)
	{
		this->m_elements.push_back({ GL_FLOAT, count, normalized });
		m_stride += VertexBufferElement::getSizeOfType(GL_FLOAT) * count;
	}

	template<>
	void Push<unsigned int>(unsigned int count, bool normalized)
	{
		this->m_elements.push_back({ GL_UNSIGNED_INT, count, normalized });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_INT) * count;
	}

	template<>
	void Push<unsigned char>(unsigned int count, bool normalized)
	{
		this->m_elements.push_back({ GL_UNSIGNED_BYTE, count, normalized });
		m_stride += VertexBufferElement::getSizeOfType(GL_UNSIGNED_BYTE) * count;
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return m_elements; }
	inline unsigned int GetStride() const { return m_stride; }
};


#include "IndexBuffer.h"

#include <GL/glew.h>
#include "ErrorHandler.h"

IndexBuffer::IndexBuffer()
	: m_count(0)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &this->m_id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id));
}

IndexBuffer::IndexBuffer(const unsigned int * data, unsigned int count, DrawType type /*= DrawType::STATIC*/)
	: m_count(count)
{
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &this->m_id));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id));
	if (type == DrawType::STATIC)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	else if (type == DrawType::DYNAMIC)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
	}
	else if (type == DrawType::STREAM)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STREAM_DRAW));
	}
}

IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &this->m_id));
}

void IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id));
}

void IndexBuffer::unbind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void IndexBuffer::create(const void * data, const unsigned int & count, DrawType type)
{
	m_count = count;
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id));
	if (type == DrawType::STATIC)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
	}
	else if (type == DrawType::DYNAMIC)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_DYNAMIC_DRAW));
	}
	else if (type == DrawType::STREAM)
	{
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STREAM_DRAW));
	}
}

void IndexBuffer::setData(const unsigned int * data, const unsigned int & count, const unsigned int & Position_in_Memory)
{
	m_count = count;
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_id));
	GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, Position_in_Memory, count * sizeof(unsigned int), data));
}

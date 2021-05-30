#include "VertexArray.h"
#include "VertexBufferLayout.h"

#include <GL/glew.h>
#include "ErrorHandler.h"

VertexArray::VertexArray()
{
	GLCall(glGenVertexArrays(1, &m_id));
	GLCall(glBindVertexArray(m_id));
}

VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_id));
}

void VertexArray::addBuffer(const VertexBuffer & vb, const VertexBufferLayout & layout)
{
	bind();
	vb.bind();
	const auto& elements = layout.GetElements();
	unsigned int elSize = (unsigned int)elements.size();
	size_t offset = 0;
	for (unsigned int i = 0; i < elSize; i++)
	{
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, 
			element.normalized, layout.GetStride(), (const void*)offset);
		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}
}

void VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_id));
}

void VertexArray::unbind() const
{
	GLCall(glBindVertexArray(0));
}

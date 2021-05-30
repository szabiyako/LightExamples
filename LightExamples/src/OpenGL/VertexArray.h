#pragma once

#include "VertexBuffer.h"

class VertexBufferLayout;

#include "Utils/NonCopyable.h"

class VertexArray : public NonCopyable
{
private:
	unsigned int m_id;
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	void unbind() const;
};

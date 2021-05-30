#pragma once

#include "Utils/NonCopyable.h"

class VertexBuffer : public NonCopyable
{
private:
	unsigned int m_RendererID;
public:
	enum class DrawType
	{
		STATIC,
		DYNAMIC,
		STREAM
	};
	VertexBuffer();
	VertexBuffer(const void* data, unsigned int size, DrawType type = DrawType::STATIC);
	~VertexBuffer();


	void create(const void* data, const unsigned int& size, DrawType type = DrawType::STATIC) const;
	void setData(const void* data, const unsigned int& size_in_bytes, const unsigned int& Position_in_Memory) const;

	void bind() const;
	void unbind() const;
};


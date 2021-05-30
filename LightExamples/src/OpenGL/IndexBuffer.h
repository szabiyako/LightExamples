#pragma once

#include "Utils/NonCopyable.h"

class IndexBuffer : public NonCopyable
{
private:
	unsigned int m_id;
	unsigned int m_count;
public:
	enum class DrawType
	{
		STATIC,
		DYNAMIC,
		STREAM
	};

	IndexBuffer();
	IndexBuffer(const unsigned int* data, unsigned int count, DrawType type = DrawType::STATIC);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	void create(const void* data, const unsigned int& count, DrawType type = DrawType::STATIC);
	void setData(const unsigned int* data, const unsigned int& count, const unsigned int& Position_in_Memory);

	inline unsigned int getCount() const { return m_count; }
};


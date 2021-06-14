#pragma once

#include "Utils/NonCopyable.h"

class FrameBuffer : public NonCopyable
{
public:
	FrameBuffer();
	virtual ~FrameBuffer();

	virtual void bind() const;
	virtual void unbind() const;

	unsigned int getID() const;
protected:
	unsigned int m_id = 0;
};

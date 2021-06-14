#include "FrameBuffer.h"

#include <GL/glew.h>
#include "OpenGL/ErrorHandler.h"

FrameBuffer::FrameBuffer()
{
    GLCall(glGenFramebuffers(1, &m_id));
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

FrameBuffer::~FrameBuffer()
{
    GLCall(glDeleteFramebuffers(1, &m_id));
}

void FrameBuffer::bind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

void FrameBuffer::unbind() const
{
    GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

unsigned int FrameBuffer::getID() const
{
    return m_id;
}

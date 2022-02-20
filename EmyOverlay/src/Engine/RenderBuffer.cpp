#include "RenderBuffer.h"

RenderBuffer::RenderBuffer(unsigned int width, unsigned int height,GLenum format)
{
	glGenRenderbuffers(1, &m_RendererID);
	Bind();
	glRenderbufferStorage(GL_RENDERBUFFER, format, width, height);
}

void RenderBuffer::Bind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, m_RendererID);
}

void RenderBuffer::Unbind() const
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

}

void RenderBuffer::Destroy()
{
	glDeleteRenderbuffers(1, &m_RendererID);
}

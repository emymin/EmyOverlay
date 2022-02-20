#include "FrameBuffer.h"
#include "Console.h"


FrameBuffer::FrameBuffer(unsigned int width, unsigned int height, unsigned int format, unsigned int internal_format) : m_InternalFormat(internal_format),m_Format(format),m_Width(width),m_Height(height)
{
	glGenFramebuffers(1, &m_RendererID);
	Resize(width, height);
}

void FrameBuffer::Bind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
	glViewport(0, 0, m_Width, m_Height);
}

void FrameBuffer::Unbind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0,1000,1000); //TODO remove placeholder
}

void FrameBuffer::Destroy()
{
	glDeleteFramebuffers(1, &m_RendererID);
}

void FrameBuffer::Resize(unsigned int width, unsigned int height)
{
	m_Width = width;
	m_Height = height;

	m_ColorTexture.Destroy();
	m_DepthStencilBuffer.Destroy();
	Bind();
	
	m_DepthStencilBuffer = RenderBuffer(width, height, GL_DEPTH24_STENCIL8);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthStencilBuffer.m_RendererID);
	
	m_ColorTexture = Texture(NULL, width, height, m_InternalFormat,m_Format, GL_UNSIGNED_BYTE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorTexture.m_RendererID, 0);
	
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Console::Error("FrameBuffer not complete!");
	}
	else {
		Console::Log("Framebuffer completed");
	}
	Unbind();

}

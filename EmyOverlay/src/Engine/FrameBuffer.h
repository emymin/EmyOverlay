#pragma once
#include <glad/glad.h>
#include "RenderBuffer.h"
#include "Texture.h"
class FrameBuffer
{
private:
	unsigned int m_RendererID;
	RenderBuffer m_DepthStencilBuffer;
	Texture m_ColorTexture;
	unsigned int m_InternalFormat;
	unsigned int m_Format;
	unsigned int m_Width, m_Height;
public:
	FrameBuffer(unsigned int width,unsigned int height,unsigned int format=GL_RGB,unsigned int internal_format=GL_RGB);
	void Bind() const;
	void Unbind() const;
	void Destroy();
	void Resize(unsigned int width, unsigned int height);
	unsigned int GetRendererID() { return m_RendererID; }
	Texture* GetTexture() { return &m_ColorTexture; }
	friend class DesktopRenderer;
	friend class OpenVRRenderer;
};


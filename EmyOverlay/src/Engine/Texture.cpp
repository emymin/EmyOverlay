#include "Texture.h"
#include "Console.h"



Texture::Texture(unsigned char* buffer,int width,int height,unsigned int internalformat,unsigned int format,unsigned int type):m_LocalBuffer(buffer),m_Width(width),m_Height(height),m_BPP(0),m_RendererID(0)
{
	glGenTextures(1, &m_RendererID);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, m_Width, m_Height, 0, format, type, m_LocalBuffer);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	Unbind();
}


void Texture::Destroy()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(unsigned int slot /*= 0*/) const
{
	glActiveTexture(GL_TEXTURE0+slot);
	glBindTexture(GL_TEXTURE_2D, m_RendererID);
}

void Texture::Unbind(unsigned int slot)const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}


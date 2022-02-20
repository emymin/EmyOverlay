#pragma once

#include <glad/glad.h>
#include <iostream>
#include <unordered_map>

class Texture
{
private:
	unsigned int m_RendererID;
	unsigned char* m_LocalBuffer;
	int m_Width, m_Height, m_BPP;
public:
	Texture() {}
	Texture(unsigned char* buffer,int width,int height, unsigned int internalformat = GL_RGBA8, unsigned int format = GL_RGBA, unsigned int type = GL_UNSIGNED_BYTE);

	void Bind(unsigned int slot = 0) const;
	void Unbind(unsigned int slot = 0)const;
	void Destroy();

	inline int GetWidth() const { return m_Width; }
	inline int GetHeight() const { return m_Height; }

	unsigned int GetRendererID() { return m_RendererID; }
	friend class FrameBuffer;
};


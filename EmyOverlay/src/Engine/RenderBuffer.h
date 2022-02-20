#pragma once
#include <glad/glad.h>
class RenderBuffer
{
private:
	unsigned int m_RendererID;
	RenderBuffer() {}
public:
	RenderBuffer(unsigned int width, unsigned int height,GLenum format);
	void Bind() const;
	void Unbind() const;
	void Destroy();

	friend class FrameBuffer;
};


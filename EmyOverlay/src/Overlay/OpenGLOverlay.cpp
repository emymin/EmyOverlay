#include "OpenGLOverlay.h"

OpenGLOverlay::OpenGLOverlay(std::string name, uint32_t width, uint32_t height) : Overlay(name), m_Buffer(width, height, GL_RGBA, GL_RGBA8), m_Width(width), m_Height(height) {
	vr::EColorSpace colorSpace = vr::ColorSpace_Gamma;
	vr::ETextureType textureType = vr::TextureType_OpenGL;
	m_VRTexture = { (void*)(uintptr_t)(m_Buffer.GetTexture()->GetRendererID()),textureType,colorSpace };
}

void OpenGLOverlay::Draw()
{
	m_Buffer.Bind();
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT);
	OnDraw();
	m_Buffer.Unbind();
	SetTexture(&m_VRTexture);
}

void OpenGLOverlay::DrawBufferToScreen()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_Buffer.GetRendererID());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000,
		GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

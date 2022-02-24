#pragma once
#include "Overlay.h"
#include "../Engine/FrameBuffer.h"

class OpenGLOverlay : public Overlay
{
public:
	OpenGLOverlay(std::string name, uint32_t width, uint32_t height);

	virtual void OnDraw() = 0;
	virtual void Draw() override;
	void DrawBufferToScreen();
private:
	FrameBuffer m_Buffer;
	vr::Texture_t m_VRTexture;
protected:
	uint32_t m_Width, m_Height;
};


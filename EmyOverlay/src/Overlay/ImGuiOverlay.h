#pragma once

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include "imgui_impl_glfw.h"
#include "OpenGLOverlay.h"

class ImGuiOverlay : public OpenGLOverlay
{
public:
	ImGuiOverlay(std::string name, uint32_t width, uint32_t height) : OpenGLOverlay(name, width, height){}

	virtual void OnDraw() override;

	virtual void OnGui() = 0;

};


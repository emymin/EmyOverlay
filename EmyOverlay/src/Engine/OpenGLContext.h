#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

class OpenGLContext
{
public:
	OpenGLContext(bool showWindow=false) {
		InitGL(1000,1000,showWindow);
	}
	~OpenGLContext() {
		Destroy();
	}

	bool ShouldClose() {
		return glfwWindowShouldClose(window);
	}
	GLFWwindow* window;
private:
	void InitGL(uint32_t width,uint32_t height,bool showWindow);
	void Destroy();
};


#include "OpenGLContext.h"
#include "Console.h"
void APIENTRY glDebugOutput(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam)
{
    // ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

    Console::Error("========== OPENGL ERROR ==========");
    Console::Error(message);

    switch (source)
    {
    case GL_DEBUG_SOURCE_API:             Console::Error("Source: API"); break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   Console::Error("Source: Window System"); break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER: Console::Error("Source: Shader Compiler"); break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:     Console::Error("Source: Third Party"); break;
    case GL_DEBUG_SOURCE_APPLICATION:     Console::Error("Source: Application"); break;
    case GL_DEBUG_SOURCE_OTHER:           Console::Error("Source: Other"); break;
    }

    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               Console::Error("Type: Error"); break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: Console::Error("Type: Deprecated Behaviour"); break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  Console::Error("Type: Undefined Behaviour"); break;
    case GL_DEBUG_TYPE_PORTABILITY:         Console::Error("Type: Portability"); break;
    case GL_DEBUG_TYPE_PERFORMANCE:         Console::Error("Type: Performance"); break;
    case GL_DEBUG_TYPE_MARKER:              Console::Error("Type: Marker"); break;
    case GL_DEBUG_TYPE_PUSH_GROUP:          Console::Error("Type: Push Group"); break;
    case GL_DEBUG_TYPE_POP_GROUP:           Console::Error("Type: Pop Group"); break;
    case GL_DEBUG_TYPE_OTHER:               Console::Error("Type: Other"); break;
    }

    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:         Console::Error("Severity: high"); break;
    case GL_DEBUG_SEVERITY_MEDIUM:       Console::Error("Severity: medium"); break;
    case GL_DEBUG_SEVERITY_LOW:          Console::Error("Severity: low"); break;
    case GL_DEBUG_SEVERITY_NOTIFICATION: Console::Error("Severity: notification"); break;
    }
}

void OpenGLContext::InitGL(uint32_t width,uint32_t height,bool showWindow)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!showWindow) {
        glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); //offscreen rendering
    }

	window = glfwCreateWindow(width,height, "test", NULL, NULL);
	if (window == NULL) {
        Console::Error("Error creating window");
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(0); //no vsync
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Console::Error("Failed to initialize GLAD");
	}

	//SET CALLBACKS
	/*
	glfwSetFramebufferSizeCallback(_instance->window, framebuffer_size_callback);
	glfwSetKeyCallback(_instance->window, input_callback);
	glfwSetWindowFocusCallback(_instance->window, window_focus_callback);
	*/
#if _DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
	int flags; glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
	if (GL_CONTEXT_FLAG_DEBUG_BIT) {
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(glDebugOutput, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
	}
#endif
	glViewport(0, 0, width, height);
	glClearColor(0.f, 0.f, 0.f, 1.0f);
	Console::Log("OpenGL initialized");

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 430");
    Console::Log("ImGui initialized");
}

void OpenGLContext::Destroy()
{
    Console::Log("Shutting down...");
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

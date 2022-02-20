#include <stdio.h>
#include "Overlay.h"
#include "Engine/OpenGLContext.h"
#include "Engine/FrameBuffer.h"

using namespace vr;

void check_error(int line, EVRInitError error) { if (error != 0) printf("%d: error %s\n", line, VR_GetVRInitErrorAsSymbol(error)); }

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    EVRInitError error;
    VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);

    Overlay overlay("test");
    overlay.SetWidth(1);
    overlay.Show();

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, -2.0f
    };
    overlay.SetTransform(transform);

    OpenGLContext context;
    FrameBuffer buffer(1000, 1000);
   

    vr::EColorSpace colorSpace = vr::ColorSpace_Gamma;
    vr::ETextureType textureType = vr::TextureType_OpenGL;
    vr::Texture_t frameTexture = { (void*)(buffer.GetTexture()->GetRendererID()),textureType,colorSpace };

    overlay.SetTexture(&frameTexture);
    overlay.Show();

    while (!context.ShouldClose()) {

        buffer.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        buffer.Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer.GetRendererID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);



        glfwSwapBuffers(context.window);
        glfwPollEvents();

    }
    return 0;
}
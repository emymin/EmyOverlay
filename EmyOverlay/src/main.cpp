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

    Overlay overlay("EmyOverlay");
    overlay.SetWidth(1);

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, -2.0f
    };
    overlay.SetTransform(transform);

    OpenGLContext context;
    FrameBuffer buffer(1000, 1000,GL_RGBA,GL_RGBA8);

   

    vr::EColorSpace colorSpace = vr::ColorSpace_Gamma;
    vr::ETextureType textureType = vr::TextureType_OpenGL;
    vr::Texture_t frameTexture = { (void*)(uintptr_t)(buffer.GetTexture()->GetRendererID()),textureType,colorSpace };

    overlay.SetTexture(&frameTexture);
    
    overlay.Show();

    while (!context.ShouldClose()) {

        buffer.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Console::Log(std::to_string(io.MousePos.x) + " " + std::to_string(io.MousePos.y));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        
        vr::VREvent_t event;
        while (vr::VROverlay()->PollNextOverlayEvent(overlay.m_Handle, &event, sizeof(event))) {
            switch (event.eventType) {
            case vr::VREvent_MouseMove: {
                //Console::Log(std::to_string(event.data.mouse.x)+" "+std::to_string(event.data.mouse.y));
                
                ImGui::GetIO().AddMousePosEvent(event.data.mouse.x*1000, event.data.mouse.y*1000);
                break;
            }
            case vr::VREvent_MouseButtonDown: {
                ImGui::GetIO().AddMouseButtonEvent(0, true);
            }
            case vr::VREvent_MouseButtonUp: {
                ImGui::GetIO().AddMouseButtonEvent(0, false);
            }
            }
        }
        ImGui::ShowDemoWindow();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        buffer.Unbind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        

        glBindFramebuffer(GL_READ_FRAMEBUFFER, buffer.GetRendererID());
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBlitFramebuffer(0, 0, 1000, 1000, 0, 0, 1000, 1000,
            GL_COLOR_BUFFER_BIT, GL_NEAREST);


        overlay.SetTexture(&frameTexture);

       // overlay.Show();

        glfwSwapBuffers(context.window);
        glfwPollEvents();

    }
    return 0;
}

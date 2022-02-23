#include <stdio.h>
#include "Overlay.h"
#include "Engine/OpenGLContext.h"
#include "Engine/FrameBuffer.h"
#include "Utils/Matrices.h"
#include "Utils/Vectors.h"
#include "Utils/Util.h"

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

    static vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];

    glClearColor(0, 0, 0, 0);

    while (!context.ShouldClose()) {

        buffer.Bind();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        //Console::Log(std::to_string(io.MousePos.x) + " " + std::to_string(io.MousePos.y));


        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        vr::VROverlayIntersectionResults_t results;
        vr::VROverlayIntersectionParams_t params;

        TrackedDeviceIndex_t index = 2;
        vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];

        vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, poses, vr::k_unMaxTrackedDeviceCount);
        
        TrackedDevicePose_t controllerPose = poses[2];
        
        vr::VROverlay()->SetOverlayTransformAbsolute(overlay.m_Handle, vr::TrackingUniverseStanding, &poses[1].mDeviceToAbsoluteTracking);

        Matrix4 mat_controller = poses[2].mDeviceToAbsoluteTracking;
        mat_controller = mat_controller * GetControllerTipMatrix(true);

        Vector3 pos = mat_controller.getTranslation();
        Vector3 forward = { mat_controller[8], mat_controller[9], mat_controller[10] };
        forward *= -1.0f;

        params.eOrigin = vr::TrackingUniverseStanding;
        params.vSource = { pos.x, pos.y, pos.z };
        params.vDirection = { forward.x, forward.y, forward.z };


        auto intersects = vr::VROverlay()->ComputeOverlayIntersection(overlay.m_Handle, &params, &results);
        float x = results.vUVs.v[0]*1000;
        float y = (results.vUVs.v[1])*1000;
        ImGui::GetIO().AddMousePosEvent(x, y);



        vr::VREvent_t event;
        while (vr::VROverlay()->PollNextOverlayEvent(overlay.m_Handle, &event, sizeof(event))) {
            switch (event.eventType) {
            case vr::VREvent_MouseMove: {
                
                //ImGui::GetIO().AddMousePosEvent(event.data.mouse.x*1000, event.data.mouse.y*1000);
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

        ImGui::Begin("Controller Data");
        ImGui::Text("Controller Pos: %.4f %.4f %.4f",pos.x,pos.y,pos.z);
        ImGui::Text("Controller Forward: %.4f %.4f %.4f",forward.x,forward.y,forward.z);
        ImGui::Text("Intersects: %d at %.4f %.4f", intersects, x, y);

        ImGui::End();


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

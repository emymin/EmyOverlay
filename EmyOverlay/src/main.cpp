#include <stdio.h>
#include "Overlays/MainOverlay.h"
#include "Overlay/OverlayManager.h"
#include "Engine/OpenGLContext.h"

using namespace vr;

void check_error(int line, EVRInitError error) { if (error != 0) printf("%d: error %s\n", line, VR_GetVRInitErrorAsSymbol(error)); }



int main(int argc, char** argv) {
    EVRInitError error;
    VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);
    OpenGLContext context;

    OverlayManager manager;

    MainOverlay overlay("main");
    overlay.SetWidth(1);
    overlay.Show();

    vr::HmdMatrix34_t transform = {
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f, -2.0f
    };


    manager.RegisterOverlay(&overlay);

    
    while (!context.ShouldClose()) {
        manager.Update();
        manager.Draw();

        overlay.DrawBufferToScreen();
        

        glfwSwapBuffers(context.window);
        glfwPollEvents();

    }
    return 0;
}

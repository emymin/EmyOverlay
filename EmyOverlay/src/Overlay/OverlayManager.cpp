#include "OverlayManager.h"
#include "../Utils/Util.h"

void OverlayManager::Update()
{
    TrackedDeviceIndex_t index = 2;
    vr::TrackedDevicePose_t poses[vr::k_unMaxTrackedDeviceCount];
    vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, 0, poses, vr::k_unMaxTrackedDeviceCount);
    TrackedDevicePose_t controllerPose = poses[2];

    m_Overlays[0]->SetTransform(poses[1].mDeviceToAbsoluteTracking); //TODO each overlay should have its own transform

    Matrix4 mat_controller = poses[2].mDeviceToAbsoluteTracking;
    mat_controller = mat_controller * GetControllerTipMatrix(true);
    Vector3 pos = mat_controller.getTranslation();
    Vector3 forward = { mat_controller[8], mat_controller[9], mat_controller[10] };
    forward *= -1.0f;

    Overlay* closest_overlay = m_Overlays[0];
    IntersectionResult closest_intersection;
    float closest_overlay_distance = INT_MAX;
    for (auto overlay : m_Overlays) {
        IntersectionResult intersection = overlay->GetIntersection(pos, forward);
        if (intersection.distance < closest_overlay_distance) {
            closest_overlay = overlay;
            closest_overlay_distance = intersection.distance;
            closest_intersection = intersection;
        }
    }

    if (VROverlay()->IsDashboardVisible()) {
        vr::VREvent_t event;
        while (vr::VROverlay()->PollNextOverlayEvent(closest_overlay->m_Handle, &event, sizeof(event))) {
            switch (event.eventType) {

            case vr::VREvent_MouseButtonDown: {
                m_isMousePressed = true;
            }
            case vr::VREvent_MouseButtonUp: {
                m_isMousePressed = false;
            }
            }
        }
    }
    else {
        vr::VRControllerState_t state;
        vr::VRSystem()->GetControllerState(2, &state, sizeof(vr::VRControllerState_t));
        bool isTriggerPressed = (state.ulButtonPressed & vr::ButtonMaskFromId(vr::EVRButtonId::k_EButton_SteamVR_Trigger)) != 0;
        m_isMousePressed = isTriggerPressed;
    }



    closest_overlay->m_mousePos = closest_intersection.pos;
    closest_overlay->m_isMousePressed = m_isMousePressed;

}

void OverlayManager::Draw()
{
    for (auto overlay : m_Overlays) {
        overlay->Draw();
    }
}


//Some misc utility functions shared between both applications

#pragma once

#include <algorithm>
#include <string>
#define NOMINMAX
#include <windows.h>
#include <d3d11.h>
#include <ctime>
#include <chrono>
#include <sstream>

#include "Matrices.h"

//String conversion functions using win32's conversion. Return empty string on failure
std::string StringConvertFromUTF16(LPCWSTR str);
std::wstring WStringConvertFromUTF8(const char* str);
std::wstring WStringConvertFromLocalEncoding(const char* str);

//VR helpers
void OffsetTransformFromSelf(vr::HmdMatrix34_t& matrix, float offset_right, float offset_up, float offset_forward);
void OffsetTransformFromSelf(Matrix4& matrix, float offset_right, float offset_up, float offset_forward);
void TransformLookAt(Matrix4& matrix, const Vector3 pos_target, const Vector3 up = {0.0f, 1.0f, 0.0f});
vr::TrackedDeviceIndex_t GetFirstVRTracker();
Matrix4 GetControllerTipMatrix(bool right_hand = true);
float GetTimeNowToPhotons();
void SetConfigForWMR(int& wmr_ignore_vscreens);
vr::EVROverlayError SetSharedOverlayTexture(vr::VROverlayHandle_t ovrl_handle_source, vr::VROverlayHandle_t ovrl_handle_target, ID3D11Resource* device_texture_ref);

template <typename T> T clamp(const T& value, const T& value_min, const T& value_max) 
{
    return std::max(value_min, std::min(value, value_max));
}

template <typename T> int sgn(T val)
{ 
    return (T(0) < val) - (val < T(0));
}

std::string GetTimeString();
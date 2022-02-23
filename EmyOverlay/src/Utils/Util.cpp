#include "Util.h"

#include <d3d11.h>
#include <wrl/client.h>

std::string StringConvertFromUTF16(LPCWSTR str)
{
	std::string stdstr;
	int length_utf8 = WideCharToMultiByte(CP_UTF8, 0, str, -1, nullptr, 0, nullptr, nullptr);

	if (length_utf8 != 0)
	{
		char* str_utf8 = new char[length_utf8];
		
		if (WideCharToMultiByte(CP_UTF8, 0, str, -1, str_utf8, length_utf8, nullptr, nullptr) != 0)
		{
			stdstr = str_utf8;
		}

		delete[] str_utf8;
	}
		
	return stdstr;
}

std::wstring WStringConvertFromUTF8(const char * str)
{
	std::wstring wstr;
	int length_utf16 = MultiByteToWideChar(CP_UTF8, 0, str, -1, nullptr, 0);

	if (length_utf16 != 0)
	{
		WCHAR* str_utf16 = new WCHAR[length_utf16];

		if (MultiByteToWideChar(CP_UTF8, 0, str, -1, str_utf16, length_utf16) != 0)
		{
			wstr = str_utf16;
		}

		delete[] str_utf16;
	}

	return wstr;
}

//This is only needed for std::error_code.message(), thanks to it being in the local ANSI codepage instead of UTF-8
std::wstring WStringConvertFromLocalEncoding(const char* str)
{
    std::wstring wstr;
    int length_utf16 = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);

    if (length_utf16 != 0)
    {
        WCHAR* str_utf16 = new WCHAR[length_utf16];

        if (MultiByteToWideChar(CP_ACP, 0, str, -1, str_utf16, length_utf16) != 0)
        {
            wstr = str_utf16;
        }

        delete[] str_utf16;
    }

    return wstr;
}

void OffsetTransformFromSelf(vr::HmdMatrix34_t& matrix, float offset_right, float offset_up, float offset_forward)
{
	matrix.m[0][3] += offset_right * matrix.m[0][0];
	matrix.m[1][3] += offset_right * matrix.m[1][0];
	matrix.m[2][3] += offset_right * matrix.m[2][0];

	matrix.m[0][3] += offset_up * matrix.m[0][1];
	matrix.m[1][3] += offset_up * matrix.m[1][1];
	matrix.m[2][3] += offset_up * matrix.m[2][1];

	matrix.m[0][3] += offset_forward * matrix.m[0][2];
	matrix.m[1][3] += offset_forward * matrix.m[1][2];
	matrix.m[2][3] += offset_forward * matrix.m[2][2];
}

void OffsetTransformFromSelf(Matrix4& matrix, float offset_right, float offset_up, float offset_forward)
{
    matrix[12] += offset_right * matrix[0];
    matrix[13] += offset_right * matrix[1];
    matrix[14] += offset_right * matrix[2];

    matrix[12] += offset_up * matrix[4];
    matrix[13] += offset_up * matrix[5];
    matrix[14] += offset_up * matrix[6];

    matrix[12] += offset_forward * matrix[8];
    matrix[13] += offset_forward * matrix[9];
    matrix[14] += offset_forward * matrix[10];
}

void TransformLookAt(Matrix4& matrix, const Vector3 pos_target, const Vector3 up)
{
    const Vector3 pos(matrix.getTranslation());

    Vector3 z_axis = pos_target - pos;
    z_axis.normalize();
    Vector3 x_axis = up.cross(z_axis);
    x_axis.normalize();
    Vector3 y_axis = z_axis.cross(x_axis);

    matrix = { x_axis.x, x_axis.y, x_axis.z, 0.0f,
               y_axis.x, y_axis.y, y_axis.z, 0.0f,
               z_axis.x, z_axis.y, z_axis.z, 0.0f,
               pos.x,    pos.y,    pos.z,    1.0f };
}

vr::TrackedDeviceIndex_t GetFirstVRTracker()
{
    //Get the first generic tracker
    for (int i = 0; i < vr::k_unMaxTrackedDeviceCount; ++i)
    {
        if (vr::VRSystem()->GetTrackedDeviceClass(i) == vr::TrackedDeviceClass_GenericTracker)
        {
            return i;
        }
    }

    return vr::k_unTrackedDeviceIndexInvalid;
}

Matrix4 GetControllerTipMatrix(bool right_hand)
{
    char buffer[vr::k_unMaxPropertyStringSize];
    vr::VRInputValueHandle_t input_value = vr::k_ulInvalidInputValueHandle;

    if (right_hand)
    {
        vr::VRSystem()->GetStringTrackedDeviceProperty(vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_RightHand), 
                                                       vr::Prop_RenderModelName_String, buffer, vr::k_unMaxPropertyStringSize);
        vr::VRInput()->GetInputSourceHandle("/user/hand/right", &input_value);
    }
    else
    {
        vr::VRSystem()->GetStringTrackedDeviceProperty(vr::VRSystem()->GetTrackedDeviceIndexForControllerRole(vr::TrackedControllerRole_LeftHand), 
                                                       vr::Prop_RenderModelName_String, buffer, vr::k_unMaxPropertyStringSize);
        vr::VRInput()->GetInputSourceHandle("/user/hand/left", &input_value);
    }

    vr::RenderModel_ControllerMode_State_t controller_state = {0};
    vr::RenderModel_ComponentState_t component_state = {0};

    if (vr::VRRenderModels()->GetComponentStateForDevicePath(buffer, vr::k_pch_Controller_Component_Tip, input_value, &controller_state, &component_state))
    {
        return component_state.mTrackingToComponentLocal;
    }

    return Matrix4();
}

float GetTimeNowToPhotons()
{
    float seconds_since_last_vsync;
    vr::VRSystem()->GetTimeSinceLastVsync(&seconds_since_last_vsync, nullptr);

    const float vsync_to_photons  = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);
    const float display_frequency = vr::VRSystem()->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);

    return (1.0f / display_frequency) - seconds_since_last_vsync + vsync_to_photons;
}

void SetConfigForWMR(int& wmr_ignore_vscreens)
{
    //Check if system is WMR and set WMR-specific default values if needed
    char buffer[vr::k_unMaxPropertyStringSize];
    vr::VRSystem()->GetStringTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_TrackingSystemName_String, buffer, vr::k_unMaxPropertyStringSize);

    bool is_wmr_system = (strcmp(buffer, "holographic") == 0);

    if (is_wmr_system) //Is WMR, enable settings by default
    {
        if (wmr_ignore_vscreens == -1)
        {
            wmr_ignore_vscreens = 1;
        }        
    }
    else //Not a WMR system, set values to -1. -1 settings will not be save to disk so a WMR user's settings is preserved if they switch around HMDs, but the setting is still false
    {
        wmr_ignore_vscreens = -1;
    }
}

vr::EVROverlayError SetSharedOverlayTexture(vr::VROverlayHandle_t ovrl_handle_source, vr::VROverlayHandle_t ovrl_handle_target, ID3D11Resource* device_texture_ref)
{
    //Get overlay texture handle from OpenVR and set it as handle for the target overlay
    ID3D11ShaderResourceView* ovrl_shader_res;
    uint32_t ovrl_width;
    uint32_t ovrl_height;
    uint32_t ovrl_native_format;
    vr::ETextureType ovrl_api_type;
    vr::EColorSpace ovrl_color_space;
    vr::VRTextureBounds_t ovrl_tex_bounds;

    vr::VROverlayError ovrl_error = vr::VROverlayError_None;
    ovrl_error = vr::VROverlay()->GetOverlayTexture(ovrl_handle_source, (void**)&ovrl_shader_res, device_texture_ref, &ovrl_width, &ovrl_height, &ovrl_native_format,
                                                    &ovrl_api_type, &ovrl_color_space, &ovrl_tex_bounds);

    if (ovrl_error == vr::VROverlayError_None)
    {
        {
            Microsoft::WRL::ComPtr<ID3D11Resource> ovrl_tex;
            Microsoft::WRL::ComPtr<IDXGIResource> ovrl_dxgi_resource;
            ovrl_shader_res->GetResource(&ovrl_tex);

            HRESULT hr = ovrl_tex.As(&ovrl_dxgi_resource);

            if (!FAILED(hr))
            {
                HANDLE ovrl_tex_handle = nullptr;
                ovrl_dxgi_resource->GetSharedHandle(&ovrl_tex_handle);

                vr::Texture_t vrtex_target;
                vrtex_target.eType = vr::TextureType_DXGISharedHandle;
                vrtex_target.eColorSpace = vr::ColorSpace_Gamma;
                vrtex_target.handle = ovrl_tex_handle;

                vr::VROverlay()->SetOverlayTexture(ovrl_handle_target, &vrtex_target);
            }
        }

        vr::VROverlay()->ReleaseNativeOverlayHandle(ovrl_handle_source, (void*)ovrl_shader_res);
        ovrl_shader_res = nullptr;
    }

    return ovrl_error;
}

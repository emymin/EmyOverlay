#pragma once
#include <openvr.h>
#include <stdio.h>
#include <string>
#include "Engine/Console.h"
using namespace vr;
class Overlay
{
public:
	Overlay(std::string name) {
		VROverlay()->CreateOverlay(name.c_str(), name.c_str(), &m_Handle);
	}

	void SetImage(std::string path) {
		VROverlay()->SetOverlayFromFile(m_Handle, path.c_str());
	}
	void SetTexture(vr::Texture_t* texture) {
		auto error = VROverlay()->SetOverlayTexture(m_Handle, texture);
		//Console::Log(std::to_string(error));
	}


	void SetWidth(float width) {
		VROverlay()->SetOverlayWidthInMeters(m_Handle,width);
	}
	void Show() {
		vr::EVROverlayError error = VROverlay()->ShowOverlay(m_Handle);
		//Console::Log(std::to_string(error));
	}

	void SetTransform(HmdMatrix34_t& transform) {
		VROverlay()->SetOverlayTransformAbsolute(m_Handle, TrackingUniverseStanding, &transform);
	}

private:
	VROverlayHandle_t m_Handle;
};


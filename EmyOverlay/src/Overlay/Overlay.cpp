#include "Overlay.h"

Overlay::Overlay(std::string name) {
	VROverlay()->CreateOverlay(name.c_str(), name.c_str(), &m_Handle);
	//VROverlay()->CreateDashboardOverlay(name.c_str(), name.c_str(), &m_Handle,&m_thumbnailHandle);
	VROverlay()->SetOverlayInputMethod(m_Handle, vr::VROverlayInputMethod_Mouse);

}

void Overlay::SetImage(std::string path) {
	VROverlay()->SetOverlayFromFile(m_Handle, path.c_str());
}

void Overlay::SetTexture(vr::Texture_t* texture) {
	auto error = VROverlay()->SetOverlayTexture(m_Handle, texture);
}

void Overlay::SetWidth(float width) {
	VROverlay()->SetOverlayWidthInMeters(m_Handle, width);
}

void Overlay::Show() {
	vr::EVROverlayError error = VROverlay()->ShowOverlay(m_Handle);
}

IntersectionResult Overlay::GetIntersection(Vector3 origin, Vector3 direction)
{
	vr::VROverlayIntersectionResults_t results;
	vr::VROverlayIntersectionParams_t params;
	params.eOrigin = vr::TrackingUniverseStanding;
	params.vSource = { origin.x, origin.y, origin.z };
	params.vDirection = { direction.x, direction.y, direction.z };

	bool intersects = vr::VROverlay()->ComputeOverlayIntersection(m_Handle, &params, &results);
	float x = results.vUVs.v[0];
	float y = results.vUVs.v[1];

	IntersectionResult result = {
		intersects,
		Vector2(x,y),
		results.fDistance
	};

	return result;

}

void Overlay::SetTransform(HmdMatrix34_t& transform) {
	VROverlay()->SetOverlayTransformAbsolute(m_Handle, TrackingUniverseStanding, &transform);
}

#pragma once
#include <openvr.h>
#include <stdio.h>
#include <string>
#include "../Utils/Vectors.h"
#include "../Utils/Matrices.h"
using namespace vr;

struct IntersectionResult {
	bool intersects;
	Vector2 pos;
	float distance;
};


class Overlay
{
public:
	Overlay(std::string name);

	void SetImage(std::string path);
	void SetTexture(vr::Texture_t* texture);


	void SetWidth(float width);
	void Show();

	IntersectionResult GetIntersection(Vector3 origin, Vector3 direction);

	void SetTransform(HmdMatrix34_t& transform);

	virtual void Draw() = 0;

private:
	VROverlayHandle_t m_Handle;
	VROverlayHandle_t m_thumbnailHandle;
protected:
	Vector2 m_mousePos;
	bool m_isMousePressed;

	friend class OverlayManager;
};


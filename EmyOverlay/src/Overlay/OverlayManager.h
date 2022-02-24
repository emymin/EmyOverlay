#pragma once
#include "Overlay.h"
#include <vector>
class OverlayManager
{
public:
	void RegisterOverlay(Overlay* overlay) { m_Overlays.push_back(overlay); }
	void Update();
	void Draw();
private:
	std::vector<Overlay*> m_Overlays;
};


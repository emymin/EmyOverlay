#include "Overlay/ImGuiOverlay.h"

class MainOverlay : public ImGuiOverlay {
public:
	MainOverlay() : ImGuiOverlay("Main", 1000, 1000){}
	virtual void OnGui() override
	{
		ImGui::ShowDemoWindow();
		ImGui::Begin("Info");
		ImGui::Text("Cursor Position: %f %f", m_mousePos.x, m_mousePos.y);
		ImGui::End();
	}
};
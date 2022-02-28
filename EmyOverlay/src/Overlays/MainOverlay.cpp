#include "MainOverlay.h"
#include "../Utils/Util.h"


 void MainOverlay::OnGui()
{
	ImGui::Begin("Info");
	ImGui::Text("Cursor Position: %f %f", m_mousePos.x, m_mousePos.y);
	ImGui::End();


	ImGui::Begin("EmyOverlay");
	ImGui::Text(GetTimeString().c_str());
	ImGui::End();
}

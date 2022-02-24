#include "ImGuiOverlay.h"

void ImGuiOverlay::OnDraw()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::GetIO().AddMousePosEvent(m_mousePos.x*m_Width,m_mousePos.y*m_Height);
    ImGui::GetIO().AddMouseButtonEvent(0, m_isMousePressed);

    OnGui();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
#include "../Overlay/ImGuiOverlay.h"

class MainOverlay : public ImGuiOverlay {
public:
	MainOverlay(std::string name) : ImGuiOverlay(name, 1000, 1000){}
	virtual void OnGui() override;
	
};
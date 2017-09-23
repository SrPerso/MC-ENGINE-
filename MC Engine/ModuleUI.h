#ifndef __MODULEIUI_H__
#define __MODULEIUI_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include <string>
#include <vector>
class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();
public:
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	void AddLogToConsole(std::string toAdd);

public://math
	bool intersects = false;

private:// open Windows
	bool openConsoleW;

private:
	std::vector<std::string> consoleTxt;

};









#endif // __ModuleUI_H__
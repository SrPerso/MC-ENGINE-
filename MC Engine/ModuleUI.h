#ifndef __MODULEIUI_H__
#define __MODULEIUI_H__

#include "Module.h"
#include "Globals.h"

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
	bool intersects = false;

};









#endif // __ModuleUI_H__
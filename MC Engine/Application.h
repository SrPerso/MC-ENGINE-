#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"


#include "ModuleGameObjectManager.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"

#include "ModulePhysics3D.h"
#include "ModuleUI.h"

#include "ModuleDataManager.h"
#include "ModuleDataFile.h"
#include "ModuleTexture.h"

#include <list>


class Application
{
public:
	ModuleWindow* window = nullptr;
	ModuleInput* input = nullptr;
	ModuleAudio* audio = nullptr;
	ModuleDataManager* datamanager = nullptr;
	ModuleUI* ui = nullptr;
	ModuleRenderer3D* renderer3D = nullptr;
	ModuleCamera3D* camera = nullptr;
	ModulePhysics3D* physics = nullptr;
	GObjectManager* goManager = nullptr;
	ModuleSceneIntro* scene_intro = nullptr;
	DataJSON* jtsonData = nullptr;
	DataFBX* fbxdata = nullptr;
	ModuleTexture* texture = nullptr;



private:

	Timer	ms_timer;	
	std::list<Module*> list_modules;
	float	dt;
	float lastFPS = 0;
	float lastMs = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	const std::list<Module*>* GetModuleList()const;

public:
	float GetFPS();
	float GetMs();

	std::vector<std::string> consoleTxt;
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};

extern Application* App;
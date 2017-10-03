#pragma once

#include "p2List.h"
#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleSceneIntro.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModulePhysics3D.h"
#include "ModulePlayer.h"
#include "ModuleUI.h"

class Application
{
public:
	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	
	ModuleUI* ui;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModulePhysics3D* physics;
	ModuleSceneIntro* scene_intro;
	ModulePlayer* player;

private:

	Timer	ms_timer;	
	p2List<Module*> list_modules;
	float	dt;
	float lastFPS = 0;
	float lastMs = 0;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

public:
	float GetFPS();
	float GetMs();
private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();
};
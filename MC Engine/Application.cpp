
#pragma once
#include "Application.h"
#include "Globals.h"
#include "parson\parson.h"



Application::Application()
{
	randGen = new math::LCG();

	timeManager = new ModuleTimeManager(this);
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	
	ui = new ModuleUI(this);
	renderer3D = new ModuleRenderer3D(this);

	camera = new ModuleCamera3D(this);
	
	scene_intro = new ModuleSceneIntro(this);
	datamanager = new ModuleDataManager(this);
	goManager = new GObjectManager(this);
	texture = new ModuleTexture(this);
	


	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(timeManager);
	AddModule(window);

	AddModule(camera);
	AddModule(input);
	AddModule(audio);

	// Scenes
	AddModule(goManager);
	AddModule(datamanager);
	AddModule(texture);
	AddModule(scene_intro);
	AddModule(ui);
	// Renderer last!
	AddModule(renderer3D);

	

}

Application::~Application()
{
	for (std::list<Module*>::reverse_iterator item = list_modules.rbegin(); item != list_modules.crend(); ++item)
	{
		if ((*item) != NULL)
		{                     
			delete (*item);
			(*item) = NULL;
		}
	}
		list_modules.clear();
}

bool Application::Init()
{
	bool ret = true;


	// Call Init() in all modules
	std::list<Module*>::iterator item = list_modules.begin();

	while (item != list_modules.end() && ret == true)
	{
		ret = (item._Ptr->_Myval)->Init();
		item++;
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	item = list_modules.begin();

	while(item != list_modules.end() && ret == true)
	{
		ret = (item._Ptr->_Myval)->Start();
		item++;
	}
	
	timeManager->StartGameTime();
	timeManager->StartRealTime();
	ms_timer.Start();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	lastFPS = 1.0f / dt;
	lastMs = (float)ms_timer.Read();

	timeManager->SetFrame(1);
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::iterator item = list_modules.begin();
	
	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}

	item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;
	std::list<Module*>::iterator item = list_modules.begin();

	while(item != list_modules.end())
	{
		ret = (*item)->CleanUp();
		item++;
	}

	delete randGen;

	return ret;
}

const std::list<Module*>* Application::GetModuleList() const
{
	return &list_modules;
}

float Application::GetFPS()
{
	return float(lastFPS);
}

float Application::GetMs()
{
	return float(lastMs);
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}
#include "ModuleTimeManager.h"

ModuleTimeManager::ModuleTimeManager(Application * app, bool start_enabled): Module(app, start_enabled)
{
	name = "ModuleTimeManager";

	Frame_Count = 0;	
	Time_Scale = 0.0f;
	Delta_Time = 0.0f;
	

	Paused = false;
}

ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init()
{
	return true;
}

bool ModuleTimeManager::CleanUp()
{
	return true;
}

void ModuleTimeManager::OnConfiguration()
{
	if (ImGui::CollapsingHeader("Time")) 
	{
		if (ImGui::Button("PLAY")) 
		{
			TimeStatus(true);
		}
		ImGui::SameLine();
		if (ImGui::Button("PAUSE"))
		{
			TimeStatus(false);
		}
		ImGui::SameLine();
		if (ImGui::Button("FRAME"))
		{
			//PlayOneFrame();
		}

		ImGui::Text("Real Time Since Startup: %.2f", Real_Time_Since_Startup);
		ImGui::Text("Game Time: %.2f", Time);
		ImGui::Text("Frame Count %i", Frame_Count);
	}
}

void ModuleTimeManager::TimeStatus(bool status)
{
	if (status == true)
	{
		Time.Continue();
	}
	if (status == false)
	{
		Time.Stop();
		
	}
}

void ModuleTimeManager::SetFrame(int numFrame)
{
	Frame_Count += numFrame;

}

void ModuleTimeManager::StartGameTime()
{
	Time.Start();
}

void ModuleTimeManager::StartRealTime()
{
	Real_Time_Since_Startup.Start();

}

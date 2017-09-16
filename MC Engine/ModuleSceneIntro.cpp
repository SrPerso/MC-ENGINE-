#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleSceneIntro::ModuleSceneIntro(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleSceneIntro::~ModuleSceneIntro()
{}

// Load assets
bool ModuleSceneIntro::Start()
{
	LOG("Loading Intro assets");
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	//App->camera->Move(vec3(1.0f, 1.0f, 0.0f));
	//App->camera->LookAt(vec3(0, 0, 0));

	return ret;
}

// Load assets
bool ModuleSceneIntro::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplSdlGL3_Shutdown();
	
	return true;
}

// PreUpdate
update_status ModuleSceneIntro::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return(UPDATE_CONTINUE);
}

// Update
update_status ModuleSceneIntro::Update(float dt)
{	

	static bool show_test_window = false;

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}
	
	//tests
	ImGui::Text("Probe %i",num);

	if (ImGui::Button("OK")) {
		num = 1;
	}
	

	
	if (ImGui::BeginMainMenuBar())
	{
		
		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Show test window", &show_test_window);
			ImGui::EndMenu();
		}

	
		ImGui::EndMainMenuBar();
	}
	
	ImGui::Render();

	return UPDATE_CONTINUE;
}

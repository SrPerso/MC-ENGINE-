#include "Globals.h"
#include "Application.h"
#include "ModuleSceneIntro.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"


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
	
	

	
	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("Window"))
		{
			ImGui::Checkbox("Show test window", &show_test_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("MathTest")) 
		{
			ImGui::Text("Sphere1 (1,1,0) with 2 radius and Sphere2(2,2,0) with radius 1");

			math::Sphere sphere1(float3(1.0f, 1.0f, 0.0f), 2);
			math::Sphere sphere2(float3(2.0f, 2.0f, 0.0f), 1);

			intersects = sphere1.Intersects(sphere2);


			if (intersects)
			{
				ImGui::Text("True");
			}
			else
			{
				ImGui::Text("False");
			}

			ImGui::Text("Sphere3 (0,0,0) with 1 radius and Sphere4(2,2,0) with radius 1");

			math::Sphere sphere3(float3(0.0f, 0.0f, 0.0f), 1);
			math::Sphere sphere4(float3(2.0f, 2.0f, 0.0f), 1);

			intersects = sphere3.Intersects(sphere4);


			if (intersects)
			{
				ImGui::Text("True");
			}
			else
			{
				ImGui::Text("False");
			}


			ImGui::Text("Sphere5 (1,1,0) with 2 radius and Capsule1(2,2,0) with radius 1");

			math::Sphere sphere5(float3(1.0f, 1.0f, 0.0f), 2);
			math::Capsule capsule1(float3(2.0f, 2.0f, 0.0f), float3(2.0f,3.0f,0.0f),1);

			intersects = sphere5.Intersects(capsule1);


			if (intersects)
			{
				ImGui::Text("True");
			}
			else
			{
				ImGui::Text("False");
			}

			ImGui::Text("Sphere6 (1,1,0) with 2 radius and Capsule2(10,20,0) with radius 1");

			math::Sphere sphere6(float3(1.0f, 1.0f, 0.0f), 2);
			math::Capsule capsule2(float3(10.0f, 20.0f, 0.0f), float3(10.0f, 23.0f, 0.0f), 1);

			intersects = sphere6.Intersects(capsule2);


			if (intersects)
			{
				ImGui::Text("True");
			}
			else
			{
				ImGui::Text("False");
			}
			ImGui::EndMenu();
		}

		

		

		

		





		ImGui::EndMainMenuBar();

	}
	
	ImGui::Render();

	return UPDATE_CONTINUE;
}

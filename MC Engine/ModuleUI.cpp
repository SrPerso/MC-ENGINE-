#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

ModuleUI::ModuleUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleUI::~ModuleUI()
{
}

bool ModuleUI::Start()
{
	bool ret = true;

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);




	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return update_status(UPDATE_CONTINUE);
}

update_status ModuleUI::Update(float dt)
{


	static bool show_test_window = false;


	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}


	if (ImGui::BeginMainMenuBar())
	{
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
			math::Capsule capsule1(float3(2.0f, 2.0f, 0.0f), float3(2.0f, 3.0f, 0.0f), 1);

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
		
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Quit", "ESC"))
					return UPDATE_STOP;			

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Team"))
		{
			ImGui::Checkbox("Team", &show_test_window);
			ImGui::Checkbox("Project", &show_test_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			ImGui::Checkbox("test window", &show_test_window);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{

			ImGui::EndMenu();
		}



		ImGui::EndMainMenuBar();

	}

	ImGui::Render();

	return update_status(UPDATE_CONTINUE);
}

bool ModuleUI::CleanUp()
{
	bool ret = true;
	ImGui_ImplSdlGL3_Shutdown();

	return ret;
}

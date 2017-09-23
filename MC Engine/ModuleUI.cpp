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
	App->ui->AddLogToConsole("Loading UI Engine");
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);

	//Windows ;

	OpenTeamInfoW = false;
	openConsoleW = false;


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
	static bool show_Console_window = false;
	static bool show_Configuration_window = false;
	static bool show_MathTest_window = false;

	if (show_test_window)
	{
		ImGui::ShowTestWindow();
	}


	if (ImGui::BeginMainMenuBar())
	{
	
		
		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Quit", "ESC"))
					return UPDATE_STOP;			

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("View"))
		{
			
			ImGui::Checkbox("test window", &show_test_window);


			if (ImGui::Checkbox("Console", &show_Console_window))
			{
				openConsoleW = !openConsoleW;
				consoleActive = !consoleActive;
			}

			if (ImGui::Checkbox("Configuration", &show_Configuration_window))
			{

			}

			ImGui::EndMenu();
		}



		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Team"))
			{
				OpenTeamInfoW = !OpenTeamInfoW;
				TeamInfoActive = !TeamInfoActive;
			}

			if (ImGui::BeginMenu("About MC Engine..."))
			{					

				if (ImGui::MenuItem("Github repository"))
				{
					ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-", NULL, NULL, SW_SHOWNORMAL);
				}
			
				if (ImGui::MenuItem("Wiki"))
				{
					ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/wiki", NULL, NULL, SW_SHOWNORMAL);
				}
		
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Latest Release"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/issues", NULL, NULL, SW_SHOWNORMAL);
			}

			ImGui::EndMenu();
		}

	

		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Checkbox("MathTest",&show_MathTest_window)) {



			}

		/*	if (ImGui::BeginMenu("MathTest"))
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
			}*/


			ImGui::EndMenu();
		}
			
		ImGui::EndMainMenuBar();
	}




	if (show_MathTest_window)
		ShowMathWindow();

	if (consoleActive)
		ShowConsoleWindow();

	if (TeamInfoActive)
		ShowTeamInfoWindow();



	ImGui::Render();

	return update_status(UPDATE_CONTINUE);
}

bool ModuleUI::CleanUp()
{
	bool ret = true;
	ImGui_ImplSdlGL3_Shutdown();
	App->ui->AddLogToConsole("Unloading UI Engine");
	return ret;
}

IMGUI_API void ModuleUI::ShowConsoleWindow(bool * p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();

		return;
	}


	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

																//ImGui::Text("%s", consoleText);

	if (ImGui::Button("Clear"))
	{
		consoleTxt.clear();
	}

	for (int i = consoleTxt.size() - 1; i >= 0; i--)
	{
		ImGui::Text("%s", consoleTxt[i].c_str());
	}

	ImGui::End();

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowTeamInfoWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Team", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();

		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

						
										//ImGui::Text("%s", consoleText);
	ImGui::Text("This engine is made by:\n ");
	ImGui::Text("\t Marc Samper Tarancon and Carlos Peralta  ");
	ImGui::Text("\n\n\n");
	ImGui::Text("You can found us on github: \n\n");


	if (ImGui::MenuItem(" - Carlos"))
	{
		ShellExecuteA(NULL, "open", "https://github.com/SrPerso/", NULL, NULL, SW_SHOWNORMAL);
	}
	if (ImGui::MenuItem(" - Marc"))
	{
		ShellExecuteA(NULL, "open", "https://github.com/marcsamper", NULL, NULL, SW_SHOWNORMAL);
	}

	ImGui::End();

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowMathWindow(bool * p_open)
{
	// Demonstrate the various window flags. Typically you would just use the default.
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Math Library Tests", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	static bool intersects= false;
	static bool intersectsTrue = false;
	static bool intersectsFalse = false;

	static bool sphere1selected= false;
	static bool sphere2selected = false;
	static bool Capsuleselected = false;


	ImGui::Checkbox("Sphere1", &sphere1selected);
	ImGui::Checkbox("Sphere2", &sphere2selected);
	ImGui::Checkbox("Capsule1", &Capsuleselected);


	if (sphere1selected){
		ImGui::Text("Sphere 1:");
		ImGui::InputInt("Radius", &sphere1.sphereRadius);
		ImGui::InputInt("X", &sphere1.sphereX);
		ImGui::InputInt("Y", &sphere1.sphereY);
		ImGui::InputInt("Z", &sphere1.sphereZ);
	}
	if (sphere2selected) {
		ImGui::Text("Sphere 2:");
		ImGui::InputInt("Radius 2", &sphere2.sphereRadius);
		ImGui::InputInt("X 2", &sphere2.sphereX);
		ImGui::InputInt("Y 2", &sphere2.sphereY);
		ImGui::InputInt("Z 2", &sphere2.sphereZ);
	}

	if (Capsuleselected) {
		ImGui::Text("Capsule 1:");
		ImGui::InputInt("Capsule 1 Radius", &capsule1.capsuleRadius);
		ImGui::InputInt("Bottom X", &capsule1.capsuleBotX);
		ImGui::InputInt("Bottom Y", &capsule1.capsuleBotY);
		ImGui::InputInt("Bottom Z", &capsule1.capsuleBotZ);
		ImGui::InputInt("Top X", &capsule1.capsuleTopX);
		ImGui::InputInt("Top Y", &capsule1.capsuleTopY);
		ImGui::InputInt("Top Z", &capsule1.capsuleTopZ);
	}
	
	if (ImGui::Button("Run Math Test"))
	{
		if (sphere1selected && sphere2selected && Capsuleselected) 
			ImGui::Text("it can be done.. Too many selections");

		else if (!sphere1selected && !sphere2selected || !sphere2selected && !Capsuleselected)
			ImGui::Text("it can be done.. is necesary 2 selections");

		else if (sphere1selected && sphere2selected) 
		{
			math::Sphere sphere1(float3(sphere1.sphereX, sphere1.sphereY, sphere1.sphereZ), sphere1.sphereRadius);
			math::Sphere sphere2(float3(sphere2.sphereX, sphere2.sphereY, sphere2.sphereZ), sphere2.sphereRadius);

			intersects = sphere1.Intersects(sphere2);
		}

		else if (sphere1selected && Capsuleselected) 
		{

			math::Sphere sphere1(float3(sphere1.sphereX, sphere1.sphereY, sphere1.sphereZ), sphere1.sphereRadius);
			math::Capsule capsule1(float3(capsule1.capsuleBotX, capsule1.capsuleBotY, capsule1.capsuleBotZ), float3(capsule1.capsuleTopX, capsule1.capsuleTopY, capsule1.capsuleTopZ), capsule1.capsuleRadius);


			intersects = sphere1.Intersects(capsule1);
		}

		else if (sphere1selected && sphere2selected)
		{

			math::Capsule capsule1(float3(capsule1.capsuleBotX, capsule1.capsuleBotY, capsule1.capsuleBotZ), float3(capsule1.capsuleTopX, capsule1.capsuleTopY, capsule1.capsuleTopZ), capsule1.capsuleRadius);
			math::Sphere sphere2(float3(sphere2.sphereX, sphere2.sphereY, sphere2.sphereZ), sphere2.sphereRadius);

			intersects = capsule1.Intersects(sphere2);
		}

		if (intersects)
		{
			intersectsTrue = true;
			intersectsFalse = false;
		}
		else
		{
			intersectsTrue = false;
			intersectsFalse = true;
		}
	}

	ImGui::Text("Intersection:");

	if (intersectsTrue)
		ImGui::Text("\t True");
		
	if (intersectsFalse)
		ImGui::Text("\t False");
		
	
	if (ImGui::Button("Reset"))
	{
		intersects = false;
		intersectsTrue = false;
		intersectsFalse = false;
		sphere1.~sphereTest();
		sphere2.~sphereTest();
		capsule1.~capsuleTest();
	}


		ImGui::End();
	return IMGUI_API void();
}

void ModuleUI::AddLogToConsole(std::string toAdd)
{
	consoleTxt.push_back(toAdd);

}

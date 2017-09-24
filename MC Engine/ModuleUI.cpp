#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"
#include "SDL/include/SDL_cpuinfo.h"

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
	openTestW = false;
	openMathW = false;
	openConfigurationW = true;
	openConsoleW = false;
	
	windowConfig.brightness = 1.0f;

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
	static bool show_Console_window = true;
	static bool show_Configuration_window = false;
	static bool show_MathTest_window = false;
	static bool show_Config_window = true;
	


	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{

			if (ImGui::MenuItem("Quit", "ESC"))
				return UPDATE_STOP;

			ImGui::EndMenu();
		}


		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo", "Ctrl + Z")) {}
			if (ImGui::MenuItem("Redo", "Ctrl + Y")) {}

			ImGui::Separator;
			if (ImGui::MenuItem("Cut", "Ctrl + X")){}
			if (ImGui::MenuItem("Copy", "Ctrl + C")) {}
			if (ImGui::MenuItem("Paste", "Ctrl + V")) {}
			ImGui::Separator;

			if (ImGui::MenuItem("Preferences"))
			{
			}

			ImGui::EndMenu();
		}

	
		
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("About..")) {
				teamInfoActive = !teamInfoActive;
			}

			if (ImGui::BeginMenu("Project..."))
			{					

				if (ImGui::MenuItem("Github repository"))
				{
					ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-", NULL, NULL, SW_SHOWNORMAL);
				}
			
				if (ImGui::MenuItem("Documentation"))
				{
					ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/wiki", NULL, NULL, SW_SHOWNORMAL);
				}
				if (ImGui::MenuItem("Releases"))
				{
					ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/releases", NULL, NULL, SW_SHOWNORMAL);
				}
				ImGui::EndMenu();
			}
	
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/SrPerso/MC-ENGINE-/issues", NULL, NULL, SW_SHOWNORMAL);
			}

			ImGui::EndMenu();
		}
			
		if (ImGui::BeginMenu("Window"))
		{
			if (ImGui::Checkbox("test window", &show_test_window))
			{
				openTestW = !openTestW;
			
			}

			if (ImGui::MenuItem("Console", "Ctrl + Shift + C")) 
			{
				openConsoleW = !openConsoleW;
				consoleActive = !consoleActive;
			}

			if (ImGui::Checkbox("Configuration", &show_Configuration_window))
			{
				openConfigurationW = !openConfigurationW;
				configActive = !configActive;
			}

			ImGui::Separator;
			ImGui::Checkbox("MathTest", &show_MathTest_window);
			ImGui::EndMenu();
		}
			
		ImGui::EndMainMenuBar();
	}

	if (show_test_window)
		ImGui::ShowTestWindow();
	

	if (openConfigurationW)
		ShowConfigWindow();

	if (show_MathTest_window)
		ShowMathWindow();

	if (show_Console_window)
		ShowConsoleWindow();

	if (teamInfoActive)
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


	window_flags = window_flags << ImGuiWindowFlags_NoMove;

	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();

		return;
	}
	
	ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	//ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels
	
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
	ImGui::Text("\t MC ENGINE ");

	ImGui::Text("This is MC Engine and we will try to create \n a full working engine for the subject Engines \n in te university \n ");

	ImGui::Separator();
	ImGui::Text("imGUI 1.51");
	ImGui::Text("MathGeoLib 1.5");
	ImGui::Text("Bullet 2.x");
	ImGui::Text("SDL 2.0.3");
	ImGui::Separator();
	ImGui::Text("Software licence: Apache License 2.0");
	ImGui::Separator();
	ImGui::Text("\n This engine is made by:\n ");

	if (ImGui::MenuItem(" - Marc Samper Tarancon"))
	{
		ShellExecuteA(NULL, "open", "https://github.com/marcsamper", NULL, NULL, SW_SHOWNORMAL);
	}
	if (ImGui::MenuItem(" - Carlos Peralta Sorolla"))
	{
		ShellExecuteA(NULL, "open", "https://github.com/SrPerso/", NULL, NULL, SW_SHOWNORMAL);
	}



	ImGui::End();

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowConfigWindow(bool * p_open)
{


	if (!ImGui::Begin("Configuration", p_open))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	//ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.65f);    // 2/3 of the space for widget and 1/3 for labels
	ImGui::PushItemWidth(-140);                                 // Right align, keep 140 pixels for labels

	ImGui::Text("Configuration");

	if (ImGui::CollapsingHeader("Timer"))
	{

			//FPS
			if (FPSData.size() >= MAX_FPS_MS_COUNT)
			{
				for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
				{
					FPSData[i] = FPSData[i + 1];
				}
				FPSData[MAX_FPS_MS_COUNT - 1] = App->GetFPS();
			}
			else
			{
				FPSData.push_back(App->GetFPS());
			}

			//MS
			if (MsData.size() >= MAX_FPS_MS_COUNT)
			{
				for (int i = 0; i < MAX_FPS_MS_COUNT - 2; i++)
				{
					MsData[i] = MsData[i + 1];
				}
				MsData[MAX_FPS_MS_COUNT - 1] = App->GetMs();
			}
			else
			{
				MsData.push_back(App->GetMs());
			}


			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", FPSData[FPSData.size() - 1]);
			ImGui::PlotHistogram("##framerate", &FPSData[0], FPSData.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", MsData[MsData.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &MsData[0], MsData.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		
	}

	if (ImGui::CollapsingHeader("Window"))
	{
		if (ImGui::SliderFloat("Brightness", &windowConfig.brightness, 0.0f, 2.0f, "%.2f"))
		{
			App->window->SetBrightness(windowConfig.brightness);
		}
		if (ImGui::SliderInt("Width", &windowConfig.windowWidth, 1, 1920))
		{
			App->window->ResizeWindow(windowConfig.windowWidth, windowConfig.windowHeight);
		}
		if (ImGui::SliderInt("Height", &windowConfig.windowHeight, 1, 1080))
		{
			App->window->ResizeWindow(windowConfig.windowWidth, windowConfig.windowHeight);
		}
		if (ImGui::Checkbox("Fullscreen", &windowConfig.fullscreen))
		{
			App->window->SetFullscreen(windowConfig.fullscreen);
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Borderless", &windowConfig.borderless))
		{
			App->window->SetBorderless(windowConfig.borderless);
		}		
		ImGui::SameLine();
		if (ImGui::Checkbox("Full Desktop", &windowConfig.fullDesktop))
		{
			App->window->SetFullDesktop(windowConfig.fullDesktop);
		}

		if (ImGui::Button("Reset"))
		{
			App->window->ResizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
			App->window->SetBrightness(BRIGHTNESS);
			App->window->SetFullscreen(WIN_FULLSCREEN);
			App->window->SetBorderless(WIN_BORDERLESS);
			App->window->SetFullDesktop(WIN_FULLSCREEN_DESKTOP);
			windowConfig.~WindowSetings();
		}
	}

	if (ImGui::CollapsingHeader("Hardware")) {
		
		SDL_version compiled;
		SDL_GetVersion(&compiled);
		ImGui::Text("SDL Version:");
		ImGui::SameLine();	
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(compiled.major).c_str(), nullptr);
		ImGui::SameLine();	ImGui::Text(".");ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(compiled.minor).c_str(), nullptr);
		ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(compiled.patch).c_str(), nullptr);
		ImGui::Text(" ");
		ImGui::Separator();
	

		ImGui::Text("CPUs:");
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(SDL_GetCPUCount()).c_str(), nullptr);


		ImGui::Text(" System RAM:");
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(SDL_GetSystemRAM()).c_str(), nullptr);
	
		ImGui::Text(" Caps:");
		ImGui::SameLine();
		if(SDL_HasRDTSC())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " RDTSC", nullptr);

		ImGui::SameLine();
		if (SDL_HasMMX())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,MMX", nullptr);

		ImGui::SameLine();
		if (SDL_HasSSE())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,SSE", nullptr);

		ImGui::SameLine();
		if (SDL_HasSSE2())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,SSE2", nullptr);

		if (SDL_HasSSE3())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,SS3", nullptr);

		ImGui::SameLine();
		if (SDL_HasSSE41())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,SS41", nullptr);

		ImGui::SameLine();
		if (SDL_HasSSE42())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,SS42", nullptr);

		ImGui::SameLine();
		if (SDL_HasAVX())
			ImGui::TextColoredV(ImVec4{0,100,0,255 }, " ,AVX",nullptr);
		/*
		ImGui::SameLine();
		if (SDL_HasAVX2())
			ImGui::Text(",AVX2");   　　　　 WHYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY!!!!!!!!!
			*/
		ImGui::SameLine();
		if (SDL_HasAltiVec())
			ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,AltiVec", nullptr);
	





		ImGui::Text("\n");
		ImGui::Separator();
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

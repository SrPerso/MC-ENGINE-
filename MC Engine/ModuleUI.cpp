#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "Primitive.h"
#define GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX 0x9049
#define GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX 0x9048
#pragma comment( lib, "Glew/libx86/glew32.lib" )



ModuleUI::ModuleUI(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "UI";
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

	return ret;
}

update_status ModuleUI::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->window);
	return update_status(UPDATE_CONTINUE);
}

update_status ModuleUI::Update(float dt)
{

	update_status ret = UPDATE_CONTINUE;

	/*  MENU **************************************	*/

	if (ImGui::BeginMainMenuBar())
	{

		if (ImGui::BeginMenu("File"))
		{
			ret = FileMenuBar();
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Edit"))
		{
			EditMenuBar();
			ImGui::EndMenu();
		}
		
		if (ImGui::BeginMenu("Help"))
		{
			HelpMenuBar();
			ImGui::EndMenu();
		}
			
		if (ImGui::BeginMenu("Window"))
		{
			WindowMenuBar();
			ImGui::EndMenu();
		}
			
		ImGui::EndMainMenuBar();
	}


	/*  *Execute Windows**************************************	*/


	if (show_test_window)
		ImGui::ShowTestWindow();
	
	if (show_ImageView_window)
		ShowImageViewWindow();
	
	if (show_Console_window)
		ShowConsoleWindow();
	
	if (show_Configuration_window)
		ShowConfigWindow();

	if (show_TeamInfo_window)
		ShowTeamInfoWindow();
	if (show_Geometry_window)
		ShowGeometryWindow();
	


	ImGui::Render();

	return update_status(ret);
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

	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;


	if (!ImGui::Begin("Console", p_open, window_flags))
	{
		//todo
		ImGui::End();

		return;
	}
                        
	
	const char * test = "aaA";
	ImGui::Text("%s", test);

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
	
	ImGui::PushItemWidth(-140);            						
									
	ImGui::Text("\t MC ENGINE ");

	ImGui::Text("This is MC Engine and we will try to create \n a full working engine for the subject Engines \n in te university \n ");

	ImGui::Separator();
	ImGui::Text("imGUI 1.51");
	ImGui::Text("MathGeoLib 1.5");
	ImGui::Text("Bullet 2.x");
	ImGui::Text("SDL 2.0.3");
	ImGui::Text("Glew 2.1.0");
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


	ImGui::PushItemWidth(-100);                                

	ImGui::Text("Configuration");


	if (ImGui::CollapsingHeader("Aplication"))
		AplicationSetingsC();		

	if (ImGui::CollapsingHeader("Window"))
		WindowSetingsC();

	if (ImGui::CollapsingHeader("Hardware")) {}
		//menu.HardwareConfig();

	if (ImGui::CollapsingHeader("Audio"))
		AudioSetingsC();

	//if (ImGui::CollapsingHeader("Devices"))
	//	DevicesSetingsC();


	ImGui::End();

	return IMGUI_API void();
}
/*
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
}*/

IMGUI_API void ModuleUI::ShowImageViewWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;
	
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("Image View", p_open, window_flags))
	{		

		if (ImGui::Checkbox("LIGHTING", &sb_Lighting)) 
			App->renderer3D->EDglView();		
		
		ImGui::SameLine();
		if (ImGui::Checkbox("COLOR MATERIAL", &sb_Color_Material))
			App->renderer3D->EDglView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("TEXTURE 2D", &sb_Texture_2D)) 
			App->renderer3D->TextureView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("DEPTH TEST", &sb_Depth_Test)) 
			App->renderer3D->EDglView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("CULL FACE", &sb_Cull_Face))
			App->renderer3D->EDglView();

		ImGui::SameLine();
		if (ImGui::Checkbox("WIRE", &sb_Wire_Face))
			App->renderer3D->EDglView();

		ImGui::End();
	}	

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowGeometryWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Geometry", p_open))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-100);

	ImGui::Text("Geometry");

	if (ImGui::CollapsingHeader("Cube"))
	{	
		GeometryMenuCube();	

		if (ImGui::Button("Build Cube") && cubex>0.0f && cubey>0.0f && cubez>0.0f) 
		{
			App->scene_intro->CreateCube(vec3(cubex,cubey,cubez),vec3(cubeposX,cubeposY,cubeposZ));

			cubex = 0;
			cubey = 0;
			cubez = 0;
			cubeposX = 0;
			cubeposY = 0;
			cubeposZ = 0;
		}

	}

	if (ImGui::CollapsingHeader("Sphere"))
	{
		GeometryMenuSphere();

		if (ImGui::Button("Build Sphere") && sphereRad > 0) 
		{
			App->scene_intro->CreateSphere(sphereRad, vec3(sphereposX, sphereposY, sphereposZ));

			sphereRad = 0;
			sphereposX = 0;
			sphereposY = 0;
			sphereposZ = 0;
		}
	}

	if (ImGui::CollapsingHeader("Cylinder")) 
	{

	}
	

	if (ImGui::CollapsingHeader("Plane"))
	{

	}
	

	if (ImGui::CollapsingHeader("Capsule"))
	{

	}

	ImGui::End();
	return IMGUI_API void();
}


//show the logs on Console..................................................
void ModuleUI::AddLogToConsole(std::string toAdd)
{
	consoleTxt.push_back(toAdd);

}

//Config Window.............................................................

void ModuleUI::HardwareSetingsC()
{
	SDL_version compiled;
	SDL_GetVersion(&compiled);
	ImGui::Text("SDL Version:");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(compiled.major).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
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
	if (SDL_HasRDTSC())
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
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,AVX", nullptr);

	ImGui::SameLine();
	if (SDL_HasAltiVec())
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, " ,AltiVec", nullptr);


	ImGui::Text("\n");
	ImGui::Separator();

	ImGui::Text("GPU:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 100, 0, 255), "%s", (const char*)glGetString(GL_RENDERER));

	int VRAM = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_TOTAL_AVAILABLE_MEM_NVX, &VRAM);
	ImGui::Text("VRAM capacity:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 100, 0, 255), "%i", VRAM / 1000);

	int VRAMRemaining = 0;
	glGetIntegerv(GL_GPU_MEM_INFO_CURRENT_AVAILABLE_MEM_NVX, &VRAMRemaining);
	ImGui::Text("VRAM available:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 100, 0, 255), "%i", VRAMRemaining / 1000);

	int VRAMWorking = VRAM - VRAMRemaining;
	ImGui::Text("VRAM Working:");
	ImGui::SameLine();
	ImGui::TextColored(ImVec4(0, 100, 0, 255), "%i", VRAMWorking / 1000);


}

void ModuleUI::WindowSetingsC()
{
	if (ImGui::SliderFloat("Brightness", &WindowSetingsS.brightness, 0.0f, 2.0f, "%.2f"))
	{
		App->window->SetBrightness(WindowSetingsS.brightness);
	}
	if (ImGui::SliderInt("Width", &WindowSetingsS.windowWidth, 1, 1920))
	{
		App->window->ResizeWindow(WindowSetingsS.windowWidth, WindowSetingsS.windowHeight);
	}
	if (ImGui::SliderInt("Height", &WindowSetingsS.windowHeight, 1, 1080))
	{
		App->window->ResizeWindow(WindowSetingsS.windowWidth, WindowSetingsS.windowHeight);
	}
	if (ImGui::Checkbox("Fullscreen", &WindowSetingsS.fullscreen))
	{
		App->window->SetFullscreen(WindowSetingsS.fullscreen);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Borderless", &WindowSetingsS.borderless))
	{
		App->window->SetBorderless(WindowSetingsS.borderless);
	}
	ImGui::SameLine();
	if (ImGui::Checkbox("Full Desktop", &WindowSetingsS.fullDesktop))
	{
		App->window->SetFullDesktop(WindowSetingsS.fullDesktop);
	}

	if (ImGui::Button("Reset"))
	{
		App->window->ResizeWindow(SCREEN_WIDTH, SCREEN_HEIGHT);
		App->window->SetBrightness(BRIGHTNESS);
		App->window->SetFullscreen(WIN_FULLSCREEN);
		App->window->SetBorderless(WIN_BORDERLESS);
		App->window->SetFullDesktop(WIN_FULLSCREEN_DESKTOP);
		WindowSetingsS.~WindowSetings();
	}

}

void ModuleUI::AplicationSetingsC()
{
	static char buf1[64] = ""; ImGui::InputText("App Name", buf1, 64);
	static char buf2[64] = ""; ImGui::InputText("Organization", buf1, 64);

	if (FPSData.size() >= MAX_FPSMS_COUNT)
	{
		for (int i = 0; i < MAX_FPSMS_COUNT - 2; i++)
		{
			FPSData[i] = FPSData[i + 1];
		}
		FPSData[MAX_FPSMS_COUNT - 1] = App->GetFPS();
	}
	else
	{
		FPSData.push_back(App->GetFPS());
	}

	if (MsData.size() >= MAX_FPSMS_COUNT)
	{
		for (int i = 0; i < MAX_FPSMS_COUNT - 2; i++)
		{
			MsData[i] = MsData[i + 1];
		}
		MsData[MAX_FPSMS_COUNT - 1] = App->GetMs();
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

void ModuleUI::AudioSetingsC()
{
	if (ImGui::SliderFloat("Volume", &AudioSetingsS.MasterVolume, 0, 128))
	{
		Mix_Volume(-1, AudioSetingsS.MasterVolume);
	}
	if (ImGui::SliderFloat("FX Volume", &AudioSetingsS.FXVolume, 0, 128))
	{
		//TODO AUDIO
	}
	if (ImGui::SliderFloat("BSO Volume", &AudioSetingsS.BSOVolume, 0, 128))
	{
		Mix_VolumeMusic(AudioSetingsS.MasterVolume);
	}
	if (ImGui::TreeNode("Global"))
	{
		if (ImGui::Button("Mute", { 50,20 })) {

			AudioSetingsS.MasterVolume = 0;
			Mix_Volume(-1, AudioSetingsS.MasterVolume);
		}
		ImGui::SameLine();
		if (ImGui::Button("Play", { 50,20 }))
		{
			App->audio->PlayMusic();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause", { 50,20 }))
		{
			for (int i = 0; i < 5; i++)
				Mix_Pause(i);
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop", { 50,20 }))
		{
			Mix_CloseAudio();
		}
		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Music"))
	{
		if (ImGui::Button("Mute", { 50,20 }))
		{
			Mix_VolumeMusic( 0);
			AudioSetingsS.BSOVolume = 0;
		}
		ImGui::SameLine();
		if (ImGui::Button("Play", { 50,20 }))
		{
			App->audio->PlayMusic();
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause", { 50,20 }))
		{
			Mix_PauseMusic();
		}
		ImGui::SameLine();
		if (ImGui::Button("Resume", { 50,20 }))
		{
			Mix_ResumeMusic();
		}
		
		if (ImGui::Button("Rewind", { 50,20 }))
		{
			Mix_RewindMusic();
		}
		ImGui::SameLine();
		if (ImGui::Button("Stop", { 50,20 }))
		{
			Mix_HaltMusic();
		}
		ImGui::TreePop();
	}
}


//MENU BAR,.................................................................

void ModuleUI::HelpMenuBar()
{
	if (ImGui::MenuItem("About..")) {
		show_TeamInfo_window = !show_TeamInfo_window;
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
}

void ModuleUI::WindowMenuBar()
{
	ImGui::Checkbox("test window", &show_test_window);

	ImGui::Checkbox("Image Views", &show_ImageView_window);
	
	if (ImGui::MenuItem("Console", "Ctrl + Shift + C"))
		show_Console_window = !show_Console_window;
	
	ImGui::Checkbox("Configuration", &show_Configuration_window);
	
	ImGui::Checkbox("Geometry", &show_Geometry_window);
	//ImGui::Checkbox("MathTest", &show_MathTest_window);

}

void ModuleUI::GeometryMenuSphere()
{
	
	if (ImGui::Button("Sphere PosX +"))
	{
		sphereposX += 0.1f;
	}
	ImGui::InputFloat("Sphere PosX", &sphereposX, 0, 0, 2);
	if (ImGui::Button("Sphere PosX -"))
	{
		sphereposX -= 0.1f;
	}
	if (ImGui::Button("Sphere PosY +"))
	{
		sphereposY += 0.1f;
	}
	ImGui::InputFloat("Sphere PosY", &sphereposY, 0, 0, 2);
	if (ImGui::Button("Sphere PosY -"))
	{
		sphereposY -= 0.1f;
	}
	if (ImGui::Button("Sphere PosZ +"))
	{
		sphereposZ += 0.1f;
	}
	ImGui::InputFloat("Sphere PosZ", &sphereposZ, 0, 0, 2);
	if (ImGui::Button("Sphere PosZ -"))
	{
		sphereposZ -= 0.1f;
	}

		 
}

void ModuleUI::GeometryMenuCube()
{
	//CUBEXYZ
	if (ImGui::Button("Cube X +"))
	{
		cubex += 0.1f;
	}
	ImGui::InputFloat("Cube X", &cubex, 0, 0, 2);
	if (ImGui::Button("Cube X -"))
	{
		cubex -= 0.1f;
	}
	if (ImGui::Button("Cube Y +"))
	{
		cubey += 0.1f;
	}
	ImGui::InputFloat("Cube Y", &cubey, 0, 0, 2);
	if (ImGui::Button("Cube Y -"))
	{
		cubey -= 0.1f;
	}
	if (ImGui::Button("Cube Z +"))
	{
		cubez += 0.1f;
	}
	ImGui::InputFloat("Cube Z", &cubez, 0, 0, 2);
	if (ImGui::Button("Cube Z -"))
	{
		cubez -= 0.1f;
	}

	//CUBEPOSXYZ
	if (ImGui::Button("Cube PosX +"))
	{
		cubeposX += 0.1f;
	}
	ImGui::InputFloat("Cube PosX", &cubeposX, 0, 0, 2);
	if (ImGui::Button("Cube PosX -"))
	{
		cubeposX -= 0.1f;
	}
	if (ImGui::Button("Cube PosY +"))
	{
		cubeposY += 0.1f;
	}
	ImGui::InputFloat("Cube PosY", &cubeposY, 0, 0, 2);
	if (ImGui::Button("Cube PosY -"))
	{
		cubeposY -= 0.1f;
	}
	if (ImGui::Button("Cube PosZ +"))
	{
		cubeposZ += 0.1f;
	}
	ImGui::InputFloat("Cube PosZ", &cubeposZ, 0, 0, 2);
	if (ImGui::Button("Cube PosZ -"))
	{
		cubeposZ -= 0.1f;
	}
}

update_status ModuleUI::FileMenuBar()
{
	update_status ret = UPDATE_CONTINUE;

	if (ImGui::MenuItem("Quit", "ESC"))
		return UPDATE_STOP;


	return update_status(ret);
}

void ModuleUI::EditMenuBar()
{
	if (ImGui::MenuItem("Undo", "Ctrl + Z")) {} //conect to IObjects
	if (ImGui::MenuItem("Redo", "Ctrl + Y")) {}

	ImGui::Separator;
	if (ImGui::MenuItem("Cut", "Ctrl + X")) {}
	if (ImGui::MenuItem("Copy", "Ctrl + C")) {}
	if (ImGui::MenuItem("Paste", "Ctrl + V")) {}
	ImGui::Separator;

	if (ImGui::MenuItem("Preferences"))
	{
	}
}

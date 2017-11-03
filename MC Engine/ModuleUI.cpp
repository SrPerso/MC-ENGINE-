#include "Globals.h"
#include "Application.h"
#include "ModuleUI.h"
#include "imGUI\imgui.h"
#include "imGUI\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "Math.h"
#include "SDL/include/SDL_cpuinfo.h"
#include "Primitive.h"
#include "CTexture.h"
#include "CTransformation.h"
#include "CMesh.h"
#include "CCamera.h"
#include "ModuleGameObjectManager.h"
#include "GameObject.h"



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
	LOGUI("-START- Loading UI Engine");

	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->window);
	App->window->SetFullscreen(WindowSetingsS.fullscreen);
	JSON_Value* configValue = json_parse_file("config.json");
	JSON_Object* configObject = json_value_get_object(configValue);
	JSON_Object* data = json_object_dotget_object(configObject, this->name.c_str());

	if (data) 
	{

	show_test_window = json_object_dotget_boolean(data, "show_test_window");
	show_Console_window = json_object_dotget_boolean(data, "show_Console_window");
	show_Configuration_window = json_object_dotget_boolean(data, "show_Configuration_window");
	show_Config_window = json_object_dotget_boolean(data, "show_Config_window");
	show_ImageView_window = json_object_dotget_boolean(data, "show_ImageView_window");
	show_TeamInfo_window = json_object_dotget_boolean(data, "show_TeamInfo_window");
	show_Geometry_window = json_object_dotget_boolean(data, "show_Geometry_window");
	show_Debug_window = json_object_dotget_boolean(data, "show_Debug_window");
	show_Editor_window = json_object_dotget_boolean(data, "show_Editor_window");
	show_Inspector_window = json_object_dotget_boolean(data, "show_Inspector_window");
	debug_active = json_object_dotget_boolean(data, "debug_active");
	debug_Vertex_Normals = json_object_dotget_boolean(data, "debug_Vertex_Normals");
	debug_Box = json_object_dotget_boolean(data, "debug_Box");
	sb_Depth_Test = json_object_dotget_boolean(data, "sb_Depth_Test");
	sb_Cull_Face = json_object_dotget_boolean(data, "sb_Cull_Face");
	sb_Wire_Face = json_object_dotget_boolean(data, "sb_Wire_Face");
	sb_Lighting = json_object_dotget_boolean(data, "sb_Lighting");
	sb_Color_Material = json_object_dotget_boolean(data, "sb_Color_Material");
	sb_Texture_2D = json_object_dotget_boolean(data, "sb_Texture_2D");

	LOGUI("-START- Loaded config.json");

	}

	else 
	{
		debug_active = false;
		debug_Vertex_Normals = true;
		debug_Box = false;

	LOGUI("[ERROR]-START- Cant load config.json");
	}


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
		
		//if (ImGui::BeginMenu("Edit"))
		//{
		//	EditMenuBar();
		//	ImGui::EndMenu();
		//}
		
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

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT &&App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT && App->input->GetKey(SDL_SCANCODE_C) == KEY_DOWN)
	{ 
		if (show_Console_window) 
		{
			AddLogToConsole("Close Console");
		}
		else
		{
			AddLogToConsole("Open Console");
		}


		show_Console_window = !show_Console_window;
	}
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

	if (show_Editor_window)
		ShowEditorWindow();
	
	if (show_Debug_window)
		ShowDebugWindow();

	if (show_Inspector_window)
		ShowInspectorWindow();


	return update_status(ret);
}

bool ModuleUI::CleanUp()
{
	bool ret = true;
	ImGui_ImplSdlGL3_Shutdown();
	LOGUI("-CLEANUP- Unloading UI Engine");

	return ret;
}

IMGUI_API void ModuleUI::ShowConsoleWindow(bool * p_open)
{

	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	
	if (!ImGui::Begin("Console", p_open, window_flags))
	{
			ImGui::End();
	}                      
	
	const char * test = "CONSOLE";
	ImGui::Text("%s", test);
	ImGui::Separator();
	if (ImGui::Button("Clear"))
	{
		App->consoleTxt.clear();
	}

	for (int i = App->consoleTxt.size() - 1; i >= 0; i--)
	{
		ImGui::Text("%s", App->consoleTxt[i].c_str());
	}

	ImGui::End();

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowTeamInfoWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;

	if (!ImGui::Begin("Team", p_open, window_flags))
	{
	
		ImGui::End();
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
	ImGui::Text("assimp-4.0.1");
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

	if (ImGui::CollapsingHeader("Hardware"))
		HardwareSetingsC();

	if (ImGui::CollapsingHeader("Audio"))
		AudioSetingsC();

	if (ImGui::CollapsingHeader("Render"))
		RenderSetings();
	if (ImGui::CollapsingHeader("Input"))
		DevicesSetingsC();
	if (ImGui::CollapsingHeader("Time Configuration"))
		App->timeManager->OnConfiguration();

	ImGui::End();

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowImageViewWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;
	
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("Render Setings", p_open, window_flags))
	{
		ImGui::Text("RENDER SETINGS:");
		ImGui::Separator();

		if (ImGui::Checkbox("LIGHTING", &sb_Lighting)) 
			App->renderer3D->EDglView();		
		
		ImGui::SameLine();
		if (ImGui::Checkbox("COLOR MATERIAL", &sb_Color_Material))
			App->renderer3D->EDglView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("TEXTURE 2D", &sb_Texture_2D)) 
			App->renderer3D->EDglView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("DEPTH TEST", &sb_Depth_Test)) 
			App->renderer3D->EDglView();
		
		ImGui::SameLine();
		if (ImGui::Checkbox("CULL FACE", &sb_Cull_Face))
			App->renderer3D->EDglView();

		ImGui::SameLine();
		if (ImGui::Checkbox("WIRE", &sb_Wire_Face)) 
		{
			App->renderer3D->EDglView();
		}


		ImGui::End();
	}	

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowGeometryWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
//	window_flags |= ImGuiWindowFlags_NoResize;

	if (!ImGui::Begin("Geometry", p_open, window_flags))
	{
		// Early out if the window is collapsed, as an optimization.
		ImGui::End();
		return;
	}

	ImGui::PushItemWidth(-100);

	ImGui::Text("GEOMETRY WINDOW:");
	ImGui::Separator();

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

			App->scene_intro->CreateSphere(sphereRad, vec3(sphereposX, sphereposY, sphereposZ), numStacks, numSides);

			sphereRad = 0;
			sphereposX = 0;
			sphereposY = 0;
			sphereposZ = 0;
			numSides = 0;
			numStacks = 0;
		}
	}

	if (ImGui::CollapsingHeader("Cylinder")) 
	{
		GeometryMenuCylinder();
		if (ImGui::Button("Build Cylinder") && cylinderRad > 0)
		{

			App->scene_intro->CreateCylinder(cylinderRad,cylinderHeight, vec3(cylinderposX, cylinderposY, cylinderposZ));

			cylinderRad = 0;
			cylinderHeight = 0;
			cylinderposX = 0;
			cylinderposY = 0;
			cylinderposZ = 0;
		}
	}
	

	if (ImGui::CollapsingHeader("Cube1"))
	{
		GeometryMenuCube1();

		if (ImGui::Button("Build Cube1"))
		{
			App->scene_intro->CreateCube1(Cube1size, Cube1pos);
			Cube1size.x = 0;
			Cube1size.y = 0;
			Cube1size.z = 0;
			Cube1pos.x = 0;
			Cube1pos.y = 0;
			Cube1pos.z = 0;

		}
		
		
	}
	
	if (ImGui::CollapsingHeader("Cube2"))
	{
		GeometryMenuCube2();

		if (ImGui::Button("Build Cube2"))
		{
			App->scene_intro->CreateCube2(Cube2size, Cube2pos);
			Cube2size.x = 0;
			Cube2size.y = 0;
			Cube2size.z = 0;
			Cube2pos.x = 0;
			Cube2pos.y = 0;
			Cube2pos.z = 0;
		}
	}

	if (ImGui::CollapsingHeader("Capsule"))
	{

	}

	ImGui::End();
	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowDebugWindow(bool * p_open)
{

	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("Debug Window", p_open, window_flags))
	{
		ImGui::Text("DEBUG WINDOW:");
		ImGui::Separator();
		ImGui::Checkbox("Debug ON", &debug_active);
		ImGui::SameLine();
		ImGui::Checkbox("Vertex Normals", &debug_Vertex_Normals);
		ImGui::SameLine();
		ImGui::Checkbox("Eclosing Box", &debug_Box);
	
		ImGui::Text("Triss:");		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(App->scene_intro->sceneDebugInfo.tris).c_str(), nullptr);
		ImGui::SameLine();
		ImGui::Text("Faces:");		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(App->scene_intro->sceneDebugInfo.faces).c_str(), nullptr);
		ImGui::SameLine();
		ImGui::Text("Vertex:");		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(App->scene_intro->sceneDebugInfo.vertex).c_str(), nullptr);
		ImGui::SameLine();
		ImGui::Text("FPS:");		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string((int)App->GetFPS()).c_str(), nullptr);
		ImGui::SameLine();
		ImGui::Text("Ms:");		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string((int)App->GetMs()).c_str(), nullptr);


		ImGui::End();
	}


	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowEditorWindow(bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;

	window_flags |= ImGuiWindowFlags_NoTitleBar;
	//window_flags |= ImGuiWindowFlags_NoMove;
	//window_flags |= ImGuiWindowFlags_NoResize;

	if (ImGui::Begin("Editor", p_open, window_flags))
	{
		ImGui::Text("EDITOR WINDOW:");
		ImGui::Separator();
		App->goManager->GetRoot()->OnEditor();
		ImGui::End();
	}

	return IMGUI_API void();
}

IMGUI_API void ModuleUI::ShowInspectorWindow(Component* component, bool * p_open)
{
	ImGuiWindowFlags window_flags = 0;
	
	window_flags |= ImGuiWindowFlags_NoTitleBar;

	if (ImGui::Begin("Inspector", p_open, window_flags))
	{
		ImGui::Text("INSPECTOR");
		ImGui::Separator();

		
		if (component != nullptr) {
			Component_Type type = component->getType();
		
			switch (type)
			{
			case COMP_TRANSFORMATION:
				component = (CTransformation*)component;
				component->OnInspector();
				break;

			case COMP_TEXTURE:
				component = (CTexture*)component;
				component->OnInspector();
				break;

			case COMP_MESH:
				component = (CMesh*)component;
				component->OnInspector();
				break;

			case COMP_CAMERA:
				component = (CCamera*)component;
				component->OnInspector();
				break;


			}
		}		//App->goManager->GetRoot()->OnInspector();

		ImGui::End();
	}

	return IMGUI_API void();
}


//show the logs on Console..................................................
void ModuleUI::AddLogToConsole(std::string toAdd)
{
	LOGUI(toAdd.c_str());
	consoleTxt.push_back(toAdd);

}

void ModuleUI::AddLogToConsole(std::string toAdd, int dataToAdd)
{

	toAdd.append(std::to_string(dataToAdd));
	consoleTxt.push_back(toAdd);
}
void ModuleUI::AddLogToConsole(const char file[], int line, const char* format, ...)
{
	static char tmp_string[4096];
	static char tmp_string2[4096];
	static va_list  ap;

	va_start(ap, format);
	vsprintf_s(tmp_string, 4096, format, ap);
	va_end(ap);
	sprintf_s(tmp_string2, 4096, "\n%s(%d) : %s", file, line, tmp_string);

	consoleTxt.push_back(tmp_string2);

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

	ImGui::Text("imGUI");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(1).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(51).c_str(), nullptr);

	ImGui::Text("MathGeoLib");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(1).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(5).c_str(), nullptr);

	ImGui::Text("Bullet");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(2).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, "X", nullptr);

	ImGui::Text("Glew");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(2).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(1).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(0).c_str(), nullptr);

	ImGui::Text("Assimp");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(4).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(0).c_str(), nullptr);
	ImGui::SameLine();	ImGui::Text("."); ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, std::to_string(1).c_str(), nullptr);
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
		if(WindowSetingsS.fullscreen)
		App->window->SetFullscreen(WindowSetingsS.fullscreen);
		else
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
	//static char buf1[64] = ""; ImGui::InputText("App Name", buf1, 64);
	//static char buf2[64] = ""; ImGui::InputText("Organization", buf1, 64);

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
		if (ImGui::Button("Mute", { 50,20 }))
		{

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
	if (ImGui::MenuItem("About.."))
	{
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
	//ImGui::Checkbox("test window", &show_test_window);

	ImGui::Checkbox("Image Views", &show_ImageView_window);
	
	ImGui::Checkbox("Debug Mode", &show_Debug_window);
	
	ImGui::Checkbox("Configuration", &show_Configuration_window);
	
	//ImGui::Checkbox("Geometry", &show_Geometry_window);

	//ImGui::Checkbox("MathTest", &show_MathTest_window);

	ImGui::Checkbox("GameObjects Editor", &show_Editor_window);

	ImGui::Checkbox("GameObjects Inspector", &show_Inspector_window);
	if (ImGui::MenuItem("Console", "Ctrl + Shift + C"))
		show_Console_window = !show_Console_window;
}

void ModuleUI::GeometryMenuSphere()
{

	if (ImGui::Button("Sphere Rad +"))
	{
		sphereRad += 0.1f;
	}
	ImGui::InputFloat("Sphere Radius", &sphereRad, 0, 0, 2);
	if (ImGui::Button("Sphere Rad -"))
	{
		sphereRad -= 0.1f;
	}

	if (ImGui::Button("Sphere Sides +"))
	{
		numSides += 1;
	}
	ImGui::InputInt("Sphere PosZ", &numSides, 0, 0, 2);
	if (ImGui::Button("Sphere Sides -"))
	{
		numSides -= 1;
	}

	if (ImGui::Button("Sphere Stacks +"))
	{
		numStacks += 1;
	}
	ImGui::InputInt("Sphere PosZ", &numStacks, 0, 0, 2);
	if (ImGui::Button("Sphere Stacks -"))
	{
		numStacks -= 1;
	}
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

void ModuleUI::GeometryMenuCylinder()
{
	if (ImGui::Button("Cylinder Rad +"))
	{
		cylinderRad += 0.1f;
	}
	ImGui::InputFloat("Cylinder Radius", &cylinderRad, 0, 0, 2);
	if (ImGui::Button("Cylinder Rad -"))
	{
		cylinderRad -= 0.1f;
	}
	if (ImGui::Button("Cylinder Height +"))
	{
		cylinderHeight += 0.1f;
	}
	ImGui::InputFloat("Cylinder Height", &cylinderHeight, 0, 0, 2);
	if (ImGui::Button("Cylinder Height -"))
	{
		cylinderHeight -= 0.1f;
	}

	if (ImGui::Button("Cylinder PosX +"))
	{
		cylinderposX += 0.1f;
	}
	ImGui::InputFloat("Cylinder PosX", &cylinderposX, 0, 0, 2);
	if (ImGui::Button("Cylinder PosX -"))
	{
		cylinderposX -= 0.1f;
	}
	if (ImGui::Button("Sphere PosY +"))
	{
		cylinderposY += 0.1f;
	}
	ImGui::InputFloat("Cylinder PosY", &cylinderposY, 0, 0, 2);
	if (ImGui::Button("Cylinder PosY -"))
	{
		cylinderposY -= 0.1f;
	}
	if (ImGui::Button("Cylinder PosZ +"))
	{
		cylinderposZ += 0.1f;
	}
	ImGui::InputFloat("Cylinder PosZ", &cylinderposZ, 0, 0, 2);
	if (ImGui::Button("Cylinder PosZ -"))
	{
		cylinderposZ -= 0.1f;
	}
}

void ModuleUI::GeometryMenuCube1()
{
	if (ImGui::Button("Size X +")) 
	{
		Cube1size.x += 0.1f;
	}
	ImGui::InputFloat("Cube1 Size X", &Cube1size.x, 0, 0, 2);
	if (ImGui::Button("Size X -"))
	{
		Cube1size.x -= 0.1f;
	}
	if (ImGui::Button("Size Y +"))
	{
		Cube1size.y += 0.1f;
	}
	ImGui::InputFloat("Cube1 Size Y", &Cube1size.y, 0, 0, 2);
	if (ImGui::Button("Size Y -"))
	{
		Cube1size.y -= 0.1f;
	}
	if (ImGui::Button("Size Z +"))
	{
		Cube1size.z += 0.1f;
	}
	ImGui::InputFloat("Cube1 Size Z", &Cube1size.z, 0, 0, 2);
	if (ImGui::Button("Size Z -"))
	{
		Cube1size.z -= 0.1f;
	}

	if (ImGui::Button("Pos X +"))
	{
		Cube1pos.x += 0.1f;
	}
	ImGui::InputFloat("Cube1 Pos X", &Cube1pos.x, 0, 0, 2);
	if (ImGui::Button("Pos X -"))
	{
		Cube1pos.x -= 0.1f;
	}
	if (ImGui::Button("Pos Y +"))
	{
		Cube1pos.y += 0.1f;
	}
	ImGui::InputFloat("Cube1 Pos Y", &Cube1pos.y, 0, 0, 2);
	if (ImGui::Button("Pos Y -"))
	{
		Cube1pos.y -= 0.1f;
	}
	if (ImGui::Button("Pos Z +"))
	{
		Cube1pos.z += 0.1f;
	}
	ImGui::InputFloat("Cube1 Pos Z", &Cube1pos.z, 0, 0, 2);
	if (ImGui::Button("Pos Z -"))
	{
		Cube1pos.z -= 0.1f;
	}
	

}

void ModuleUI::GeometryMenuCube2()
{
	if (ImGui::Button("Size X +"))
	{
		Cube2size.x += 0.1f;
	}
	ImGui::InputFloat("Cube2 Size X", &Cube2size.x, 0, 0, 2);
	if (ImGui::Button("Size X -"))
	{
		Cube2size.x -= 0.1f;
	}
	if (ImGui::Button("Size Y +"))
	{
		Cube2size.y += 0.1f;
	}
	ImGui::InputFloat("Cube2 Size Y", &Cube2size.y, 0, 0, 2);
	if (ImGui::Button("Size Y -"))
	{
		Cube2size.y -= 0.1f;
	}
	if (ImGui::Button("Size Z +"))
	{
		Cube2size.z += 0.1f;
	}
	ImGui::InputFloat("Cube2 Size Z", &Cube2size.z, 0, 0, 2);
	if (ImGui::Button("Size Z -"))
	{
		Cube2size.z -= 0.1f;
	}

	if (ImGui::Button("Pos X +"))
	{
		Cube2pos.x += 0.1f;
	}
	ImGui::InputFloat("Cube2 Pos X", &Cube2pos.x, 0, 0, 2);
	if (ImGui::Button("Pos X -"))
	{
		Cube2pos.x -= 0.1f;
	}
	if (ImGui::Button("Pos Y +"))
	{
		Cube2pos.y += 0.1f;
	}
	ImGui::InputFloat("Cube2 Pos Y", &Cube2pos.y, 0, 0, 2);
	if (ImGui::Button("Pos Y -"))
	{
		Cube2pos.y -= 0.1f;
	}
	if (ImGui::Button("Pos Z +"))
	{
		Cube2pos.z += 0.1f;
	}
	ImGui::InputFloat("Cube2 Pos Z", &Cube2pos.z, 0, 0, 2);
	if (ImGui::Button("Pos Z -"))
	{
		Cube2pos.z -= 0.1f;
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

void ModuleUI::RenderSetings() 
{

	if (ImGui::Checkbox("LIGHTING", &sb_Lighting))
		App->renderer3D->EDglView();

	ImGui::SameLine();
	if (ImGui::Checkbox("COLOR MATERIAL", &sb_Color_Material))
		App->renderer3D->EDglView();


	if (ImGui::Checkbox("TEXTURE 2D", &sb_Texture_2D))
		App->renderer3D->EDglView();

	ImGui::SameLine();
	if (ImGui::Checkbox("DEPTH TEST", &sb_Depth_Test))
		App->renderer3D->EDglView();


	if (ImGui::Checkbox("CULL FACE", &sb_Cull_Face))
		App->renderer3D->EDglView();

	ImGui::SameLine();
	if (ImGui::Checkbox("WIRE", &sb_Wire_Face))
	{
		App->renderer3D->EDglView();
	}
}
void ModuleUI::DevicesSetingsC()
{
	/*//ImGui::Text("Mouse Status:");
	if (App->input->MouseConected())
	{
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, "CONECTED", nullptr);
	}
	else {
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 100,0,0,255 }, "DISCONECTED", nullptr);
	}

	ImGui::Text("Keyboard Status:"); //Not Workings
	if (App->input->KeyBoardConected())
	{
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, "CONECTED", nullptr);
	}
	else {
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 100,0,0,255 }, "DISCONECTED", nullptr);
	}
*/
	ImGui::Text("Screen Status:");
	if (App->input->ScreenConected())
	{
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, "CONECTED", nullptr);
	}
	else{
		ImGui::SameLine();
		ImGui::TextColoredV(ImVec4{ 100,0,0,255 }, "DISCONECTED", nullptr);
	}
	//ImGui::Text("Speakers Status:");  //Not Working
	//if (App->input->SpeakersConected())
	//{
	//	ImGui::SameLine();
	//	ImGui::TextColoredV(ImVec4{ 0,100,0,255 }, "CONECTED", nullptr);
	//}
	//else {
	//	ImGui::SameLine();
	//	ImGui::TextColoredV(ImVec4{ 100,0,0,255 }, "DISCONECTED", nullptr);
	//}

	ImGui::Text("Social life:");
	ImGui::SameLine();
	ImGui::TextColoredV(ImVec4{ 100,0,0,255 }, "DISCONECTED", nullptr);

	ImGui::Text("Mouse Position: [%d, %d]", App->input->GetMouseX(), App->input->GetMouseY());
	ImGui::Text("Mouse Motion: [%d, %d]", App->input->GetMouseXMotion(), App->input->GetMouseYMotion());


}
void ModuleUI::Draw()
{

	ImGui::Render();
}
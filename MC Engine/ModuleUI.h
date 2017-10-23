#ifndef __MODULEIUI_H__
#define __MODULEIUI_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include <string>
#include <vector>
#include "Component.h"
//#include "ModuleAudio.h"
#define MAX_FPSMS_COUNT 80

class UIMenu;
struct AudioSetings;
struct WindowSetings;
class Win32_VideoController;


class ModuleUI : public Module
{
public:
	ModuleUI(Application* app, bool start_enabled = true);
	~ModuleUI();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	bool CleanUp();
	void Draw();

public:	//setings
	void HardwareSetingsC();
	void WindowSetingsC();
	void AplicationSetingsC();
	void AudioSetingsC();
	void RenderSetings();
	void DevicesSetingsC();
public:
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowTeamInfoWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigWindow(bool* p_open = NULL);
//	IMGUI_API void ShowMathWindow(bool* p_open = NULL);
	IMGUI_API void ShowImageViewWindow(bool* p_open = NULL);
	IMGUI_API void ShowGeometryWindow(bool* p_open = NULL);
	IMGUI_API void ShowDebugWindow(bool* p_open = NULL);

	IMGUI_API void ShowEditorWindow(bool* p_open = NULL);
	IMGUI_API void ShowInspectorWindow(Component* component= NULL,bool* p_open = NULL);

	void AddLogToConsole(std::string toAdd);
	void AddLogToConsole(std::string toAdd,int dataToAdd);
	void AddLogToConsole(const char file[], int line, const char* format, ...);

	bool show_Inspector_window = false;
private:
	std::vector<std::string> consoleTxt;
	std::vector<float> FPSData;
	std::vector<float> MsData;

public: //setings structure
	WindowSetings WindowSetingsS;
	AudioSetings AudioSetingsS;

private:
	// engine main menu bar
	update_status FileMenuBar();
	void EditMenuBar();
	void HelpMenuBar();
	void WindowMenuBar();
	void GeometryMenuCube();
	void GeometryMenuSphere();
	void GeometryMenuCylinder();
	void GeometryMenuCube1();
	void GeometryMenuCube2();
	//Execute

//	UIMenu menuu;
private:
	bool show_test_window = false;
	bool show_Console_window = true;
	bool show_Configuration_window = false;
	bool show_Config_window = false;
	bool show_ImageView_window = true;
	bool show_TeamInfo_window = false;
	bool show_Geometry_window = false;
	bool show_Debug_window = true;
	bool show_Editor_window = true;
	

	//Geometry Test
	float cubex = 0, cubey = 0, cubez = 0;
	float cubeposX = 0, cubeposY = 0, cubeposZ = 0;

	float sphereRad = 0;
	float sphereposX = 0, sphereposY = 0, sphereposZ = 0;

	float cylinderRad = 0, cylinderHeight = 0;
	float cylinderposX = 0, cylinderposY = 0, cylinderposZ = 0;

	int numStacks = 0;
	int numSides=0;

	vec3 Cube1size;
	vec3 Cube1pos;

	vec3 Cube2size;
	vec3 Cube2pos;

public: //
	bool sb_Depth_Test = true;
	bool sb_Cull_Face = true;
	bool sb_Wire_Face = false;
	bool sb_Lighting = true;
	bool sb_Color_Material = true;
	bool sb_Texture_2D = true;
	
//debug
	bool debug_active;
	bool debug_Tri_Normals;
	bool debug_Vertex_Normals;
	bool debug_Box;

};

#endif // __ModuleUI_H__
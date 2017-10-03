#ifndef __MODULEIUI_H__
#define __MODULEIUI_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include <string>
#include <vector>
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
	bool Init();
	//style

	void LoadStyle(JSON_Object* data = nullptr);
	void SaveStyle(JSON_Object* data = nullptr);

public:	//setings
	void HardwareSetingsC();
	void WindowSetingsC();
	void AplicationSetingsC();
	void AudioSetingsC();

public:

	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowTeamInfoWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigWindow(bool* p_open = NULL);
//	IMGUI_API void ShowMathWindow(bool* p_open = NULL);
	IMGUI_API void ShowImageViewWindow(bool* p_open = NULL);
	void AddLogToConsole(std::string toAdd);
	IMGUI_API void ShowStyleWindow(bool* p_open = NULL);
	
private:

	std::vector<std::string> consoleTxt;
	std::vector<float> FPSData;
	std::vector<float> MsData;

private: //setings structure

	WindowSetings WindowSetingsS;
	AudioSetings AudioSetingsS;
public:
	ImGuiStyle* uiStyle;
private:// engine main menu bar

	update_status FileMenuBar();
	void EditMenuBar();
	void HelpMenuBar();
	void WindowMenuBar();


//	UIMenu menuu;
private:
	bool show_test_window = false;
	bool show_Console_window = true;
	bool show_Configuration_window = false;
	bool show_Config_window = false;
	bool show_ImageView_window = false;
	bool show_TeamInfo_window = false;
	bool show_Style_window = false;

public: //
	bool sb_Depth_Test = false;
	bool sb_Cull_Face = false;
	bool sb_Wire_Face = false;
	bool sb_Lighting = false;
	bool sb_Color_Material = false;
	bool sb_Texture_2D = false;

};

#endif // __ModuleUI_H__
#ifndef __MODULEIUI_H__
#define __MODULEIUI_H__

#include "Module.h"
#include "Globals.h"
#include "imGUI\imgui.h"
#include <string>
#include <vector>


#define MAX_FPSMS_COUNT 80

template<class TYPE>
struct sphereTest {
	
	sphereTest() {
		sphereRadius = 0;
		sphereX = 0;
		sphereY = 0;
		sphereZ = 0;
	}
	~sphereTest() {
		sphereRadius = 0;
		sphereX = 0;
		sphereY = 0;
		sphereZ = 0;
	}

	TYPE sphereRadius = 0;
	TYPE sphereX = 0;
	TYPE sphereY = 0;
	TYPE sphereZ = 0;

};

template<class TYPE>
struct capsuleTest{

	capsuleTest() {
		capsuleRadius = 0;
		capsuleBotX = 0;
		capsuleBotY = 0;
		capsuleBotZ = 0;
		capsuleTopX = 0;
		capsuleTopY = 0;
		capsuleTopZ = 0;
	}	
	~capsuleTest() {
		capsuleRadius = 0;
		capsuleBotX = 0;
		capsuleBotY = 0;
		capsuleBotZ = 0;
		capsuleTopX = 0;
		capsuleTopY = 0;
		capsuleTopZ = 0;
	}

	TYPE capsuleRadius = 0;
	TYPE capsuleBotX = 0;
	TYPE capsuleBotY = 0;
	TYPE capsuleBotZ = 0;
	TYPE capsuleTopX = 0;
	TYPE capsuleTopY = 0;
	TYPE capsuleTopZ = 0;
};

struct WindowSetings {
	WindowSetings(){
		fullscreen = false;
		fullDesktop = false;
		windowWidth = SCREEN_WIDTH;
		brightness = BRIGHTNESS;
		borderless = WIN_BORDERLESS;
		windowHeight = SCREEN_HEIGHT;
		vsync = VSYNC;
		FPSLimit = 0;
	}
	~WindowSetings() {
		fullscreen = false;
		fullDesktop = false;
		windowWidth = SCREEN_WIDTH;
		brightness = BRIGHTNESS;
		windowHeight = SCREEN_HEIGHT;
		vsync = VSYNC;
		borderless = WIN_BORDERLESS;
		FPSLimit = 0;
	}

	bool fullscreen;
	bool fullDesktop;
	bool borderless;
	int windowWidth;
	int windowHeight;
	float brightness;
	bool vsync;
	int* FPSLimit;
};
 
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

	void HardwareSetingsC();
	void WindowSetingsC();
	void AplicationSetingsC();

public:
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowTeamInfoWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigWindow(bool* p_open = NULL);


	IMGUI_API void ShowMathWindow(bool* p_open = NULL);
	void AddLogToConsole(std::string toAdd);

public://math
	bool intersects = false;

private:

	bool openConsoleW;
	bool openConfigurationW;
	bool openMathW;
	bool openTestW;

private: // Active
	bool teamInfoActive = false;
	bool TimerActive = true;
	bool consoleActive = false;
	bool mathTestActive = false;
	bool configActive = false;
private:
	std::vector<std::string> consoleTxt;
	std::vector<float> FPSData;
	std::vector<float> MsData;


//---------Math
private:
	//future actualitation : Create a object builder
	sphereTest<int> sphere1;
	sphereTest<int> sphere2;
	capsuleTest<int> capsule1;
	WindowSetings windowConfig;
	//Win32_VideoController VC_GPU;
};









#endif // __ModuleUI_H__
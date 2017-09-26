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

struct AudioSetings {

	AudioSetings() {
		MasterVolume = 50;
		FXVolume = 50;
		BSOVolume = 50;
		Padding = 0;
		Mute = false;
	}

	~AudioSetings() {
		MasterVolume = 50;
		FXVolume = 50;
		BSOVolume = 50;
		Padding = 0;
		Mute = false;
	}

	float MasterVolume;
	float FXVolume;
	float BSOVolume;
	float Padding;
	bool Mute;

};

struct MenuBooleans {
public:

// OPEN GL
	bool DepthTest=false;
	bool CullFace = false;
	bool Lighting = false;
	bool ColorMaterial = false;
	bool Texture2D = true;
//Open windows
	bool openConsoleW;
	bool openConfigurationW;
	bool openMathW;
	bool openTestW;
	bool openImageViewW;
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

public:	//setings
	void HardwareSetingsC();
	void WindowSetingsC();
	void AplicationSetingsC();
	void AudioSetingsC();
	void DevicesSetingsC();

public:
	IMGUI_API void ShowConsoleWindow(bool* p_open = NULL);
	IMGUI_API void ShowTeamInfoWindow(bool* p_open = NULL);
	IMGUI_API void ShowConfigWindow(bool* p_open = NULL);
	IMGUI_API void ShowMathWindow(bool* p_open = NULL);
	IMGUI_API void ShowImageViewWindow(bool* p_open = NULL);
	void AddLogToConsole(std::string toAdd);

public://math
	bool intersects = false;
	MenuBooleans MenuBool;
private: // Active
	bool teamInfoActive = false;
	bool TimerActive = true;
	bool consoleActive = false;
	bool mathTestActive = false;
	bool configActive = false;
	bool ImageViewWActive = true;
//menu GL__-

	
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

private: //setings structure
	WindowSetings WindowSetingsS;
	AudioSetings AudioSetingsS;

	//Win32_VideoController VC_GPU;
};

#endif // __ModuleUI_H__
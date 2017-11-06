#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

struct WindowSetings {

	WindowSetings() {}
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

	bool fullscreen = false;
	bool fullDesktop = false;
	bool borderless = WIN_BORDERLESS;
	int windowWidth = SCREEN_WIDTH;
	int windowHeight = SCREEN_HEIGHT;
	float brightness = BRIGHTNESS;
	bool vsync = VSYNC;
	int* FPSLimit = 0;
};


class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();
	bool Init();
	bool CleanUp();
	void SetTitle(const char* title);


public:

	void ResizeWindow(int width, int height);
	void SetFullscreen(bool fullscreen);
	void SetBrightness(float value);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fullDesktop);
	void GetWidth(int& width) ;
	void GetHeight(int& height) ;
	

public:
	SDL_Window* window;
	SDL_Surface* screen_surface;

private:
	int width;
	int height;
	bool fullscreen;
	
};

#endif // __ModuleWindow_H__
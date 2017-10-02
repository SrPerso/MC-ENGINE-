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
	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);
	void SetTitle(const char* title);


public:
	SDL_Window* GetWindow() const;
	void ResizeWindow(int width, int height);
	void SetFullscreen(bool fullscreen);
	void SetBrightness(float value);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fullDesktop);

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	int width;
	int height;
	bool fullscreen;
};

#endif // __ModuleWindow_H__
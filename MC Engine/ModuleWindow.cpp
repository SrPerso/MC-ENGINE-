#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "parson\parson.h"


ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;
	name = "window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	LOG("Init SDL window & surface");
LOGUI("-START- Init SDL window & surface");
	bool ret = true;

	JSON_Value* configValue = json_parse_file("config.json");
	JSON_Object* configObject = json_value_get_object(configValue);
	JSON_Object* data = json_object_dotget_object(configObject, this->name.c_str());

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		LOGUI("[ERROR]- SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());

		ret = false;
	}
	
	else 
	{
		if (configObject == nullptr) 
		{

			//Create window
			width = SCREEN_WIDTH * SCREEN_SIZE;
			height = SCREEN_HEIGHT * SCREEN_SIZE;
			Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			if (WIN_FULLSCREEN == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN;
			}

			if (WIN_RESIZABLE == true)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}

			if (WIN_BORDERLESS == true)
			{
				flags |= SDL_WINDOW_BORDERLESS;
			}

			if (WIN_FULLSCREEN_DESKTOP == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
				
			}

			window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		}

		else 
		{

			width = json_object_dotget_number(data, "width")*SCREEN_SIZE;
			height = json_object_dotget_number(data, "height")*SCREEN_SIZE;
			App->ui->WindowSetingsS.fullscreen = json_object_dotget_boolean(data, "fullscreen");
			SetFullscreen(json_object_dotget_boolean(data, "fullscreen"));
			SetBrightness(json_object_dotget_number(data, "brightness"));

			App->ui->WindowSetingsS.fullDesktop = json_object_dotget_boolean(data, "fullDesktop");
			SetFullDesktop(json_object_dotget_boolean(data, "fullDesktop"));

			App->ui->WindowSetingsS.borderless = json_object_dotget_boolean(data, "borderless");
			SetBorderless(json_object_dotget_boolean(data, "borderless"));


			Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

			//Use OpenGL 2.1
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
			SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

			if (WIN_FULLSCREEN == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN;
			}

			if (WIN_RESIZABLE == true)
			{
				flags |= SDL_WINDOW_RESIZABLE;
			}

			if (WIN_BORDERLESS == true)
			{
				flags |= SDL_WINDOW_BORDERLESS;
			}

			if (WIN_FULLSCREEN_DESKTOP == true)
			{
				flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
			}

			window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);


		}
		if (window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			LOGUI("[ERROR]-Window could not be created! SDL_Error: %s\n", SDL_GetError());

			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}
	
	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	LOGUI("Destroying SDL window and quitting all SDL systems");
	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

void ModuleWindow::ResizeWindow(int width, int height)
{
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	Uint32 flags;
	if (fullscreen == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetBrightness(float value)
{
	SDL_SetWindowBrightness(window, value);
}

void ModuleWindow::SetBorderless(bool borderless)
{
	Uint32 flags;

	if (borderless == true)
	{
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

void ModuleWindow::SetFullDesktop(bool fullDesktop)
{
	Uint32 flags;
	if (fullDesktop == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	SDL_SetWindowFullscreen(window, flags);
}

int ModuleWindow::GetWidth() const
{
	return this->width;
}

int ModuleWindow::GetHeight() const
{
	return this->height;
}




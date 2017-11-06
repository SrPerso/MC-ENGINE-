#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "Brofiler\Brofiler.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
	name = "input";

}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("-START- Init SDL input event system");
LOGUI("-START- Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
	LOGUI("[ERROR]- SDL_EVENTS could not initialize! ");
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{	
	
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	char* fileDir = nullptr;
	SDL_Event e;
	uint size;

	while(SDL_PollEvent(&e))
	{
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:

				fileDir = e.drop.file;

				LOG("%s dropped on window.", fileDir);

				size = strlen(fileDir);

				if (strcmp(&fileDir[size - 4], ".fbx") == 0 || strcmp(&fileDir[size - 4], ".FBX") == 0)
				{
					App->datamanager->ImportGameObject(fileDir,App->goManager->GetRoot());
					//App->fbxdata->LoadMesh(fileDir);
				LOGUI("(FBX) Droped ");
				}
				else if (strcmp(&fileDir[size - 4], ".png") == 0 || strcmp(&fileDir[size - 4], ".PNG") == 0)
				{
					App->texture->AttatchTexture(App->texture->LoadTexture(fileDir));
				LOGUI("(PNG)- Droped ");
				}

				else if (strcmp(&fileDir[size - 4], ".tga") == 0)
				{
					App->texture->AttatchTexture(App->texture->LoadTexture(fileDir));
					LOGUI("(TGA)- Droped ");
				}
				
				else if (strcmp(&fileDir[size - 4], ".json") == 0)
				{
				//	App->jtsonData;
					LOGUI("(JTSON)- Droped ");
				}

				else 
				{
					LOG("File unknown");
				LOGUI("[File unknown] ");
				}
				SDL_free(fileDir);    // Free dropped_filedir memory
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break;
	


		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
LOGUI("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);

	return true;
}

bool ModuleInput::KeyBoardConected()
{
	bool ret = true;
	return ret;
}

bool ModuleInput::SpeakersConected()
{
	bool ret = true;
	ret = speakersConected;
	return ret;
}

bool ModuleInput::ScreenConected()
{
	bool ret = true;
	return ret;
}

bool ModuleInput::HeadphonesConected()
{
	bool ret = true;
	return ret;
}

bool ModuleInput::MouseConected()
{
	bool ret = true;
	return ret;
}

void ModuleInput::SpeakersConectedDisable()
{
	speakersConected = false;
}

float ModuleInput::GetNormalized_x() 
{
	int w;
	w = App->window->GetWidth();


	normalized_x = (float)(mouse_x / (float)w);
	return normalized_x;
}

float ModuleInput::GetNormalized_y() 
{
	int h;

	h = App->window->GetHeight();


	normalized_y = (float)(mouse_y / (float)h);

	return normalized_y;
}

void ModuleInput::NormalizeMouse()
{	

}

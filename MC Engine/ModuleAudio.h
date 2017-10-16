#ifndef __ModuleAudio_H__
#define __ModuleAudio_H__

#include "Application.h"
#include "Module.h"
#include "SDL_mixer\include\SDL_mixer.h"

#define DEFAULT_MUSIC_FADE_TIME 2.0f

struct AudioSetings
{

	AudioSetings(){}

	~AudioSetings(){
		MasterVolume = 50;
		FXVolume = 50;
		BSOVolume = 50;
		Padding = 0;
		Mute = false;
	}

	float MasterVolume = 50;
	float FXVolume = 50;
	float BSOVolume = 50;
	float Padding = 0;
	bool Mute = false;

};

class ModuleAudio : public Module
{
public:

	ModuleAudio(Application* app, bool start_enabled = true);
	~ModuleAudio();

	bool Init();
	bool CleanUp();

	// Play a music file
	bool PlayMusic(const char* path, float fade_time = DEFAULT_MUSIC_FADE_TIME);
	void PlayMusic();
	// Load a WAV in memory
	unsigned int LoadFx(const char* path);

	// Play a previously loaded WAV
	bool PlayFx(unsigned int fx, int repeat = 0);

private:
	Mix_Music*			music;
	p2List<Mix_Chunk*>	fx;

};

#endif // __ModuleAudio_H__
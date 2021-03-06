#ifndef __ModuleTimeManager_H__
#define __ModuleTimeManager_H__

#include "Timer.h"
#include "Module.h"



class ModuleTimeManager : public Module
{
public:

	ModuleTimeManager(Application* app, bool start_enabled = true);
	~ModuleTimeManager();

	bool Init();
	bool CleanUp();

	void OnConfiguration();
	void TimeStatus(bool status);
	void SetFrame(int numFrame);
	void StartGameTime();
	void StartRealTime();
	float ShowGameTime();
	float ShowRealTimeSinceStartup();

private:

	int Frame_Count;
	Timer Time; //seconds since game start(Game Clock)
	float Time_Scale;//scale at wich time is passing (Game Clock)
	float Delta_Time;//last frame expressed in seconds (Game Clock)
	Timer Real_Time_Since_Startup;//seconds since game start (Real Time Clock)
	float Real_Time_Delta_Time;// last frame time expressed in seconds (Real Time Clock)

	bool Paused;


};

#endif // __ModuleAudio_H__
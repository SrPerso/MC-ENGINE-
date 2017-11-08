#ifndef __Module_GAMEOBJECTMANAGER_H__
#define __Module_GAMEOBJECTMANAGER_H__

#include "Globals.h"
#include "Module.h"

class GameObject;
class DataJSON;
class GObjectManager : public Module
{
public:

	GObjectManager(Application* app, bool start_enabled = true);
	~GObjectManager();

	bool Init();
	bool Start();

	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

	GameObject* GetRoot()const;
	void deleteGameObject(GameObject* GObject);

	//Save && load scene

	void SaveScene(const char*fileName);
	void LoadScene(const char*fileName);

public:

	GameObject* root = nullptr;

};

#endif
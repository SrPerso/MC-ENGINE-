#include "ModuleGameObjectManager.h"
#include "module.h"
#include "Application.h"

GObjectManager::GObjectManager(Application * app, bool start_enabled):Module(app, start_enabled)
{
	LOG("Creating ROOT");
	root = new GameObject(nullptr);
	root->SetName("SceneRoot");

	name = "Game Object Manager";
}

GObjectManager::~GObjectManager()
{
	if (root != nullptr)
	delete[] root;

}

bool GObjectManager::Init()
{
	bool ret = true;
	return ret;
}

bool GObjectManager::Start()
{
	bool ret = true;
	return ret;
}

update_status GObjectManager::PreUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;


	return update_status(ret);
}

update_status GObjectManager::Update(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return update_status(ret);
}

update_status GObjectManager::PostUpdate(float dt)
{
	update_status ret = UPDATE_CONTINUE;

	return update_status(ret);
}

bool GObjectManager::CleanUp()
{
	bool ret = true;

	return ret;
}

GameObject * GObjectManager::GetRoot() const
{
	return root;
}

void GObjectManager::deleteGameObject(GameObject * GObject)
{
	if (GObject != nullptr)
	{
		if (GObject->GetParent())
			GObject->GetParent()->DeleteChild(GObject);
		GObject->DeleteChilds();
	}
}

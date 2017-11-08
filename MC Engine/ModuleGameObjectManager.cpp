#include "ModuleGameObjectManager.h"
#include "Application.h"
#include "GameObject.h"

#include <fstream>

GObjectManager::GObjectManager(Application * app, bool start_enabled):Module(app, start_enabled)
{
	LOG("Creating ROOT");
	
	name = "Game Object Manager";
}

GObjectManager::~GObjectManager()
{
	if (root != nullptr)
		root->cleanUp();

}

bool GObjectManager::Init()
{
	bool ret = true;

	root = new GameObject(nullptr);
	root->SetName("SceneRoot");
	root->SetGOUID(0); // root must have uid 0

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


	if (root->IsEnable() == true)
	{
		root->Update(dt);
	}

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

void GObjectManager::SaveScene(const char * fileName)
{
	DataJSON dataToSave;
	dataToSave.AddArray("Scene_GO");

	App->goManager->root->OnSerialize(dataToSave);

	char* buffer = nullptr;
	uint fileSize = dataToSave.buffSizeSaver(&buffer, "Scene file save");

	LOGUI("-------------------------------------------");
	LOGUI("[SERIALIZE]- Creating file to save  {%s}", fileName);

	std::string path;
	path = "Assets/";
	path.append(fileName);
	path.append(".MCscene");

	std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::binary);
	file.write(buffer, fileSize);
	file.close();

	RELEASE_ARRAY(buffer);
}

void GObjectManager::LoadScene(const char * fileName)
{
	LOGUI("-------------------------------------------");

	
	std::string path;
	path = "Assets/";
	path.append(fileName);
	path.append(".MCscene");


	DataJSON dataToLoad(path.c_str());

	if (dataToLoad.CanLoad() != true)
	{
		LOGUI("[ERROR]-Loading scene. Can't load file {%s}", fileName);
		return;
	}

	App->scene_intro->sceneSelected = nullptr;

	// cleaning scene


	GetRoot()->DeleteChilds();

	uint size = dataToLoad.GetArrayLenght("Scene_GO");

	for (int i = 0; i <size; i++)
	{
		GameObject*  temp = new GameObject(1);

		DataJSON deserialize = dataToLoad.GetArray("Scene_GO", i);

		temp->OnDeserialize(deserialize);

	}

	LOGUI("[DESERIALIZED]- deserialize {%s}", fileName);
}

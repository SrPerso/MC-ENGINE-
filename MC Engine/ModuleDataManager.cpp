#include "ModuleDataManager.h"
#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "GameObject.h"
#include "DContainer.h"

#include "IMesh.h"
#include <fstream>
#include "CTransformation.h"

ModuleDataManager::ModuleDataManager(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	name = "Module Data Manager"; //this module is going to import all Data of the components. and future scene loader

	importerMesh = new ImporterMesh();
	importerTexture = new ImporterTexture();

}


ModuleDataManager::~ModuleDataManager()
{
}

bool ModuleDataManager::Init()
{
	CreateDir("Library");
	CreateDir("Library/Mesh");
	CreateDir("Library/Material");

	return true;
}

bool ModuleDataManager::CleanUp()
{
	delete importerMesh;
	delete importerTransformations;
	delete importerTexture;

	return true;
}

GameObject* ModuleDataManager::ImportGameObject(std::string path, GameObject * parent)
{

	GameObject* newObject = parent->CreateChild();

	int length = strlen(path.c_str());
	std::string namePath = path;

	int i = namePath.find_last_of("\\");

	if (length > 0 && i > 0)
	{
		char* temp = new char[length - i];
		namePath.copy(temp, length - i, i);
		newObject->SetName(temp);

		delete[] temp;
		temp = nullptr;
	}

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		LOGUI("[OK]- Scene %s loaded succesfully", path);

		aiNode* node = scene->mRootNode;
		newObject->SetName(node->mName.C_Str());

		if (scene != nullptr && scene->HasMeshes())
		{
			LOG("Loading meshes");

			ImportGameObject(path, newObject, scene, node); //std::string path, GameObject * parent, const aiScene * scene, aiNode * node)
			
			aiReleaseImport(scene);
		}
		else
		{
			LOGUI("[Error]- Error importing scene %s", path);
			return nullptr;
		}
	}

	return newObject;
}


bool ModuleDataManager::ImportGameObject(std::string path, GameObject * parent, const aiScene * scene, aiNode * node)
{
	bool ret = true;

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];

		if (newMesh != nullptr)
		{
	
			GameObject * GameObjectSon = new GameObject(parent);

			GameObjectSon->SetName(node->mName.C_Str());
			GameObjectSon->SetGOID(App->randGen->Int());

			GameObjectSon->CreateComponent(COMP_TRANSFORMATION, -1, (DTransformation*)importerTransformations->ImportTrans(node, GameObjectSon, GameObjectSon->GetGOId()));

			DMesh* MeshtoCreate = (DMesh*)importerMesh->ImportMesh(newMesh, GameObjectSon, GameObjectSon->GetGOUId());
			GameObjectSon->CreateComponent(COMP_MESH, -1, MeshtoCreate);

			aiMaterial* newMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
			GameObjectSon->CreateComponent(COMP_TEXTURE, -1, (DTexture*)importerTexture->ImportTexture(newMaterial, path.c_str()));

		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
		ImportGameObject(path, parent, scene, node->mChildren[i]);

	return ret;

}

void ModuleDataManager::CreateDir(const char * dirName)
{
	if (CreateDirectory(dirName, NULL))
	{
		LOGUI("[OK]-Created Dir %s", dirName);
	}
	else
		LOGUI("[ERROR]- Cant create Dir %s", dirName);
}


void ModuleDataManager::SaveAllData()const
{
	//std::vector<const void*>* returned = nullptr;
	App->goManager->GetRoot()->SaveData();	
}

void ModuleDataManager::SaveData(const void * buff,DType type, uint id)const
{
	switch (type)
	{
	case D_UNKNOWN:
		LOGUI("[ERROR]-trying to save a unknown data");
		break;

	case D_MESH:
		importerMesh->Save(buff,nullptr,id); //(const void * buffer, const char * saverFile, uint id
		LOGUI("[OK]-MeshData %i saved", id);
		break;

	case D_MATERIAL:
	
		//LOGUI("[OK]-MaterialData %i saved", id);
		LOGUI("[ON CONSTRUCTION]-MaterialData %i saved", id);
		break;

	case D_CAMERA:
		break;

	default:
		break;
	}
}

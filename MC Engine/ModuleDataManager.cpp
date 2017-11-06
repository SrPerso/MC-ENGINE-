#include "ModuleDataManager.h"
#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "GameObject.h"
#include "DContainer.h"

#include "IMesh.h"

#include "CTransformation.h"

ModuleDataManager::ModuleDataManager(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	name = "Module Data Manager"; //this module is going to import all Data of the components. and future scene loader

	importerMesh = new ImporterMesh();
}


ModuleDataManager::~ModuleDataManager()
{
}

GameObject* ModuleDataManager::ImportGameObject(std::string path, GameObject * parent)
{

	GameObject* newObject = parent->CreateChild();

	int length = strlen(path.c_str());
	std::string namePath = path;

	int i = namePath.find_last_of("\\");

	if (length > 0 && i > 0)
	{
		char* testM = new char[length - i];
		namePath.copy(testM, length - i, i);
		newObject->SetName(testM);

		delete[] testM;
		testM = nullptr;
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
			newObject = ImportGameObject(path, newObject, scene, node);
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

GameObject * ModuleDataManager::ImportGameObject(std::string path, GameObject*parent,const aiScene* scene,aiNode* node )
{	
	GameObject* newObject = parent->CreateChild();

	newObject->SetGOID(App->randGen->Int());

			for (int i = 0; i < node->mNumMeshes; i++)
			{
				 
				aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];
				
				GameObject * GameObjectSon;
				
				if (node->mNumMeshes > 1 && i == 1 )
				{
					GameObjectSon = new GameObject(newObject);		
					GameObjectSon->SetGOID(App->randGen->Int());
				}
				else
				{
					GameObjectSon = newObject;
				}				
				
				GameObjectSon->CreateComponent(COMP_TRANSFORMATION, (DTransformation*)importerTransformations->ImportTrans(node, GameObjectSon, GameObjectSon->GetGOId()));

				DMesh* MeshtoCreate = (DMesh*)importerMesh->ImportMesh(newMesh, GameObjectSon, GameObjectSon->GetGOId());
				GameObjectSon->CreateComponent(COMP_MESH, MeshtoCreate);

				aiMaterial* newMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
				GameObjectSon->CreateComponent(COMP_TEXTURE, (DTexture*)importerTexture->ImportTexture(newMaterial, path.c_str()));        
			}		

			for (int i = 0; i < node->mNumChildren; ++i)
				newObject = ImportGameObject(path, parent, scene ,node->mChildren[i]);

			return newObject;
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

	case D_TEXTURE:
		importerTexture->Save(buff, nullptr, id);  //(const void * buffer, const char * saverFile, uint id
		LOGUI("[OK]-TextureData %i saved", id);
		break;

	case D_TRANSFORMATION:
	//	LOGUI("[OK]-Transformation Data %i saved", id);
		LOGUI("[ON CONSTRUCTION]-Transformation Data %i saved", id);
		break;

	case D_CAMERA:
		break;

	default:
		break;
	}
}

void ModuleDataManager::LoadAllData()
{
	App->goManager->GetRoot()->LoadData();

}

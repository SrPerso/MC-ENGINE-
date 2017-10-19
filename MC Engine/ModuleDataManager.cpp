#include "ModuleDataManager.h"
#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "GameObject.h"
#include "DContainer.h"

#include "CTransformation.h"

ModuleDataManager::ModuleDataManager(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	name = "Moudle Data Manager"; //this module is going to import all Data of the components. and future scene loader
}

ModuleDataManager::~ModuleDataManager()
{
}

GameObject * ModuleDataManager::ImportGameObject(std::string path, GameObject*parent)
{
	GameObject* newObject = parent->CreateChild();
	
	//int length = strlen(path.c_str());
	//std::string namePath = path;

	//int i = namePath.find_last_of("\\");

	//if (length > 0 && i > 0)
	//{
	//	char* testM = new char[length - i];
	//	namePath.copy(testM, length - i, i);
	//	newObject->SetName(testM);

	//	delete[] testM;
	//	testM = nullptr;
	//}

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		aiNode* node = scene->mRootNode;

		LOGUI("[OK]- Scene %s loaded succesfully", path);

		newObject->CreateComponent(COMP_TRANSFORMATION, (DTransformation*)importerMesh->ImportTrans(node)); //global objeto total

		if (scene != nullptr && scene->HasMeshes())
		{
			LOG("Loading meshes");

			for (int i = 0; i < scene->mNumMeshes; i++)
			{

				aiMesh* newMesh = scene->mMeshes[i];
				
				GameObject * GameObjectSon;
				
				if (scene->mNumMeshes > 1)
				{

					GameObjectSon = new GameObject();
					newObject->AddChild(GameObjectSon);
				}
				else
				{
					GameObjectSon = newObject;
				}				

				GameObjectSon->CreateComponent(COMP_MESH, (DMesh*)importerMesh->ImportMesh(newMesh));
			
				std::string fullPath = "Assets/";
				fullPath.append(path);

				aiMaterial* newMaterial = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
				GameObjectSon->CreateComponent(COMP_TEXTURE, (DTexture*)importerTexture->ImportTexture(newMaterial, fullPath.c_str()));

			}	

			aiReleaseImport(scene);

			return newObject;
		}
		else
		{
			LOGUI("[Error]- Error importing scene %s", path);
			return nullptr;
		}

		for (int i = 0; i <= node->mNumChildren; ++i)
			ImportGameObject(path,newObject);


	}

}

void ModuleDataManager::LoadAllData()
{
	//calling all load importers functions
}

void ModuleDataManager::SaveAllData()
{
	//calling all save importers functions
}

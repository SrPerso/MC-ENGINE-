#include "ModuleDataManager.h"
#include "Application.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

#include "GameObject.h"
#include "DContainer.h"
#include "DMesh.h"
#include "CMesh.h"

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

bool ModuleDataManager::LoadInMemory()
{
	return true;
}

bool ModuleDataManager::UnloadFromMemory()
{
	return true;
}

bool ModuleDataManager::CleanUp()
{
	//for (std::map<int, DContainer*>::iterator it = dContainerMap.begin(); it != dContainerMap.end(); ++it)
	//{
	//	delete (*it).second;
	//	(*it).second = nullptr;
	//}

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

	int size = namePath.find_last_of("\\");

	if (length > 0 && size > 0)
	{
		char* temp = new char[length - size];
		namePath.copy(temp, length - size, size);
		newObject->SetName(temp);

		delete[] temp;
		temp = nullptr;
	}
	size = 0; length = 0;

	const aiScene* scene = aiImportFile(path.c_str(), aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{

		LOGUI("[OK]- Scene %s loaded succesfully", path);

		aiNode* node = scene->mRootNode; 
	//	newObject->SetName(node->mName.C_Str());

		newObject->CreateComponent(COMP_TRANSFORMATION, -1, (DTransformation*)importerTransformations->ImportTrans(node));

		LOG("START-[OK] Loading meshes");

		// take the basic name 
			
		std::string fullPath = path;
		uint length = strlen(path.c_str());

		uint size = fullPath.find_last_of("\\");	
		char* pathname = new char[length - size];		
		fullPath.copy(pathname, length - size, size + 1);
		pathname[length] = '\0';

		for (int i = 0; i < scene->mNumMeshes; i++) // count all the meshes and adds the name.
		{
			scene->mMeshes[i]->mName = fullPath;
			scene->mMeshes[i]->mName.Append(std::to_string(i).c_str());
		}

		ImportGameObject(path, newObject, scene, node); //std::string path, GameObject * parent, const aiScene * scene, aiNode * node)
		
		aiReleaseImport(scene);
		return newObject;

	}
	else
	{
		LOGUI("[Error]- Error importing scene %s", path);
		return nullptr;
	}
	

}


bool ModuleDataManager::ImportGameObject(std::string path, GameObject * parent, const aiScene * scene, aiNode * node)
{
	bool ret = true;
	
	GameObject * GameObjectSon = new GameObject(parent);
	GameObjectSon->SetName(node->mName.C_Str());
	GameObjectSon->SetGOID(App->randGen->Int());

	GameObjectSon->CreateComponent(COMP_TRANSFORMATION, -1, (DTransformation*)importerTransformations->ImportTrans(node));

	for (int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* newMesh = scene->mMeshes[node->mMeshes[i]];

		if (newMesh != nullptr)
		{
		
			int nUID = ImportFile(GameObjectSon, newMesh->mName.C_Str(), newMesh);


			GameObjectSon->CreateComponent(COMP_MESH, -1, (DMesh*)GetContainerWithString(newMesh->mName.C_Str()));
			
	
			aiMaterial* newMaterial = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
			GameObjectSon->CreateComponent(COMP_TEXTURE, -1, (DTexture*)importerTexture->ImportTexture(newMaterial, path.c_str()));
			
		}
	}

	for (int i = 0; i < node->mNumChildren; ++i)
		ImportGameObject(path, GameObjectSon, scene, node->mChildren[i]);

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
	//	importerMesh->Save(buff,nullptr,id); //(const void * buffer, const char * saverFile, uint id
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


int ModuleDataManager::FindInMap(const char * name)
{
	for (std::map<int, DContainer*>::iterator it = dContainerMap.begin(); it != dContainerMap.end(); ++it)
	{
		if (strcmp(it->second->GetFile(), name) == 0)
			return it->first;
	}
	return 0;
}

DContainer * ModuleDataManager::GetContainer(int UID)
{
	std::map<int, DContainer*>::iterator it = dContainerMap.find(UID);
		if (it != dContainerMap.end())
			return it->second;

	return nullptr;
}

DContainer * ModuleDataManager::GetContainerWithString(std::string Dname)
{

	std::string fullPath = Dname;

	int i = fullPath.find_last_of("\\");

	if (i == -1)
	{
		std::string newpath = "Assets/";
		newpath.append(Dname);
		Dname = newpath.c_str();
	}
	else
	{
		int length = strlen(Dname.c_str()); // "/0"
		std::string namePath = Dname;

		int i = namePath.find_last_of("\\") + 1;

		if (length > 0 && i > 0)
		{
			char* temp = new char[length - i];
			namePath.copy(temp, length - i, i);
			fullPath = temp;
			fullPath[length - i] = '\0';
			delete[] temp;
			temp = nullptr;
		}
		Dname = fullPath.c_str();
	}


	int uid = FindInMap(Dname.c_str());

	if (uid == 0)
		return nullptr;
	else
		return GetContainer(uid);
}

int ModuleDataManager::ImportFile(GameObject * GO, const char * new_file_in_assets, aiMesh * mesh)
{	

		std::string fullPath = new_file_in_assets;

		int i = fullPath.find_last_of("\\");

		if (i == -1)
		{
			std::string newpath = "Assets/";
			newpath.append(new_file_in_assets);
			new_file_in_assets = newpath.c_str();
		}
		else
		{
			int length = strlen(new_file_in_assets); // "/0"
			std::string namePath = new_file_in_assets;

			int i = namePath.find_last_of("\\") + 1;

			if (length > 0 && i > 0)
			{
				char* temp = new char[length - i];
				namePath.copy(temp, length - i, i);
				fullPath = temp;
				fullPath[length - i] = '\0';
				delete[] temp;
				temp = nullptr;
			}
			new_file_in_assets = fullPath.c_str();
		}

		int UID = FindInMap(new_file_in_assets);

		if (UID == 0)
		{
			int ret = 0; bool import_ok = false;
			int UID = App->randGen->Int();


		std::string written_file = new_file_in_assets;

		//-------------		

		import_ok = importerMesh->ImportMesh(mesh, GO, new_file_in_assets);

		if (import_ok == true)
		{
			DContainer* newResource = CreateNewDataContainer(D_MESH, UID);
			newResource->file = new_file_in_assets;
			newResource->exportedFile = "Library/Mesh/";
			newResource->exportedFile += written_file;
			newResource->exportedFile += ".MCmesh";

			return UID;
		}
		else
		{
			UID = FindInMap(new_file_in_assets);
			return UID;
		}
		
	}
	else
		return UID;


	return UID;
}

DContainer * ModuleDataManager::CreateNewDataContainer(DType type, int UID)
{
	DContainer* ret = nullptr;

	switch (type)
	{
	case D_MESH:
	{
		ret = (DContainer*) new DMesh(UID, D_MESH);
		break;
	}
	case D_TEXTURE:
	{
		ret = (DContainer*) new DTexture(UID);
		break;
	}
	case D_TRANSFORMATION:
	{
		ret = (DContainer*) new DTransformation(UID);
		break;
	}
	case D_CAMERA:
	{
		ret = (DContainer*) new DCamera(UID);
		break;
	}
	}

	if (ret != nullptr)
	{
		dContainerMap[UID] = ret; //Put it on the map
	}

	return ret;
}


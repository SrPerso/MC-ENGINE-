#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "DContainer.h"
#include <string>
#include <map>

#include "IMesh.h"
#include "ITexture.h"

class GameObject;
class aiScene;
class aiNode;
class DContainer;

class ModuleDataManager : public Module 
{
public:
	ModuleDataManager(Application * app, bool start_enabled= true);
	virtual ~ModuleDataManager();

	bool Init();
	
	bool LoadInMemory();
	bool UnloadFromMemory();

	update_status Update(float dt)override {return UPDATE_CONTINUE;	}
	update_status PreUpdate(float dt)override { return UPDATE_CONTINUE; }
	update_status PostUpdate(float dt)override { return UPDATE_CONTINUE; }

	bool CleanUp() override;

	GameObject* ImportGameObject(std::string Path, GameObject*parent);
	bool ImportGameObject(std::string Path, GameObject*parent,const aiScene* scene, aiNode* node = nullptr);

	void CreateDir(const char* name);

	void SaveAllData()const; 
	void SaveData(const void * buff, DType type, uint id)const;


	// Objects creators?
	int FindInMap(const char*name);
	DContainer * GetContainer(int uid);
	DContainer * GetContainerWithString(std::string Dname);
	//void Add

	
	int ImportFile(GameObject*GO, const char* new_File_in_assets, aiMesh* mes);
	//Resources

	DContainer * CreateNewDataContainer(DType type, int UID);
public:
	ImporterMesh * importerMesh = nullptr;
	ImporterTrans * importerTransformations = nullptr;
	ImporterTexture * importerTexture = nullptr;

	std::map<int, DContainer*> dContainerMap;
};


#endif // !_DATA_MANAGER_H_

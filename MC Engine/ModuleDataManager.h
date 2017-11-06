#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "Globals.h"
#include "Module.h"
#include "DContainer.h"
#include <string>

#include "IMesh.h"
#include "ITexture.h"

class GameObject;
class aiScene;
class aiNode;


class ModuleDataManager : public Module 
{
public:
	ModuleDataManager(Application * app, bool start_enabled= true);
	virtual ~ModuleDataManager();

	update_status Update(float dt) {return UPDATE_CONTINUE;	}
	update_status PreUpdate(float dt){ return UPDATE_CONTINUE; }
	update_status PostUpdate(float dt){ return UPDATE_CONTINUE; }

	bool CleanUp() { return true; }

	GameObject* ImportGameObject(std::string Path, GameObject*parent);
	GameObject* ImportGameObject(std::string Path, GameObject*parent,const aiScene* scene, aiNode* node = nullptr);


	void SaveAllData()const; 
	void SaveData(const void * buff, DType type, uint id)const;

	void LoadAllData();
	
	// Objects creators?

public:
	ImporterMesh * importerMesh = nullptr;
	ImporterTrans * importerTransformations = nullptr;
	ImporterTexture * importerTexture = nullptr;
};


#endif // !_DATA_MANAGER_H_

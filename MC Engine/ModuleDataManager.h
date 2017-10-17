#ifndef _DATA_MANAGER_H_
#define _DATA_MANAGER_H_

#include "Globals.h"
#include "Module.h"


class GameObject;
class aiScene;
class aiNode;

class ModuleDataManager:public Module {
public:
	ModuleDataManager(Application * app, bool start_enabled= true);
	virtual ~ModuleDataManager();

	update_status Update(float dt) {return UPDATE_CONTINUE;	}
	update_status PreUpdate(float dt){ return UPDATE_CONTINUE; }
	update_status PostUpdate(float dt){ return UPDATE_CONTINUE; }

	bool CleanUp() { return true; }

	void Import(GameObject*parent,aiScene* scene, aiNode* node);

};


#endif // !_DATA_MANAGER_H_

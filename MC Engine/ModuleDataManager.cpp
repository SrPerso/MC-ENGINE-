#include "ModuleDataManager.h"
#include "Application.h"



ModuleDataManager::ModuleDataManager(Application* app, bool start_enabled) :Module(app, start_enabled)
{
	name = "Moudle Data Manager"; //this module is going to import all Data of the components. and future scene loader
}

ModuleDataManager::~ModuleDataManager()
{
}

void ModuleDataManager::Import(GameObject * parent, aiScene * scene, aiNode * node)
{
}

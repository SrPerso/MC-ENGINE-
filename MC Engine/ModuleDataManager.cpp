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

	//idea -> maybe this should go yo IScene

	//take transformation
	//number of childs of the node
	
	
	//meshes -> for to take every mesh and create a new game object for every mesh

	// calls to import buffers ( one switch with all importers)

	//make a function to asign the buffer to the component

}

void ModuleDataManager::ImportBuffers(const void * buffer, DType type)
{

	//my doubt i may to create the datacointainer before or insite the imports ( well after them)

	switch (type)
	{
	case D_MESH:
		//import mesh
		break;
	case D_MATERIAL:
		//import material
		break;
	case D_TEXTURE:
		//import texture
		break;
	case D_TRANSFORMATION:
		break;
	default:
		break;
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

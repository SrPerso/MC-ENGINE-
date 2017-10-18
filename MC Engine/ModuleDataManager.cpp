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

void ModuleDataManager::Import(GameObject * parent, aiScene * scene, aiNode * node)
{
	//idea -> maybe this should go yo IScene

	GameObject* GObject = new GameObject(parent);
	GObject->Enable();
	parent->AddChild(GObject);

	CTransformation* transformation = (CTransformation*)GObject->CreateComponent(COMP_TRANSFORMATION);
	
	float3 translation;
	float3 scale;
	Quat quaternion;

	//node->mTransformation.Decompose(scale, quaternion, translation);

	//number of childs of the node	
	
		//for(int num = node->mNumMeshes; num!=0; num--)
		//{
		//	if (node->mNumMeshes>1)
		//	{

		//		

		//	}
		//}
	
	
	//meshes -> for to take every mesh and create a new game object for every mesh

	// calls to import buffers ( one switch with all importers)

	//make a function to asign the buffer to the component

}

void ModuleDataManager::ImportBuffers(const void * buffer, DType type)
{
	bool controller = false;

	char* filePath;

	switch (type)
	{
	case D_MESH:
		importerMesh->ImportMesh((aiMesh*)buffer);
		LOGUI("[OK]- ImporterMesh");
		break;
	case D_MATERIAL:
		//import material
		LOGUI("[OK]- ImporterMaterial");
		break;
	case D_TEXTURE:

		importerTexture->ImportTexture((aiMaterial*)buffer,filePath);
		LOGUI("[OK]- ImporterTexture from %s", filePath);
		
		break;
	case D_TRANSFORMATION:
		LOGUI("[OK]- ImporterTransformation");
		break;

	default:
		LOGUI("[ERROR]- Cant import use this kind of importer");
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

#include "IScene.h"

#include "Assimp\include\cimport.h"
#include "Assimp\include\scene.h"
#include "Assimp\include\postprocess.h"
#include "Assimp\include\cfileio.h"

ImporterScene::ImporterScene()
{
}

ImporterScene::~ImporterScene()
{
}

bool ImporterScene::ImportScene(GameObject * parent, aiScene * scene, aiNode * node)
{

	if (node->mNumChildren > 1) {
	//	GameObject


	}




}

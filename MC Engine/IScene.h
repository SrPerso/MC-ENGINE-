#ifndef __IMPORTER_SCENE_H__
#define __IMPORTER_SCENE_H__

#include "Importer.h"
#include "GameObject.h"

struct aiScene;
struct aiNode;

class ImporterScene : public Importer
{
public:
	ImporterScene();
	~ImporterScene();	

	bool ImportScene(GameObject*parent, aiScene* scene, aiNode* node); //import all 
};

#endif
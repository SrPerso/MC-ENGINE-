#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "Globals.h"
#include "DMesh.h"
#include "DTransformation.h"
#include "GameObject.h"

struct aiMesh;
struct aiNode;

class ImporterMesh : public Importer
{
	friend class ModuleDataManager;
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	DMesh* ImportMesh(aiMesh*buffer, GameObject* object);
	DTransformation* ImportTrans(aiNode* node);

	void SaveMesh(DMesh);

	void LoadMesh(char* buffer, DMesh* data);
};





#endif

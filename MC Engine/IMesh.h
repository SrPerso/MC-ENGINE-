#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "Globals.h"
#include "GameObject.h"

struct aiMesh;
struct aiNode;

class DMesh;
class DTransformation;

class ImporterMesh : public Importer
{
	friend class ModuleDataManager;
public:
	ImporterMesh();
	~ImporterMesh();

	DMesh* ImportMesh(aiMesh*buffer, GameObject* object, uint id);
	DTransformation* ImportTrans(aiNode* node);

	void SaveMesh(DMesh);
	DMesh* Load(const void* buffer, const char* loadFile, uint id);
	bool Save(const void* buffer, const char * saverFile, uint id);

};





#endif

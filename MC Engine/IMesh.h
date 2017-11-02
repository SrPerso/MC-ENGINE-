#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "Globals.h"

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

	DMesh* ImportMesh(aiMesh*buffer);
	DTransformation* ImportTrans(aiNode* node);

	bool Save(const void* buffer, const char* saverFile, uint id);

	DMesh* Load(const void* buffer, const char* loadFile, uint id);
};





#endif

#ifndef __IMPORTER_MESH_H__
#define __IMPORTER_MESH_H__

#include "Importer.h"
#include "Globals.h"
#include "DMesh.h"

class aiMesh;

class ImporterMesh : public Importer
{
	friend class ModuleDataManager;
public:
	ImporterMesh();
	virtual ~ImporterMesh();

	virtual bool Import(const void*buffer, uint size, std::string& output_file);
	
	void SaveMesh(DMesh);

	void LoadMesh(char* buffer, DMesh* data);
};

#endif

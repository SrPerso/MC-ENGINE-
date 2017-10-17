#ifndef __IMPORTER_MATERIAL_H__
#define __IMPORTER_MATERIAL_H__

#include "Importer.h"
#include "DMaterial.h"

class aiMesh;

class ImporterMaterial : public Importer
{
public:
	ImporterMaterial();
	~ImporterMaterial();

	DMaterial* ImportMaterial(DMaterial* mesh);
};


#endif
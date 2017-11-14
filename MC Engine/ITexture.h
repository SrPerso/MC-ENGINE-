#ifndef __IMPORTER_MATERIAL_H__
#define __IMPORTER_MATERIAL_H__

#include "Importer.h"
#include "DTexture.h"

class aiMaterial;

class ImporterTexture : public Importer
{
public:
	ImporterTexture();
	~ImporterTexture();

	DTexture* ImportTexture(aiMaterial* newMaterial, const char*  FilePath);

	DTexture* Load(const void* buffer, const char* loadFile, uint id);
	bool Save(const void* buffer, const char * saverFile , uint id) override;
};

#endif
#ifndef __DATA_CONTAINER_H__
#define __DATA_CONTAINER_H__

#include <string>
#include "Globals.h"
enum DType 
{
	D_UNKNOWN,
	D_MESH,
	D_MATERIAL,
	D_TEXTURE,
	D_TRANSFORMATION,
	D_CAMERA
};

class DContainer
{
public:
	DContainer(int UID, DType resType = D_UNKNOWN);
	virtual	~DContainer();

	const char* GetFile()const;
	const char* GetExportedfile()const;

	bool IsLoadedToMemory()const;
	bool LoadToMemory();
	uint CountResourcesLoaded() const;

	virtual bool LoadInMemory() = 0;
	virtual bool UnloadFromMemory() = 0;

public:
	int UID=0;
	DType type = D_UNKNOWN;
	std::string Cname = "DContainer";


	std::string file;
	std::string exportedFile;

protected:
	uint loaded = 0;
};

#endif

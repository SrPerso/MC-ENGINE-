#include "DContainer.h"


DContainer::DContainer(int UID, DType resType): type(resType) , UID(UID)
{
}

DContainer::~DContainer()
{

}

const char * DContainer::GetFile() const
{
	return file.c_str();
}

const char * DContainer::GetExportedfile() const
{
	return exportedFile.c_str();
}

bool DContainer::IsLoadedToMemory() const
{
	return loaded > 0;
}

bool DContainer::LoadToMemory()
{
	if (loaded > 0)
		loaded++;

	else
	{
		if (LoadInMemory())
			return true;
		else
			return false;	
	}

	return loaded > 0;

}

uint DContainer::CountResourcesLoaded() const
{
	return uint(loaded);
}

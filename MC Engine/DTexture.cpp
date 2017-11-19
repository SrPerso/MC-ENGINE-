#include "DTexture.h"
#include "Application.h"
DTexture::DTexture(int UID, DType resType): DContainer(resType)
{

}

DTexture::~DTexture()
{
	//if (texCoords != nullptr)
	//{
	//	delete texCoords;
	//	texCoords = nullptr;
	//}
}

bool DTexture::LoadInMemory()
{
	App->texture->LoadTexture(exportedFile.c_str());

	if (idTexCoords == NULL)
		return false;

	return true;
}

bool DTexture::UnloadFromMemory()
{
	return true;
}

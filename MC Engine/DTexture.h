#ifndef __DATA_TEXTURE_H__
#define __DATA_TEXTURE_H__


#include "DContainer.h"
#include "Globals.h"
#include <string>

class DTexture : public DContainer
{
public:
	DTexture(int UID, DType resType = D_TEXTURE);
	DTexture::~DTexture();

	bool LoadInMemory();
	bool UnloadFromMemory();

public:

	uint idTexCoords = 0;
	//float* texCoords = nullptr;
	
	uint image = 0;
	std::string textNamePath;
	std::string textureName;

};

#endif
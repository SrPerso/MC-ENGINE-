#include "DTexture.h"

DTexture::DTexture(DType resType): DContainer(resType)
{

}

DTexture::~DTexture()
{
	if (texCoords != nullptr)
	{
		delete texCoords;
		texCoords = nullptr;
	}
}

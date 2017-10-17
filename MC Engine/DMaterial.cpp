#include "DMaterial.h"

DMaterial::~DMaterial()
{
	if (texCoords != nullptr)
	{
		delete texCoords;
		texCoords = nullptr;
	}

}

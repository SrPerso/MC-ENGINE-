
#ifndef __DATA_MATERIAL_H__
#define __DATA_MATERIAL_H__

#include "Globals.h"
#include <string>

class DMaterial
{
public:

	uint idTexCoords = 0;
	float* texCoords = nullptr;

	std::string texturePath;

public:
	DMaterial::~DMaterial();

};

#endif
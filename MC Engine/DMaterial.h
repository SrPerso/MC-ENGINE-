
#ifndef __DATA_MATERIAL_H__
#define __DATA_MATERIAL_H__

#include "DContainer.h"
#include "Globals.h"
#include <string>

class DMaterial : public DContainer
{
public:
	DMaterial(int UID, DType);
	~DMaterial();

	bool LoadInMemory();
	bool UnloadFromMemory();
public:

};

#endif
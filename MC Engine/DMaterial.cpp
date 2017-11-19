#include "DMaterial.h"

DMaterial::DMaterial(int UID, DType type): DContainer(UID,D_MATERIAL)
{
}

DMaterial::~DMaterial()
{

}

bool DMaterial::LoadInMemory()
{
	return true;
}

bool DMaterial::UnloadFromMemory()
{
	return true;
}

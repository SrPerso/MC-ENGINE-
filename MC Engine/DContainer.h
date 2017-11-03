#ifndef __DATA_CONTAINER_H__
#define __DATA_CONTAINER_H__

#include <string>

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
	DContainer(DType resType = D_UNKNOWN);
	virtual	~DContainer();

	DType type;
	std::string Cname = "DContainer";
};

#endif

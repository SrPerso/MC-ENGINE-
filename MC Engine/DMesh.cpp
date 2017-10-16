#include "DMesh.h"
#ifndef __DATA_MESH_H__
#define __DATA_MESH_H__
















#endif

DMesh::~DMesh()
{
	if (Vertex != nullptr)
	{
		delete Vertex;
		Vertex = nullptr;
	}
	if (Index != nullptr)
	{
		delete Index;
		Index = nullptr;
	}
	if (normals != nullptr)
	{
		delete normals;
		normals = nullptr;
	}
}

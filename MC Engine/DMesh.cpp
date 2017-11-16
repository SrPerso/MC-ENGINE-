#include "DMesh.h"

DMesh::DMesh(DType resType):DContainer(D_MESH)
{

}

DMesh::~DMesh()
{
	if (Vertex != nullptr)
	{
		delete[] Vertex;
		Vertex = nullptr;
	}

	if (Index != nullptr)
	{
		delete[] Index;
		Index = nullptr;
	}

	if (normals != nullptr)
	{
		delete[] normals;
		normals = nullptr;
	}

	if (colors != nullptr)
	{
		delete[] colors;
		colors = nullptr;
	}

	if (texCoords != nullptr)
	{
		delete[] texCoords;
		texCoords = nullptr;
		}
	
}


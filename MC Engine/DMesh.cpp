#include "DMesh.h"
#include "Application.h"
DMesh::DMesh(int UID, DType resType):DContainer(UID,D_MESH)
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

bool DMesh::LoadInMemory()
{
	App->datamanager->importerMesh->Save(this,file.c_str());

	return true;
}

bool DMesh::UnloadFromMemory()
{
	return true;
}


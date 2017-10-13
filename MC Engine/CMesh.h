#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"

class DataMesh {
public:
	uint nVertex = 0;
	uint idVertex = 0;//id RAM
	float* Vertex = nullptr;
	
	bool wire = false;
	
	uint nIndex = 0;
	uint idIndex = 0;//id RAM
	float* Index = nullptr;
	
	uint nFaces = 0;

	float* normals = nullptr;
	uint idNormals = 0;
	uint nNormals = 0;

	uint idColors = 0; 
	float* colors = nullptr;

	uint idTexCoords = 0; 
	float* texCoords = nullptr;

	bool debugMode = true;
	AABB debugBox;

	DataMesh::~DataMesh();

};

class CMesh : public Component
{
public:
	CMesh(GameObject* object);
	~CMesh();

	void OnUpdate(float dt);

	
public:
	DataMesh* data;
};

#endif
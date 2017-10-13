#ifndef __MESH_H__
#define __MESH_H__

#include "Component.h"
#include "MathGeolib\Geometry\AABB.h"
class GameObject;
class Component;

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
	

	bool debugMode = true;
	AABB debugBox;
public:
	DataMesh::~DataMesh();

};

class CMesh : public Component, public DataMesh
{
public:
	CMesh(GameObject* object);
	~CMesh();

	void OnUpdate(float dt);
};

#endif
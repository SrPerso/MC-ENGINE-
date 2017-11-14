#ifndef __DATA_MESH_H__
#define __DATA_MESH_H__

#include "DContainer.h"
#include "Globals.h"
#include "MathGeolib\Geometry\AABB.h"


class DMesh : public DContainer
{
public:

	DMesh(DType type = D_MESH);
	~DMesh();
public:

	uint nVertex = 0;
	uint idVertex = 0;//id RAM
	float* Vertex = nullptr;

	bool wire = false;

	uint nIndex = 0;
	uint idIndex = 0;//id RAM
	uint* Index = nullptr;

	uint nFaces = 0;

	float* normals = nullptr;
	uint idNormals = 0;
	uint nNormals = 0;
 
	uint idColors = 0;
	float* colors = nullptr;

	uint idTexCoords = 0;
	float* texCoords = nullptr;

	bool debugMode = false;
	//

	AABB debugBox;

};

#endif
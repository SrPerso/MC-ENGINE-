#ifndef __MESH_H__
#define __MESH_H__

#include "DMesh.h"
#include "Component.h"

#include "MathGeolib\Geometry\AABB.h"

#include <string>


class GameObject;

class CMesh : public Component, public DMesh
{
public:
	CMesh(GameObject* object);
	~CMesh();

	void OnUpdate(float dt);
	void OnEditor();

	uint mesh_ID;
};

#endif
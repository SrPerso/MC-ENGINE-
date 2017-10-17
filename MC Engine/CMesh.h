#ifndef __MESH_H__
#define __MESH_H__

#include "DMesh.h"
#include "Component.h"

#include "MathGeolib\Geometry\AABB.h"

#include <string>


class GameObject;
enum Component_Type;
class CMesh : public Component, public DMesh
{
public:
	CMesh(GameObject* object, Component_Type type = COMP_MESH );
	~CMesh();

	void OnUpdate(float dt);
	void OnEditor();

	uint mesh_ID;
};

#endif
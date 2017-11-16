#ifndef __MESH_H__
#define __MESH_H__

#include "DMesh.h"
#include "Component.h"
#include "Glew\include\glew.h"

#include "MathGeolib\Geometry\AABB.h"
#include <map>
#include <string>

class DataJSON;
class GameObject;
enum Component_Type;

class CMesh : public Component
{
public:
	CMesh(GameObject* object,int UID, Component_Type type = COMP_MESH , DMesh* data = nullptr);
	~CMesh();

	void OnUpdate(float dt) override;
	void OnEditor() override;
	void OnInspector() override;
	
	void OnCleanUp() override;

	void OnSave(DataJSON&file)const override;
	void OnLoad(DataJSON&file) override;

	bool IntersectTriangle(LineSegment & picking, float& distance, float3 &hitPoint);
	void Move(float3 destiny, float3 position);
	const void* GetData();
	void SetData(DMesh*data);
	uint mesh_ID;

public: 
	DMesh*  dataMesh;
};

#endif
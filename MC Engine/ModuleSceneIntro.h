#pragma once


#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>


struct PhysBody3D;
struct PhysMotor3D;



struct GlobalDebugInfo
{
	uint tris = 0;
	uint vertex = 0;
	uint faces = 0;
};

class ModuleSceneIntro : public Module
{
public:
	ModuleSceneIntro(Application* app, bool start_enabled = true);
	~ModuleSceneIntro();
	int num = 0;
	
	bool Start();
	update_status Update(float dt);
	update_status PreUpdate(float dt);
	bool CleanUp();

	void Draw();
	bool intersects = false;


	void CreateCube(vec3 size, vec3 pos=vec3(0,0,0));
	void CreateSphere(float radius, vec3 pos, int numStacks, int numSides);
	void CreateCylinder(float radius,float height, vec3 pos= vec3(0,0,0));
	void CreateCube1(vec3 size, vec3 pos);
	void CreateCube2(vec3 size, vec3 pos);
	void CreateLine(vec3 Origin, vec3 destintation, Color color = Red);

	void SelectObject(LineSegment &picking);

	GameObject* IntersectAABB(LineSegment &picking);
	void IntersectTriangle(LineSegment &picking, GameObject* closest);

public:
	std::list<Primitive*> GeometryObjects;

	std::list<PrimitiveLine*> NormalsLines;

	std::list<GameObject*> DistanceList;
	
	GlobalDebugInfo sceneDebugInfo;

	float MinDistance;
public:
	void CreateCylinder(const float x, const float y, const float z, const float radious, const float h);
};

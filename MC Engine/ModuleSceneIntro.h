#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>
#include "MathGeolib\Geometry\AABB.h"

struct PhysBody3D;
struct PhysMotor3D;


class ObjectMesh {
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

	bool debugMode =true;

};
struct GlobalDebugInfo
{
	uint tris = 0;
	double vertex = 0;
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

	void CreateMesh(ObjectMesh*Mesh);

public:
	std::list<Primitive*> GeometryObjects;
	std::list<ObjectMesh*> MeshObjects;
	std::list<PrimitiveLine*> NormalsLines;
	
	GlobalDebugInfo sceneDebugInfo;
	bool debugMode=true;
public:
	void CreateCylinder(const float x, const float y, const float z, const float radious, const float h);
};

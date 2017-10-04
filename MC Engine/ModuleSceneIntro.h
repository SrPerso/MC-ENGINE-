#pragma once
#include "Module.h"
#include "p2DynArray.h"
#include "Globals.h"
#include "Primitive.h"
#include <list>

struct PhysBody3D;
struct PhysMotor3D;

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
	void CreateSphere(float radius, vec3 pos = vec3(0, 0, 0));
	void CreateCylinder(float radius,float height, vec3 pos= vec3(0,0,0));
	

	std::list<Primitive*> GeometryObjects;

public:
	void CreateCylinder(const float x, const float y, const float z, const float radious, const float h);
};

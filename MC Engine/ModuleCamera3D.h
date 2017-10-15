#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"



#define ZOOM_FACTOR 1.6

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference = false);
	void LookAt(const vec3 &Spot);
	void Move(const vec3 &Movement);
	float* GetViewMatrix();

	void CenterCameraToObject(AABB* box);
	void CenterCamera();
private:

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	AABB* GeometryCentre;
private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
};
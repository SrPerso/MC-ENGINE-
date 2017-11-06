#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "DCamera.h"
#include "CCamera.h"
#include "MathGeoLib\Geometry\LineSegment.h"


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
	const float* GetViewMatrix();

	

	void CenterCameraToObject(AABB* box);
	void CenterCamera();

	void CalculateViewMatrix();

public:
	
	vec3 X, Y, Z, Position, Reference;
	AABB* GeometryCentre;

	vec3 centreB;
	DCamera* dcamera = nullptr;
	CCamera* mainCam = nullptr;
	CCamera* editorCam= nullptr;
	GameObject* selected=nullptr;
	LineSegment thispick;
private:

	mat4x4 ViewMatrix, ViewMatrixInverse;
	
};
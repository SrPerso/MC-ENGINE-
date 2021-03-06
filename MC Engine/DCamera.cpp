#include "DCamera.h"

DCamera::DCamera(int UID,float3 pos, float3 front, float3 up, float nearPlaneDistance, float farPlaneDistance, float verticalFov, float aspectRatio, FrustumType type) :DContainer(UID)
{
	//name = "Camera";
	Active = false;
	this->aspectRatio = aspectRatio;
	this->aspectRatio = (float)16 / 9;
	frustum.type = type;
	frustum.pos = pos;
	frustum.front = front;
	frustum.up = up;
	frustum.nearPlaneDistance = nearPlaneDistance;
	frustum.farPlaneDistance = farPlaneDistance;
	FOV = verticalFov;
	frustum.verticalFov = DEGTORAD * FOV;
	frustum.horizontalFov = 2.f * atanf((tanf(frustum.verticalFov * 0.5f)) * (aspectRatio));

	frustum.ProjectionMatrix();

	needToCull = false;
}

DCamera::~DCamera()
{

}

bool DCamera::LoadInMemory()
{
	return true;
}

bool DCamera::UnloadFromMemory()
{
	return true;
}

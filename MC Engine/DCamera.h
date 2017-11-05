#ifndef __DATA_CAMERA_H__
#define __DATA_CAMERA_H__

#include "Component.h"
#include "DContainer.h"
#include "Globals.h"
#include <string>
#include "MathGeoLib\Geometry\Frustum.h"
#include "MathGeolib\Math\float3.h"
#include "MathGeolib\Math\float4x4.h"

#include <map>
#include <string>


class DCamera : public DContainer
{


public:
	DCamera(float3 pos = { 0, 0, 0 }, float3 front = { 0, 0, 1 }, float3 up = { 0, 1, 0 }, float nearPlaneDistance = 0.5f, float farPlaneDistance = 2000.0f, float verticalFov = 15.0f, float aspectRatio = (float)16 / 9, FrustumType type = PerspectiveFrustum);
	~DCamera();

public:
	float FOV;
	float aspectRatio;
	Frustum frustum;
	bool Active;
	bool needToCull;
};

#endif
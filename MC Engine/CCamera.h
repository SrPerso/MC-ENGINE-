#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "DCamera.h"
#include "Component.h"
#include "Glew\include\glew.h"

#include "MathGeoLib\Geometry\Frustum.h"
#include <map>
#include <string>
#include "DCamera.h"



class GameObject;
enum Component_Type;
class CCamera : public Component, public DCamera
{
public:
	
	CCamera(GameObject* object, Component_Type type = COMP_UNKNOWN);
	
	CCamera(GameObject* object, Component_Type type = COMP_TRANSFORMATION, DCamera* data = nullptr);
	~CCamera();

	void SetPos(float3 newPos);
	void OnUpdate(float dt);
	void OnEditor();
	void OnInspector();
	void DrawFrustum();
	bool Contains(const AABB & aabb) const;
	
};

#endif
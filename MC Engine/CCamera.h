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
	
	CCamera(Component_Type type = COMP_UNKNOWN, DCamera* data = nullptr);
	
	CCamera(GameObject* object, Component_Type type = COMP_TRANSFORMATION, DCamera* data = nullptr);
	~CCamera();

	void SetPos(float3 newpos, float3 front, float3 up);
	void OnUpdate(float dt);
	void OnEditor();
	void OnInspector();
	void DrawFrustum();
	bool Contains(const AABB & aabb) const;
	Frustum GetFrustum()const;

	const float* GetViewMatrix() const;
	
};

#endif
#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "DCamera.h"
#include "Component.h"
#include "Glew\include\glew.h"

#include "MathGeoLib\Geometry\Frustum.h"
#include <map>
#include <string>
#include "DCamera.h"
#include "ModuleDataFile.h"
class GameObject;
enum Component_Type;


class CCamera : public Component, public DCamera
{
public:
	
	CCamera(int UID, Component_Type type = COMP_UNKNOWN, DCamera* data = nullptr);
	
	CCamera(GameObject* object, int UID, Component_Type type = COMP_TRANSFORMATION, DCamera* data = nullptr);
	~CCamera();
	
	void OnUpdate(float dt) override;
	void OnEditor() override;
	void OnInspector() override;

	void OnSave(DataJSON&file)const override;
	void OnLoad(DataJSON&file) override;

	void SetPos(float3 newpos, float3 front, float3 up);
	void DrawFrustum();
	bool Contains(const AABB & aabb) const;
	Frustum GetFrustum()const;

	const float* GetViewMatrix() const;
	
};

#endif
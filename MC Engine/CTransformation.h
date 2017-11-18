#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include <string>
#include <vector>
#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"
#include "MathGeolib\Math\float4x4.h"
#include "DTransformation.h"
#include "ImGuizmo\ImGuizmo.h"

class DataJSON;
class GameObject;


class CTransformation : public Component
{
public:

	//CTransformation() :Component(nullptr, COMP_TRANSFORMATION){}
//	CTransformation(GameObject* object,int UID, Component_Type type = COMP_TRANSFORMATION) :Component(object, UID, type) {}
	CTransformation(GameObject* object,int UID, Component_Type type = COMP_TRANSFORMATION, DTransformation* data = nullptr);

	~CTransformation();


	void OnUpdate(float dt) override;
	void OnEditor() override;
	void OnInspector() override;
	
	void OnCleanUp() override;

	void OnSave(DataJSON&file)const override;
	void OnLoad(DataJSON&file) override;

	void TransUpdate();
	void UpdateTransFromParent(GameObject* parent);
	void SetLocalTrans(GameObject* parent);

	void SetPos(float3);
	uint Transformation_ID;// is not necesary (?)
	float4x4 GetTransMatrix();

	void EditTransform(const CCamera camera);

	void OnGuizmo();
	//ImGuizmo::OPERATION Operator_Guiz = ImGuizmo::OPERATION::ROTATE;
public:
	DTransformation* dataTransformation;
};

#endif //__TRANSFORMATION_H__
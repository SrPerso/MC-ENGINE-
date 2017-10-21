#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "Application.h"
#include "Component.h"
#include "GameObject.h"
#include <string>
#include <vector>
#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"
#include "DTransformation.h"
#include "MathGeolib\Math\float4x4.h"

class GameObject;


class CTransformation : public Component, public DTransformation
{
public:

	//CTransformation() :Component(nullptr, COMP_TRANSFORMATION){}
	CTransformation(GameObject* object, Component_Type type = COMP_TRANSFORMATION) :Component(object, type) {}
	CTransformation(GameObject* object, Component_Type type = COMP_TRANSFORMATION, DTransformation* data = nullptr);
	~CTransformation();

	void OnUpdate(float dt);
	void OnEditor();
	void Rotate(float3 rotation);
	void UpdateTransform();

	void SetPos(float3);
	uint Transformation_ID;// is not necesary (?)
	void SetLocalRotation(Quat newRot);

public:
	


};

#endif //__TRANSFORMATION_H__
#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "Component.h"
#include <string>
#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"
#include "DTransformation.h"

class GameObject;

class CTransformation : public Component, public DTransformation
{
public:
	CTransformation(GameObject* object);
	~CTransformation();

	void OnUpdate(float dt);
	void OnEditor();

	void SetPos(float3);
	uint Transformation_ID;// is not necesary (?)
public:

};

#endif //__TRANSFORMATION_H__
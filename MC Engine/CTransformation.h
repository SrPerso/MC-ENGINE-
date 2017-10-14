#ifndef __TRANSFORMATION_H__
#define __TRANSFORMATION_H__

#include "Component.h"
#include <string>
#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"


class GameObject;

class CTransformation : public Component
{
public:
	CTransformation(GameObject* object);
	~CTransformation();

	void OnUpdate(float dt);
	void OnEditor();

	void SetPos(float3);
	uint Transformation_ID;// is not necesary (?)
public:
	float3 position;
	float3 scale;
	Quat rotation;
};

#endif //__TRANSFORMATION_H__
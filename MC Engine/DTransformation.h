#ifndef __DATA_TRANSFORMATION_H__
#define __DATA_TRANSFORMATION_H__

#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"

class DTransformation
{
public:
	DTransformation(){}
	DTransformation(float3 pos, float3 scale,Quat rot);

	float3 position;
	float3 destiny;
	float3 scale;
	Quat rotation;

	float3 GetPosition()const;
	float3 GetScale()const;
	Quat GetRotation()const;

	void SetPosition(float3);
	void SetScale(float3);
	void SetRotation(Quat);

	float3 GetGlobalPosition()const;
	float3 GetGlobalScale()const;
	Quat GetGlobalRotation()const;
	
	void SetGlobalPosition(float3);
	void SetGlobalScale(float3);
	void SetGlobalRotation(Quat);

public:
	DTransformation::~DTransformation();

protected:
	float3 globalPosition;
	float3 globalScale;
	Quat globalRotation;
	bool moving;
};

#endif
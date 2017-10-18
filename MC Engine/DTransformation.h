#ifndef __DATA_TRANSFORMATION_H__
#define __DATA_TRANSFORMATION_H__

#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"

class DTransformation
{
public:
	DTransformation();

	float3 position;
	float3 scale;
	Quat rotation;

	float3 GetGlobalPosition()const;
	float3 GetGlobalScale()const;
	Quat GetGlobalRotation()const;
	
	void SetGlobalPosition(float3);
	void SetGlobalScale(float3);
	void SetGlobalRotation(Quat);

public:
	DTransformation::~DTransformation();

private:
	float3 globalPosition;
	float3 globalScale;
	Quat globalRotation;
};

#endif
#ifndef __DATA_TRANSFORMATION_H__
#define __DATA_TRANSFORMATION_H__

#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"

class DTransformation
{
public:

	float3 position;
	float3 scale;
	Quat rotation;

public:
	DTransformation::~DTransformation();

};

#endif
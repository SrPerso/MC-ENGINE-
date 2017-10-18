#include "DTransformation.h"

DTransformation::DTransformation()
{
}

float3 DTransformation::GetGlobalPosition() const
{
	return float3(globalPosition);
}

float3 DTransformation::GetGlobalScale() const
{
	return float3(globalScale);
}

Quat DTransformation::GetGlobalRotation() const
{
	return Quat(globalRotation);
}

void DTransformation::SetGlobalPosition(float3 newPos)
{
	globalPosition = newPos;
}

void DTransformation::SetGlobalScale(float3 newScale)
{
	globalScale = newScale;
}

void DTransformation::SetGlobalRotation(Quat newRotation)
{
	globalRotation = newRotation;
}

DTransformation::~DTransformation()
{

}

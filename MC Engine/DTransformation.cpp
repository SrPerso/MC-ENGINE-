#include "DTransformation.h"
#include "Application.h"

DTransformation::DTransformation(int UID,float3 pos, float3 scales, Quat rot) :DContainer(UID, D_TRANSFORMATION)
{
	
	position = pos;
	destiny = pos;
	scale = scales;
	eulerAngles = rot.ToEulerXYZ();
	eulerAngles *= RADTODEG;
	angle =rot.Angle();
	rotation = rot;
	globalTransformMatrix = float4x4::FromQuat(rot);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(pos.x, pos.y, pos.z);

	localTransformMatrix = globalTransformMatrix;
}

float3 DTransformation::GetPosition() const
{
	return float3(destiny);
}

float3 DTransformation::GetScale() const
{
	return float3(scale);
}

Quat DTransformation::GetRotation() const
{
	return Quat(rotation);
}

void DTransformation::SetPosition(float3 position)
{
	this->destiny = position;
}

void DTransformation::SetScale(float3 scale)
{
	this->scale = scale;
}

void DTransformation::SetRotation(Quat rotation)
{
	this->rotation = rotation;
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

bool DTransformation::GetUpdateTrans() const
{
	return UpdateTrans;
}

void DTransformation::SetUpdateTrans(bool boool)
{
	UpdateTrans = boool;
}

DTransformation::~DTransformation()
{

}

bool DTransformation::LoadInMemory()
{
	return true;
}

bool DTransformation::UnloadFromMemory()
{
	return true;
}


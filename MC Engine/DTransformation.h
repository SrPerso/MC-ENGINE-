#ifndef __DATA_TRANSFORMATION_H__
#define __DATA_TRANSFORMATION_H__

#include "MathGeolib\Math\Quat.h"
#include "MathGeolib\Math\float3.h"
#include "MathGeolib\Math\float4x4.h"
#include "DContainer.h"

class DTransformation : public DContainer
{
public:

	DTransformation(int UID, float3 pos = {0,0,0}, float3 scale = {1 ,1,1 }, Quat rot = { 0,0,0,1 });
	virtual ~DTransformation();
	
	bool LoadInMemory();
	bool UnloadFromMemory();

	float3 position;
	float3 destiny;
	float3 scale;
	Quat rotation;
	float angle;

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

	bool GetUpdateTrans()const;
	void SetUpdateTrans(bool);

public:

	float3 eulerAngles;
	float4x4 globalTransformMatrix;
	float4x4 localTransformMatrix;
	float3 newPos;

	bool OnTransform;

private:
	float3 globalPosition;
	float3 globalScale;
	Quat globalRotation;
	bool UpdateTrans;
};

#endif
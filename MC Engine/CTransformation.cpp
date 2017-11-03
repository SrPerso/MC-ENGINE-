#include "CTransformation.h"



CTransformation::CTransformation(GameObject * object, Component_Type type, DTransformation* data) :Component(object, type)
{
	
	if (data)
	{
			position = data->position;
			scale = data->scale;
			destiny = data->destiny;
			eulerAngles = data->eulerAngles;
			angle = data->angle;
			rotation = data->rotation;
			globalTransformMatrix = data->globalTransformMatrix;


			localTransformMatrix = data->localTransformMatrix;

		 //globalPosition = data->GetGlobalPosition();
		 //globalScale = data->GetGlobalScale();
		 //globalRotation = data->GetGlobalRotation();

	}
	if (object != nullptr)
	{
		this->Transformation_ID = object->NumComponentTypeSize(this->Ctype) + 1;
	}
	else
	{
		this->Transformation_ID = 0;
	}
	
	name = "- Component Transformation_";
	name.append(std::to_string(Transformation_ID));
}

CTransformation::~CTransformation()
{
}

void CTransformation::OnUpdate(float dt)
{
	if (UpdateTrans && object != nullptr)
	{
		position = destiny;
		TransUpdate();
		//UpdateTrans = false;
		
	}

	
}

void CTransformation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		
		ImGui::TreePop();
	}
	
	
}

void CTransformation::OnInspector() {

	
		ImGui::Text("   Position:");

		ImGui::Text("\t X = %.2f", destiny.x);
		ImGui::Text("\t Y = %.2f", destiny.y);
		ImGui::Text("\t Z = %.2f", destiny.z);

		ImGui::Text("Position:");
		if (ImGui::SliderFloat("X", &destiny.x, -50, 50))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("Y", &destiny.y, -50, 50))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("Z", &destiny.z, -50, 50))
		{
			UpdateTrans = true;
		}

		ImGui::Text("   Rotation:");

		ImGui::Text("\t X = %.2f", rotation.x);
		ImGui::Text("\t Y = %.2f", rotation.y);
		ImGui::Text("\t Z = %.2f", rotation.z);
		ImGui::Text("\t W = %.2f", rotation.w);

		ImGui::Text("Rotation:");
		if (ImGui::SliderFloat("RX", &eulerAngles.x, 0, 360))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("RY", &eulerAngles.y, 0, 360))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("RZ", &eulerAngles.z, 0, 360))
		{
			UpdateTrans = true;
		}

		ImGui::Text("   Scale:");

		ImGui::Text("\t X = %.2f", scale.x);
		ImGui::Text("\t Y = %.2f", scale.y);
		ImGui::Text("\t Z = %.2f", scale.z);
	
}
void CTransformation::TransUpdate()
{
	//eulerAngles.x *= DEGTORAD;
	//eulerAngles.y *= DEGTORAD;
	//eulerAngles.z *= DEGTORAD;
	rotation = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	//eulerAngles.x *= RADTODEG;
	//eulerAngles.y *= RADTODEG;
	//eulerAngles.z *= RADTODEG;
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	SetLocalTrans(object->GetParent());
	object->UpdateTranformChilds();

	UpdateTrans = false;
}

void CTransformation::UpdateTransFromParent(GameObject * parent)
{
	CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
	if (parentTrans != nullptr)
	{
		globalTransformMatrix = parentTrans->localTransformMatrix * globalTransformMatrix;
		float4x4 temp;
		globalTransformMatrix.Decompose(destiny, temp, scale);
		eulerAngles = temp.ToEulerXYZ();
		TransUpdate();
	}
}

void CTransformation::SetLocalTrans(GameObject * parent)
{
	if (parent != nullptr)
	{
		CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
		if (parentTrans != nullptr)
		{
			float3 localPos = position - parentTrans->position;
			Quat localRot = rotation * parentTrans->rotation.Conjugated();
			float3 localScale = scale.Mul(parentTrans->scale.Recip());

			localTransformMatrix = float4x4::FromQuat(localRot);
			globalTransformMatrix = float4x4::Scale(localScale, float3(0, 0, 0)) * globalTransformMatrix;
			globalTransformMatrix.float4x4::SetTranslatePart(localPos.x, localPos.y, localPos.z);
		}
		else
		{
			localTransformMatrix = globalTransformMatrix;
		}
	}
	else
	{
		localTransformMatrix = globalTransformMatrix;
	}
}

void CTransformation::SetPos(float3 pos)
{
	position = pos;
}

float4x4 CTransformation::GetTransMatrix()
{
	return globalTransformMatrix;
}

void CTransformation::Rotate()
{	
	rotation = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	SetLocalTrans(object->GetParent());
	object->UpdateTranformChilds();

	//rotating = true;
	/*if (newRotation.y > 0) {
		SetRotation( rotation * Quat::RotateY(newRotation.y));
	}
	else if (newRotation.x > 0) {
		SetRotation( rotation * Quat::RotateX(newRotation.x));
	}
	else if (newRotation.z > 0) {
		SetRotation(rotation * Quat::RotateZ(newRotation.z));
	}*/
}
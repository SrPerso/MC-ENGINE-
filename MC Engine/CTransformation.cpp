#include "CTransformation.h"



CTransformation::CTransformation(GameObject * object, Component_Type type, DTransformation* data) :Component(object, type)
{
	moving = false;
	rotating = false;
	if (data)
	{
		 position = data->GetPosition();
		 scale = data->GetScale();
		 rotation = data->GetRotation();
		 destiny = data->GetPosition();

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
	dType = D_TRANSFORMATION;
}

CTransformation::~CTransformation()
{
}

void CTransformation::OnUpdate(float dt)
{
	if (moving && object != nullptr)
	{
		object->Move(destiny, position);
		moving = false;
		position = destiny;
	}

	else if (rotating) {
		position = newPos;
		Rotate();
		rotating = false;
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
			moving = true;
		}
		if (ImGui::SliderFloat("Y", &destiny.y, -50, 50))
		{
			moving = true;
		}
		if (ImGui::SliderFloat("Z", &destiny.z, -50, 50))
		{
			moving = true;
		}

		ImGui::Text("   Rotation:");

		ImGui::Text("\t X = %.2f", rotation.x);
		ImGui::Text("\t Y = %.2f", rotation.y);
		ImGui::Text("\t Z = %.2f", rotation.z);
		ImGui::Text("\t W = %.2f", rotation.w);

		ImGui::Text("Rotation:");
		if (ImGui::SliderFloat("RX", &eulerAngles.x, 0, 360))
		{
			rotating = true;
		}
		if (ImGui::SliderFloat("RY", &eulerAngles.y, 0, 360))
		{
			rotating = true;
		}
		if (ImGui::SliderFloat("RZ", &eulerAngles.z, 0, 360))
		{
			rotating = true;
		}

		ImGui::Text("   Scale:");

		ImGui::Text("\t X = %.2f", scale.x);
		ImGui::Text("\t Y = %.2f", scale.y);
		ImGui::Text("\t Z = %.2f", scale.z);
	
}
const void* CTransformation::GetData()
{
	DTransformation* data;

	data->SetGlobalPosition(position);
	data->SetGlobalScale(scale);
	data->SetGlobalRotation(rotation);

	return (DTransformation*)data;
}

void CTransformation::UpdateTransFromParent(GameObject * parent)
{
	CTransformation* parentTrans = (CTransformation*)parent->GetComponent(COMP_TRANSFORMATION);
	if (parentTrans != nullptr)
	{
		globalTransformMatrix = parentTrans->localTransformMatrix * globalTransformMatrix;
		float4x4 temp;
		globalTransformMatrix.Decompose(newPos, temp, scale);
		eulerAngles = temp.ToEulerXYZ();
		Rotate();
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

void CTransformation::Rotate()
{	
	rotation = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	globalTransformMatrix = float4x4::FromQuat(rotation);
	globalTransformMatrix = float4x4::Scale(scale, float3(0, 0, 0)) * globalTransformMatrix;
	globalTransformMatrix.float4x4::SetTranslatePart(position.x, position.y, position.z);

	SetLocalTrans(object->GetParent());
	object->UpdateTranformChilds();

	rotating = true;
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
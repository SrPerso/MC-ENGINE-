#include "CTransformation.h"
#include "ModuleDataFile.h"


CTransformation::CTransformation(GameObject * object,int UID, Component_Type type, DTransformation* data) :Component(object, UID, type)
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
		if (ImGui::SliderFloat("X", &destiny.x, -100, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("Y", &destiny.y, -100, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("Z", &destiny.z, -100, 100))
		{
			UpdateTrans = true;
		}

		ImGui::Text("   Rotation:");

		ImGui::Text("\t X = %.2f", eulerAngles.x);
		ImGui::Text("\t Y = %.2f", eulerAngles.y);
		ImGui::Text("\t Z = %.2f", eulerAngles.z);	

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

		ImGui::Text("Scale:");

		ImGui::Text("\t X = %.2f", scale.x);
		ImGui::Text("\t Y = %.2f", scale.y);
		ImGui::Text("\t Z = %.2f", scale.z);

		if (ImGui::SliderFloat("SX", &scale.x, 1, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("SY", &scale.y, 1, 100))
		{
			UpdateTrans = true;
		}
		if (ImGui::SliderFloat("SZ", &scale.z, 1, 100))
		{
			UpdateTrans = true;
		}
	
}


void CTransformation::OnSave(DataJSON & file) const
{


}



void CTransformation::OnLoad(DataJSON & file)
{


}



void CTransformation::TransUpdate()
{
	eulerAngles.x *= DEGTORAD;
	eulerAngles.y *= DEGTORAD;
	eulerAngles.z *= DEGTORAD;
	rotation = Quat::FromEulerXYZ(eulerAngles.x, eulerAngles.y, eulerAngles.z);
	eulerAngles.x *= RADTODEG;
	eulerAngles.y *= RADTODEG;
	eulerAngles.z *= RADTODEG;
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


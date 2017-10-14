#include "CTransformation.h"
#include "GameObject.h"


CTransformation::CTransformation(GameObject * object) :Component(object, COMP_TRANSFORMATION)
{
	if (object != nullptr)
	{
		this->Transformation_ID = object->NumComponentTypeSize(this->type) + 1;
	}
	else
	{
		this->Transformation_ID = 0;
	}

	name = "Component Texture_";
	name.append(std::to_string(Transformation_ID));
}

CTransformation::~CTransformation()
{
}

void CTransformation::OnUpdate(float dt)
{
}

void CTransformation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("Position:");

		ImGui::Text("X = %f",position.x);
		ImGui::SameLine();
		ImGui::Text("Y = %f", position.y);
		ImGui::SameLine(); 
		ImGui::Text("Z = %f", position.z);


		ImGui::Text("Scale:");

		ImGui::Text("X = %f", scale.x);
		ImGui::SameLine();
		ImGui::Text("Y = %f", scale.y);
		ImGui::SameLine();
		ImGui::Text("Z = %f", scale.z);

		ImGui::Text("Rotation:");
	
		ImGui::Text("X = %f", rotation.x);
		ImGui::SameLine();
		ImGui::Text("Y = %f", rotation.y);
		ImGui::SameLine();
		ImGui::Text("Z = %f", rotation.z);
		ImGui::SameLine();
		ImGui::Text("W = %f", rotation.w);

		ImGui::TreePop();
	}
}

void CTransformation::SetPos(float3 pos)
{
	position = pos;
}

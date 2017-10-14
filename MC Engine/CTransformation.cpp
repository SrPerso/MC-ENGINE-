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

	name = "Component Transformation_";
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
		ImGui::Text("   Position:");

		ImGui::Text("\t X = %f",position.x);
		ImGui::SameLine();
		ImGui::Text("\t Y = %f", position.y);
		ImGui::SameLine(); 
		ImGui::Text("\t Z = %f", position.z);


		ImGui::Text("   Scale:");

		ImGui::Text("\t X = %f", scale.x);
		ImGui::SameLine();
		ImGui::Text("\t Y = %f", scale.y);
		ImGui::SameLine();
		ImGui::Text("\t Z = %f", scale.z);

		ImGui::Text("   Rotation:");
	
		ImGui::Text("\t X = %f", rotation.x);
		ImGui::SameLine();
		ImGui::Text("\t Y = %f", rotation.y);
		ImGui::SameLine();
		ImGui::Text("\t Z = %f", rotation.z);
		ImGui::SameLine();
		ImGui::Text("\t W = %f", rotation.w);

		ImGui::TreePop();
	}
}

void CTransformation::SetPos(float3 pos)
{
	position = pos;
}

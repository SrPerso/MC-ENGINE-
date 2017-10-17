#include "CTransformation.h"
#include "GameObject.h"


CTransformation::CTransformation(GameObject * object, Component_Type type) :Component(object, type)
{
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
}

void CTransformation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("   Position:");

		ImGui::Text("\t X = %.2f",position.x);
		ImGui::Text("\t Y = %.2f", position.y); 
		ImGui::Text("\t Z = %.2f", position.z);


		ImGui::Text("   Scale:");

		ImGui::Text("\t X = %.2f", scale.x);
		ImGui::Text("\t Y = %.2f", scale.y);
		ImGui::Text("\t Z = %.2f", scale.z);

		ImGui::Text("   Rotation:");
	
		ImGui::Text("\t X = %.2f", rotation.x);
		ImGui::Text("\t Y = %.2f", rotation.y);
		ImGui::Text("\t Z = %.2f", rotation.z);
		ImGui::Text("\t W = %.2f", rotation.w);

		ImGui::TreePop();
	}
}

void CTransformation::SetPos(float3 pos)
{
	position = pos;
}

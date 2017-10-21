#include "CTransformation.h"



CTransformation::CTransformation(GameObject * object, Component_Type type, DTransformation* data) :Component(object, type)
{
	moving = false;
	if (data)
	{
		 position = data->GetPosition();
		 scale = data->GetScale();
		 rotation = data->GetRotation();
		 destiny = data->GetPosition();

		 globalPosition = data->GetGlobalPosition();
		 globalScale = data->GetGlobalScale();
		 globalRotation = data->GetGlobalRotation();

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
	if (moving && object != nullptr)
	{
		object->Move(destiny, position);
		moving = false;
		position = destiny;
	}
}

void CTransformation::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		
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

#include "CTexture.h"
#include "GameObject.h"


CTexture::CTexture(GameObject* object, Component_Type type, DTexture * data) :Component(object, type)
{
	if (data)
	{
		this->image = data->image;	
		//this->Textname = nullptr;
		this->Textname.clear();
		this->Textname=data->Textname.c_str();
	}

	if (object != nullptr)
	{
		this->Texture_ID = object->NumComponentTypeSize(this->Ctype) + 1;
	}
	else
	{
		this->Texture_ID = 0;
	}

	name = "- Component Texture_";
	name.append(std::to_string(Texture_ID));
}

CTexture::~CTexture()
{
}

void CTexture::OnUpdate(float dt)
{

}

void CTexture::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		ImGui::Text("\t Texture path: %s", Textname.c_str());
		ImGui::TreePop();
	}
}


#include "CTexture.h"
#include "GameObject.h"

CTexture::CTexture(GameObject * object) :Component(object, COMP_TEXTURE)
{
	if (object != nullptr)
	{
		this->Texture_ID = object->NumComponentTypeSize(this->type) + 1;
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
	//if(name.length>0)
	if (ImGui::TreeNodeEx(name.c_str()))
	{
	//	ImGui::Text("\t Texture path: %s", Textname);
		ImGui::TreePop();
	}
}

DataTexture::~DataTexture()
{
}

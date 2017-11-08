#include "CTexture.h"
#include "GameObject.h"
#include "ModuleDataFile.h"

CTexture::CTexture(GameObject* object,int UID, Component_Type type, DTexture * data) :Component(object, UID, type)
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
	dType = D_TEXTURE;
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
		
		ImGui::TreePop();
	}
}

void CTexture::OnInspector()
{
	
		ImGui::Text("\t Texture path: %s", Textname.c_str());
		
		
}

void CTexture::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);
//	file.AddInt("Component Type", Ctype);


}

void CTexture::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");
}



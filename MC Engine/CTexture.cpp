#include "CTexture.h"
#include "GameObject.h"
#include "ModuleDataFile.h"
#include "Application.h"

CTexture::CTexture(GameObject* object,int UID, Component_Type type, DTexture * data) :Component(object, UID, type)
{
	SetData(data);

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

		ImGui::Text("\t Texture path: %s", dataTexture->textNamePath.c_str());
		ImGui::Text("\t Texture name: %s", dataTexture->textureName.c_str());

}

void CTexture::OnCleanUp()
{
}

void CTexture::OnSave(DataJSON & file) const
{
	file.AddInt("Component UID", UID);
	file.AddString("TextureName", dataTexture->textureName.c_str());
	file.AddString("textNamePath", dataTexture->textNamePath.c_str());
}

void CTexture::OnLoad(DataJSON & file)
{
	UID = file.GetFloat("Component UID");
	dataTexture->textureName.assign(file.GetString("TextureName"));
	dataTexture->textNamePath.assign(file.GetString("textNamePath"));

	DTexture* texture = new DTexture();
	texture = App->datamanager->importerTexture->Load(this, dataTexture->textNamePath.c_str(), this->object->GetGOUId());
	SetData(texture);

	delete texture;
}

void CTexture::SetData(DTexture * data)
{
	if (data != nullptr)
	{
		dataTexture = data;
	}
	else
		LOGUI("[ERROR]-Cant Set Data - Component texture");

}



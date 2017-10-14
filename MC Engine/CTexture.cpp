#include "CTexture.h"


CTexture::CTexture(GameObject * object) :Component(object, COMP_TEXTURE)
{

}

CTexture::~CTexture()
{
}

void CTexture::OnUpdate(float dt)
{
}

void CTexture::OnEditor()
{
	if (ImGui::TreeNodeEx("Mesh texture"))
	{
		ImGui::Text("Texture name: %s", name);
		ImGui::Text("Image: %i", image);
	}
}

DataTexture::~DataTexture()
{
}

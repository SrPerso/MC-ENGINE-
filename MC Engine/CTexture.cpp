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

DataTexture::~DataTexture()
{
}

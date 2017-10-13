#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"

class GameObject;

class DataTexture 
{
public:
	//texture
	uint idColors = 0;
	float* colors = nullptr;

	uint idTexCoords = 0;
	float* texCoords = nullptr;

public:
	DataTexture::~DataTexture();
};

class CTexture : public Component
{
public:
	CTexture(GameObject* object);
	~CTexture();

	void OnUpdate(float dt);


public:
	DataTexture* data;
};


#endif //_TEXTURE_H
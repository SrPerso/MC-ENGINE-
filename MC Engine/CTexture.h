#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"
#include <string>

class GameObject;

class DataTexture 
{
public:
	//texture
	uint idColors = 0;
	float* colors = nullptr;

	uint idTexCoords = 0;
	float* texCoords = nullptr;

	uint image=0;
	std::string name;
public:
	DataTexture::~DataTexture();
};

class CTexture : public Component, public DataTexture
{
public:
	CTexture(GameObject* object);
	~CTexture();

	void OnUpdate(float dt);

};


#endif //_TEXTURE_H
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"
#include <string>

class GameObject;

class DataTexture 
{
public:
	//texture	

	uint image=0;
	std::string Textname;	
public:
	DataTexture::~DataTexture();
};


class CTexture : public Component, public DataTexture
{
public:
	CTexture(GameObject* object, Component_Type type = COMP_TEXTURE);
	~CTexture();

	void OnUpdate(float dt);
	void OnEditor();
	uint Texture_ID; 
};
#endif //_TEXTURE_H
#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "Component.h"
#include <string>
#include "DTexture.h"

class GameObject;

class CTexture : public Component, public DTexture
{
public:
	CTexture(GameObject* object, Component_Type type , DTexture* data);
	~CTexture();

	void OnUpdate(float dt);
	void OnEditor();
	void OnInspector();
	uint Texture_ID; 
};
#endif //_TEXTURE_H
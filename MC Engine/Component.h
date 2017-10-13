#ifndef __COMPONENT_H__
#define __COMPONENT_H__
#include "Globals.h"
#include "CMesh.h"
#include "CTexture.h"


enum Component_Type {
	COMP_UNKNOWN,
	COMP_MESH,
	COMP_MATERIAL,
	COMP_CAMERA,
	COMP_SOUND
};

class GameObject;

class Component {
public:
	Component(GameObject* object, Component_Type type = COMP_UNKNOWN);
	virtual ~Component();

	Component_Type getType();

	bool IsEnable()const;
	void SetEnable(bool isEnable);
	void Enable();
	void Disable();

	virtual void OnUpdate(float dt);


	
protected:
	Component_Type type;
	GameObject* object = nullptr;
	bool isEnable = true;
	

};



#endif
#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include "Globals.h"

class GameObject;

enum Component_Type {
	COMP_UNKNOWN,
	COMP_MESH,
	COMP_TEXTURE,
	COMP_CAMERA,
	COMP_SOUND
};



class Component{
public:
	Component(GameObject* object, Component_Type type = COMP_UNKNOWN);
	virtual ~Component();

	Component_Type getType();

	virtual bool IsEnable()const;
	virtual void SetEnable(bool isEnable);
	virtual void Enable();
	virtual void Disable();

	virtual void OnUpdate(float dt);

protected:
	Component_Type type;
	GameObject* object = nullptr;
	bool isEnable = true;
	

};

#endif